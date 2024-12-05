// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MetPaintingsTypes.h"
#include "MetPaintingItem.generated.h"

class IHttpResponse;
class IHttpRequest;


// DECLARE_DELEGATE_OneParam(FJLMouseEnteredDelegate, FMetPaintingInfo)
// UDELEGATE()
// DECLARE_DYNAMIC_DELEGATE_OneParam(FJLOnMouseEnteredDelegate, FMetPaintingInfo, PaintingInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPaintingMouseOverDelegate, bool, MouseEntered, FMetPaintingInfo, PaintingInfo);


/**
 * 
 */
UCLASS()
class METPAINTINGLOADER3_API UMetPaintingItem : public UEditorUtilityWidget
{
	GENERATED_BODY()

private:
	
	
public:
	UPROPERTY(BlueprintReadOnly)
	FString URLBase = "https://collectionapi.metmuseum.org/public/collection/v1/objects/";

	UPROPERTY(BlueprintReadOnly)
	bool Initialized = false;

	UPROPERTY(BlueprintReadWrite)
	FMetPaintingInfo PaintingInfo;

	UPROPERTY(BlueprintAssignable)
	FPaintingMouseOverDelegate JLOnMouseEntered;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMesh> MeshToDrop;

	UPROPERTY(BlueprintReadWrite)
	UTexture2DDynamic* PrimaryImage;

	

	

	virtual void NativeConstruct() override;

	void JLOnInfoDownloadComplete(TSharedPtr<IHttpRequest> Request, TSharedPtr<IHttpResponse> Response, bool bSuccess);
	
	UFUNCTION(BlueprintCallable)
	void JLInitializeAndLoadInfo(int ObjectID);

	UFUNCTION(BlueprintImplementableEvent)
	void JLOnPreviewImageDownloadComplete(UTexture2DDynamic* Texture);

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	
	
};
