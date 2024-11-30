// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DetailsView.h"
#include "QueryParamsDetailsView.generated.h"

USTRUCT(BlueprintType)
struct FSearchParameters
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Use_q = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="Use_q", EditConditionHides))
	FString q;
	UPROPERTY(EditAnywhere)
	bool Use_isHighlight = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_isHighlight", EditConditionHides))
	bool isHighlight;
	UPROPERTY(EditAnywhere)
	bool Use_title = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_title", EditConditionHides))
	bool title;
	UPROPERTY(EditAnywhere)
	bool Use_tags = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_tags", EditConditionHides))
	bool tags;
	UPROPERTY(EditAnywhere)
	bool Use_departmentId = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_departmentId", EditConditionHides))
	int departmentId;
	UPROPERTY(EditAnywhere)
	bool Use_isOnView = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_isOnView", EditConditionHides))
	bool isOnView;
	UPROPERTY(EditAnywhere)
	bool Use_artistOrCulture = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_artistOrCulture", EditConditionHides))
	bool artistOrCulture;
	UPROPERTY(EditAnywhere)
	bool Use_medium = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_medium", EditConditionHides))
	FString medium;

	// We want to ALWAYS include "hasImages" in the search.
	// UPROPERTY(EditAnywhere)
	// bool Use_hasImages = false;
	// UPROPERTY(EditAnywhere, meta=(EditCondition="Use_hasImages", EditConditionHides))
	// bool hasImages;
	UPROPERTY(EditAnywhere)
	bool Use_geoLocation = false;
	// UPROPERTY(EditAnywhere, meta=(EditCondition="Use_geoLocation", EditConditionHides, ColumnWidth="0.5")) // Is this column width thing real?
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_geoLocation", EditConditionHides, ColumnWidth="0.5"))
	FString geoLocation;
	UPROPERTY(EditAnywhere)
	bool Use_dateBegin = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_dateBegin", EditConditionHides))
	int dateBegin;
	UPROPERTY(EditAnywhere)
	bool Use_dateEnd = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="Use_dateEnd", EditConditionHides))
	int dateEnd;
	
};



/**
 * 
 */
UCLASS()
class METPAINTINGLOADER3_API UQueryParamsDetailsView : public UDetailsView
{
	GENERATED_BODY()
public:
	virtual void OnWidgetRebuilt() override;

	UPROPERTY(EditAnywhere, Category = "Search Params")
	FSearchParameters SearchParameters;


	UFUNCTION(BlueprintCallable)
	FString GenerateQueryParams();
};
