// Fill out your copyright notice in the Description page of Project Settings.

#include "MetPaintingItem.h"

#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
#include "JsonObjectConverter.h"
#include "Blueprint/AsyncTaskDownloadImage.h"
#include "Engine/Texture2DDynamic.h"
#include "Interfaces/IHttpResponse.h"
#include "Windows/WindowsPlatformApplicationMisc.h"

void UMetPaintingItem::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMetPaintingItem::JLOnInfoDownloadComplete(TSharedPtr<IHttpRequest> Request,
                                                TSharedPtr<IHttpResponse> Response, bool bSuccess)
{
	if (!bSuccess)
	{
		// Todo: Fire a delegate that the image is loaded with a placeholder texture, or do something
		// to indicate that this failed. Not sure how I should handle that. 
		UE_LOG(LogTemp, Error, TEXT("Request failed"));
		// initialized will be false.
		RemoveFromParent();
		return;
	}
	IHttpResponse* HTTPResp = Response.Get();
	FString Content = HTTPResp->GetContentAsString();

	// FMetPaintingInfo PaintingInfo;
	Initialized = FJsonObjectConverter::JsonObjectStringToUStruct(Content, &PaintingInfo, 0, 0);

	if (!Initialized)
	{
		RemoveFromParent();
		return;
		
	}
	
	// This might need to emit a signal at this point when it's actually loaded the info and various properties can
	// actually be displayed. Otherwise, it might be bad trying to display undefined struct members.
	if (!PaintingInfo.primaryImageSmall.IsEmpty())
	{
		UAsyncTaskDownloadImage* DownloadImageTask = UAsyncTaskDownloadImage::DownloadImage(PaintingInfo.primaryImageSmall);
		DownloadImageTask->OnSuccess.AddDynamic(this, &UMetPaintingItem::JLOnPreviewImageDownloadComplete);
	} else if (!PaintingInfo.primaryImage.IsEmpty())
	{
		UAsyncTaskDownloadImage* DownloadImageTask = UAsyncTaskDownloadImage::DownloadImage(PaintingInfo.primaryImage);
		DownloadImageTask->OnSuccess.AddDynamic(this, &UMetPaintingItem::JLOnPreviewImageDownloadComplete);
	} else
	{
		// This widget has no images! Destroy it and remove it from the layout!
		RemoveFromParent();
	}
}


void UMetPaintingItem::JLInitializeAndLoadInfo(int ObjectID)
{
	auto Req = FHttpModule::Get().CreateRequest();
	Req->SetVerb("GET");

	Req->SetURL(URLBase + FString::FromInt(ObjectID));
	Req->OnProcessRequestComplete().BindUObject(this, &UMetPaintingItem::JLOnInfoDownloadComplete);
	Req->ProcessRequest();
}


void UMetPaintingItem::JLOnPrimaryImageHTTPComplete(TSharedPtr<IHttpRequest> HttpRequest,
	TSharedPtr<IHttpResponse> HttpResponse, bool bArg)
{
	TArray<uint8> RawImageData;
	if (HttpResponse.IsValid())
	{
		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			RawImageData = HttpResponse->GetContent();
		}
	} else
	{
		UE_LOG(LogTemp, Display, TEXT("HTTP Response is invalid"));
		return;
	}
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	// FImageUtils::Save
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(RawImageData.GetData(), RawImageData.Num());
	if (ImageFormat == EImageFormat::Invalid)
	{
		UE_LOG(LogTemp, Display, TEXT("Invalid image format"));
		return;
	}
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(ImageFormat);
	if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(RawImageData.GetData(), RawImageData.Num()))
	{
		UE_LOG(LogTemp, Display, TEXT("ImageWrapper invalid!"));
		return;
	}
	// TArray<uint8>* DecompressedData = nullptr;
	TArray64<uint8> DecompressedData;
	bool GetRawSucceeded = ImageWrapper->GetRaw(DecompressedData);
	// if (!ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, DecompressedData))
 //    {
 //        UE_LOG(LogTemp, Display, TEXT("Failed to get raw image data"));
 //        return;
 //    }

	if (!GetRawSucceeded)
	{
		UE_LOG(LogTemp, Display, TEXT("Get Decompressed data failed!"));
		return;
	}

	const int32 Width = ImageWrapper->GetWidth();
	const int32 Height = ImageWrapper->GetHeight();

	PrimaryImageTexture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
	if (!PrimaryImageTexture)
	{
		UE_LOG(LogTemp, Display, TEXT("Failed to create transient texture"));
		return;
	}

	FTexture2DMipMap& Mip = PrimaryImageTexture->GetPlatformData()->Mips[0];
	void* MipData = Mip.BulkData.Lock(LOCK_READ_WRITE); // have to lock this to prevent concurrent access.

	FMemory::Memcpy(MipData, DecompressedData.GetData(), DecompressedData.Num());

	Mip.BulkData.Unlock();
	PrimaryImageTexture->UpdateResource();
}


