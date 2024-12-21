// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QueryParamsDetailsView.h"
#include "QueryParamsEditor2.generated.h"

class UEditorUtilityComboBoxString;
class IHttpRequest;
class IHttpResponse;
class USinglePropertyView;

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
class METPAINTINGLOADER3_API UQueryParamsEditor2 : public UEditorUtilityWidget
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Search Params")
	FSearchParameters SearchParameters;

	// UPROPERTY(meta=(BindWidget))
	// USinglePropertyView* QueryParamsView;
	UPROPERTY(meta=(BindWidget))
	UDetailsView* QueryParamsDetailsView;

	UPROPERTY(meta=(BindWidget))
	UEditorUtilityComboBoxString* DepartmentsComboBox;

	virtual void NativeConstruct() override;

	// virtual void NativePreConstruct() override;

	// virtual void NativePreConstruct() override;
	virtual void OnWidgetRebuilt() override;

	void OnGetDepartmentsComplete(TSharedPtr<IHttpRequest> HttpRequest, TSharedPtr<IHttpResponse> HttpResponse, bool bSuccess);

	UFUNCTION(BlueprintCallable)
	FString GenerateQueryParams();
private:

	// TArray<FDepartmentEntry>
	FDepartmentsStruct Departments;
	
	void GetDepartments();

};
