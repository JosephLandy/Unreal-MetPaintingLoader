// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MetPaintingsTypes.h"
#include "MetPaintingItem.generated.h"

class IHttpResponse;
class IHttpRequest;



/**
 * 
 */
UCLASS()
class METPAINTINGLOADER3_API UMetPaintingItem : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FString URLBase = "https://collectionapi.metmuseum.org/public/collection/v1/objects/";

	UPROPERTY(BlueprintReadOnly)
	bool Initialized = false;

	FMetPaintingInfo PaintingInfo;

	void JLOnInfoDownloadComplete(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bSuccess);
	
	UFUNCTION(BlueprintCallable)
	void JLInitializeAndLoadInfo(int ObjectID);

	UFUNCTION(BlueprintImplementableEvent)
	void JLOnPreviewImageDownloadComplete(UTexture2DDynamic* Texture);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
};