FReply UMetPaintingItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	// copy objectID to clipboard
	if (Initialized)
	{
		// Note: I think this will only work on windows!!!
		FPlatformApplicationMisc::ClipboardCopy(*(URLBase + FString::FromInt(PaintingInfo.objectID)));
		// if (GEditor)
		// {
		// 	FNotificationInfo Info(FText::FromString("Object ID copied to clipboard"));
		// 	Info.ExpireDuration = 5.0f;
		// 	FSlateNotificationManager::Get().AddNotification(Info);
		// }
		// FSlateApplication::Get().GetGameViewport() // I wonder if this gets the editor viewport if it's
		// in editor mode.
		if (!PaintingInfo.primaryImage.IsEmpty())
		{
			// UAsyncTaskDownloadImage* DownloadImageTask = UAsyncTaskDownloadImage::DownloadImage(PaintingInfo.primaryImage);
			// DownloadImageTask->OnSuccess.AddDynamic(this, &UMetPaintingItem::UMetPaintingItem::JLOnPrimaryImageDownloadComplete);

			// instead of using the DownloadImage task, I'm going to try directly getting the image data with http.

			auto Req = FHttpModule::Get().CreateRequest();
			Req->SetVerb("GET");
			Req->SetURL(PaintingInfo.primaryImage);
			Req->OnProcessRequestComplete().BindUObject(this, &UMetPaintingItem::JLOnPrimaryImageHTTPComplete);
			Req->ProcessRequest();
		}
	}
	
	return NativeReply;
}

void UMetPaintingItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (Initialized)
	{
		JLOnMouseEntered.Broadcast(true,PaintingInfo);
	}
}

void UMetPaintingItem::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	if (!GEditor)
	{
		UE_LOG(LogTemp, Display, TEXT("GEditor is null"));
		return;
	}
	FViewport* VP = GEditor->GetActiveViewport();
	if (!VP)
	{
		UE_LOG(LogTemp, Display, TEXT("Viewport is null"));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("Viewport type: %s"), *VP->GetViewportType().ToString());
	FEditorViewportClient* ViewportClient = static_cast<FEditorViewportClient*>(VP->GetClient());
	if (!ViewportClient)
	{
		UE_LOG(LogTemp, Warning, TEXT("No active viewport client found"));
		return;
	}

	// FVector2D MousePos;
	FViewportCursorLocation CursorLocation = ViewportClient->GetCursorWorldLocationFromMousePos();
	FIntPoint CP = CursorLocation.GetCursorPos();

	// CursorLocation.GetOrigin();


	// NOTE: there is a LOT of drag and drop functionality in LevelEditorViewport.h, on LevelEditorViewportClient class.
	TArray<UObject*> DroppedMeshes = {MeshToDrop};
	TArray<FTypedElementHandle> OutNewObjects;
	// This works, but it allows dropping items anywhere in the window, and it drops them at the wrong coordinates!
	// Doesn't seem to correctly drop items 
	ViewportClient->DropObjectsAtCoordinates(CP.X, CP.Y, DroppedMeshes, OutNewObjects);
}
