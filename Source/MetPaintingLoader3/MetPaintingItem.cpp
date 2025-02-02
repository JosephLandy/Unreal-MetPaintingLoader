// Fill out your copyright notice in the Description page of Project Settings.

#include "MetPaintingItem.h"

#include "FileBrowserUtils.h"
#include "HttpModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
#include "InterchangeHelper.h"
#include "JsonObjectConverter.h"
#include "MetPaintingActor.h"
#include "MetPaintingsWindow.h"
#include "Blueprint/AsyncTaskDownloadImage.h"
#include "Engine/Texture2DDynamic.h"
#include "Interfaces/IHttpResponse.h"
#include "Materials/MaterialInstanceConstant.h"
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
	SanitizedTitle = PaintingInfo.title.Replace(TEXT(" "), TEXT("_"));
	SanitizedTitle = UFileBrowserUtils::SanitizeFilename(SanitizedTitle);
	
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
	// Determine image format.
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	EImageFormat ImageFormat = ImageWrapperModule.DetectImageFormat(RawImageData.GetData(), RawImageData.Num());
	if (ImageFormat == EImageFormat::Invalid)
	{
		UE_LOG(LogTemp, Display, TEXT("Invalid image format"));
		return;
	}
	FString Extension = GetExtensionFromImageFormat(ImageFormat);
	

	// Get 
	// Define the output file path (outside the Unreal project)
	// FString OutputFilePath = FPaths::ProjectDir() / TEXT("../../SavedImages/PrimaryImage") + Extension;
	FString ImageFileName =  SanitizedTitle + Extension;
	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner not set on MetPaintingItem"));
		return;
	}
	FString OutputFilePath = Owner->ImageSourcePath / ImageFileName;
	// Create the directory if it doesn't exist
	// FPaths::CreateDirectoryTree(*FPaths::GetPath(OutputFilePath));
	bool DirCreated = IFileManager::Get().MakeDirectory(*Owner->ImageSourcePath);
	// Save the raw image data to the file
	if (FFileHelper::SaveArrayToFile(RawImageData, *OutputFilePath))
	{
		UE_LOG(LogTemp, Log, TEXT("Image saved successfully to: %s"), *OutputFilePath);


		UTexture2D* ImportedTex = ImportImageAsAsset(
			OutputFilePath,
			Owner->AssetImportPath, // Target folder in Content Browser
			TEXT("T_") + SanitizedTitle           // Desired asset name
		);

		UMaterialInstanceConstant* Mat = CreateMaterialInstanceAsset(
			Owner->BasePaintingMat,
			Owner->AssetImportPath,
			TEXT("MI_") + SanitizedTitle
		);
		// Set the texture parameter value
		FMaterialParameterInfo ParamInfo(FName("PaintingTexture"));
		Mat->SetTextureParameterValueEditorOnly(ParamInfo, ImportedTex);
		InstantiateMetPaintingActor(Mat);
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save image to: %s"), *OutputFilePath);
	}
}

void UMetPaintingItem::InstantiateMetPaintingActor(UMaterialInstanceConstant* Mat)
{
	AMetPaintingActor* Painting = GetWorld()->SpawnActor<AMetPaintingActor>(MetPaintingActorClass);
	Painting->SetPaintingMaterial(Mat);
	float Width = PaintingInfo.measurements[0].elementMeasurements.Width;
	float Height = PaintingInfo.measurements[0].elementMeasurements.Height;
	Painting->SizePainting(Width, Height);
	Painting->SetActorRelativeLocation(FVector(0, 0, Height));
	
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
		// auto V = FSlateApplication::Get().GetGameViewport(); // I wonder if this gets the editor viewport if it's
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
