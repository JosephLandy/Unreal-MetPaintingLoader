// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MetPaintingsTypes.h"
#include "PaintingMouseOverInfoBox.generated.h"
class UMetPaintingItem;
/**
 * 
 */
UCLASS()
class METPAINTINGLOADER3_API UPaintingMouseOverInfoBox : public UEditorUtilityWidget
{
	GENERATED_BODY()

private:
	

	UPROPERTY()
	class UCanvasPanel* CanvasParent;
	
public:
	// TSharedPtr<FMetPaintingInfo> PaintingInfo = nullptr;
	UPROPERTY(BlueprintReadOnly)
	bool bPaintingInfoSet = false;

	// UPROPERTY(BlueprintReadOnly);
	// FMetPaintingInfo PaintingInfo;

	UPROPERTY(BlueprintReadOnly, FieldNotify)
	UMetPaintingItem* PaintingItem;
	
	virtual void NativeConstruct() override;

	// note: Copying the struct like this probably isn't ideal. Having a hard time referencing structs...
	UFUNCTION(BlueprintCallable)
	void SetPaintingItem(UMetPaintingItem* Item);

	UFUNCTION(BlueprintCallable)
	void UnsetPaintingItem();
	
	
};
