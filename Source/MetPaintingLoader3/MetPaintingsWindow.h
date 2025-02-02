// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MetPaintingsWindow.generated.h"

class IHttpResponse;
class IHttpRequest;
class UEditorUtilityComboBoxString;




/**
 * 
 */
UCLASS()
class METPAINTINGLOADER3_API UMetPaintingsWindow : public UEditorUtilityWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly)
	FString URLBase = "https://collectionapi.metmuseum.org/public/collection/v1/objects/";

	UPROPERTY(BlueprintReadWrite)
	FString ImageSourcePath = "C:/Users/joseph/Documents/MetPaintingImages";

	UPROPERTY(BlueprintReadWrite)
	FString AssetImportPath = "/Game/MetImages";

	UPROPERTY(EditAnywhere)
	UMaterialInterface* BasePaintingMat;

	UFUNCTION(BlueprintCallable)
	static void SaveImageSourcePath(FString Path);
	UFUNCTION(BlueprintCallable)
	static void SaveAssetImportPath(FString Path);

	UFUNCTION(BlueprintCallable)
	static FString LoadImageSourcePath();
	UFUNCTION(BlueprintCallable)
	static FString LoadAssetImportPath();

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	

private:
	// void PositionPaintingInfoMouseOver();
	
	
};
