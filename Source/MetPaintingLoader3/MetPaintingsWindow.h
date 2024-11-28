// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MetPaintingsWindow.generated.h"

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

	UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	UEditorUtilityComboBoxString* TEMPEditorComboTest1;

	virtual void NativeConstruct() override;
	
	void QueryAPI(FString search);

	void GetDepartments();
	// void OnGetDepartmentsComplete();
	
};
