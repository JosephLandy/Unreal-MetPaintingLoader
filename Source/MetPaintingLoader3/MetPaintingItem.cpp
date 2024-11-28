// Fill out your copyright notice in the Description page of Project Settings.


#include "MetPaintingItem.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Blueprint/AsyncTaskDownloadImage.h"
#include "Interfaces/IHttpResponse.h"

void UMetPaintingItem::JLOnInfoDownloadComplete(TSharedPtr<IHttpRequest> Request,
                                                TSharedPtr<IHttpResponse> Response, bool bSuccess)
{
	if (!bSuccess)
	{
		// Todo: Fire a delegate that the image is loaded with a placeholder texture, or do something
		// to indicate that this failed. Not sure how I should handle that. 
		UE_LOG(LogTemp, Error, TEXT("Request failed"));
		return;
	}
	IHttpResponse* HTTPResp = Response.Get();
	FString Content = HTTPResp->GetContentAsString();

	FMetPaintingInfo PaintingInfo;
	FJsonObjectConverter::JsonObjectStringToUStruct(Content, &PaintingInfo, 0, 0);

	// This might need to emit a signal at this point when it's actually loaded the info and various properties can
	// actually be displayed. Otherwise, it might be bad trying to display undefined struct members.
	
	UAsyncTaskDownloadImage* DownloadImageTask = UAsyncTaskDownloadImage::DownloadImage(PaintingInfo.primaryImageSmall);
	
	DownloadImageTask->OnSuccess.AddDynamic(this, &UMetPaintingItem::JLOnPreviewImageDownloadComplete);
}

void UMetPaintingItem::JLInitializeAndLoadInfo(int ObjectID)
{
	auto Req = FHttpModule::Get().CreateRequest();
	Req->SetVerb("GET");

	Req->SetURL(URLBase + FString::FromInt(ObjectID));
	Req->OnProcessRequestComplete().BindUObject(this, &UMetPaintingItem::JLOnInfoDownloadComplete);
	Req->ProcessRequest();
}
