// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DetailsView.h"
#include "QueryParamsDetailsView.generated.h"

/**
 * 
 */
UCLASS()
class METPAINTINGLOADER3_API UQueryParamsDetailsView : public UDetailsView
{
	GENERATED_BODY()
public:
	virtual void OnWidgetRebuilt() override;

	UPROPERTY(EditAnywhere, Category = "Query Params")
	FString TestStringProp = "";
	
	
};
