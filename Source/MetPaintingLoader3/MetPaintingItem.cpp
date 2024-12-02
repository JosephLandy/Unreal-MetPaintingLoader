// Fill out your copyright notice in the Description page of Project Settings.


#include "MetPaintingItem.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Blueprint/AsyncTaskDownloadImage.h"
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

FReply UMetPaintingItem::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// copy objectID to clipboard
	if (Initialized)
	{
		// Note: I think this will only work on windows!!!
		FPlatformApplicationMisc::ClipboardCopy(*(URLBase + FString::FromInt(PaintingInfo.objectID)));
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
