// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "MetPaintingItem.generated.h"

class IHttpResponse;
class IHttpRequest;

USTRUCT(BlueprintType)
struct FMetPaintingInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	int objectID;
	UPROPERTY()
	bool isHighlight;
	UPROPERTY()
	FString accessionNumber;
	UPROPERTY()
	FString accessionYear;
	UPROPERTY()
	bool isPublicDomain;
	UPROPERTY()
	FString primaryImage;
	UPROPERTY()
	FString primaryImageSmall;
	// UPROPERTY()
	// array additionalImages;
	// UPROPERTY()
	// array constituents;
	UPROPERTY()
	FString department;
	UPROPERTY()
	FString objectName;
	UPROPERTY()
	FString title;
	UPROPERTY()
	FString culture;
	UPROPERTY()
	FString period;
	UPROPERTY()
	FString dynasty;
	UPROPERTY()
	FString reign;
	UPROPERTY()
	FString portfolio;
	UPROPERTY()
	FString artistRole;
	UPROPERTY()
	FString artistPrefix;
	UPROPERTY()
	FString artistDisplayName;
	UPROPERTY()
	FString artistDisplayBio;
	UPROPERTY()
	FString artistSuffix;
	UPROPERTY()
	FString artistAlphaSort;
	UPROPERTY()
	FString artistNationality;
	UPROPERTY()
	FString artistBeginDate;
	UPROPERTY()
	FString artistEndDate;
	UPROPERTY()
	FString artistGender;
	UPROPERTY()
	FString artistWikidata_URL;
	UPROPERTY()
	FString artistULAN_URL;
	UPROPERTY()
	FString objectDate;
	UPROPERTY()
	int objectBeginDate;
	UPROPERTY()
	int objectEndDate;
	UPROPERTY()
	FString medium;
	UPROPERTY()
	FString dimensions;
	// UPROPERTY()
	// float dimensionsParsed;
	// UPROPERTY()
	// FString measurements;
	UPROPERTY()
	FString creditLine;
	UPROPERTY()
	FString geographyType;
	UPROPERTY()
	FString city;
	UPROPERTY()
	FString state;
	UPROPERTY()
	FString county;
	UPROPERTY()
	FString country;
	UPROPERTY()
	FString region;
	UPROPERTY()
	FString subregion;
	UPROPERTY()
	FString locale;
	UPROPERTY()
	FString locus;
	UPROPERTY()
	FString excavation;
	UPROPERTY()
	FString river;
	UPROPERTY()
	FString classification;
	UPROPERTY()
	FString rightsAndReproduction;
	UPROPERTY()
	FString linkResource;
	UPROPERTY()
	FString metadataDate;
	UPROPERTY()
	FString repository;
	UPROPERTY()
	FString objectURL;
	// UPROPERTY()
	// array tags;
	UPROPERTY()
	FString objectWikidata_URL;
	UPROPERTY()
	bool isTimelineWork;
	UPROPERTY()
	FString GalleryNumber;
};

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
