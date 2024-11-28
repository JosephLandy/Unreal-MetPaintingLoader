// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MetPaintingsWindow.generated.h"

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
	
	void QueryAPI(FString search);
	
};
