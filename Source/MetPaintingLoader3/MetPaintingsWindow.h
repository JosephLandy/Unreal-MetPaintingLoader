// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MetPaintingsWindow.generated.h"

class IHttpResponse;
class IHttpRequest;
class UEditorUtilityComboBoxString;

USTRUCT()
struct FDepartmentEntry
{
	GENERATED_BODY()

	UPROPERTY()
	int departmentId;
	UPROPERTY()
	FString displayName;
};


USTRUCT()
struct FDepartmentsStruct
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDepartmentEntry> departments;
};



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

	// UPROPERTY(meta=(BindWidget), BlueprintReadOnly)
	// UEditorUtilityComboBoxString* TEMPEditorComboTest1;

	virtual void NativeConstruct() override;
	
	void OnGetDepartmentsComplete(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bSuccess);
	
	// void GetDepartments();
	
};
