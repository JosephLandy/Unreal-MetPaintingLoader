#pragma once

#include "CoreMinimal.h"
#include "MetPaintingsTypes.generated.h"

enum class EImageFormat : int8;

USTRUCT(BlueprintType)
struct FTagItem
{
	GENERATED_BODY()

	FString term;
	FString AAT_URL;
	FString Wikidata_URL;
};

USTRUCT(BlueprintType)
struct FMetPaintingInfo
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	int objectID;
	UPROPERTY(BlueprintReadWrite)
	bool isHighlight;
	UPROPERTY(BlueprintReadWrite)
	FString accessionNumber;
	UPROPERTY(BlueprintReadWrite)
	FString accessionYear;
	UPROPERTY(BlueprintReadWrite)
	bool isPublicDomain;
	UPROPERTY(BlueprintReadWrite)
	FString primaryImage;
	UPROPERTY(BlueprintReadWrite)
	FString primaryImageSmall;
	// UPROPERTY(BlueprintReadWrite)
	// array additionalImages;
	// UPROPERTY(BlueprintReadWrite)
	// array constituents;
	UPROPERTY(BlueprintReadWrite)
	FString department;
	UPROPERTY(BlueprintReadWrite)
	FString objectName;
	UPROPERTY(BlueprintReadWrite)
	FString title;
	UPROPERTY(BlueprintReadWrite)
	FString culture;
	UPROPERTY(BlueprintReadWrite)
	FString period;
	UPROPERTY(BlueprintReadWrite)
	FString dynasty;
	UPROPERTY(BlueprintReadWrite)
	FString reign;
	UPROPERTY(BlueprintReadWrite)
	FString portfolio;
	UPROPERTY(BlueprintReadWrite)
	FString artistRole;
	UPROPERTY(BlueprintReadWrite)
	FString artistPrefix;
	UPROPERTY(BlueprintReadWrite)
	FString artistDisplayName;
	UPROPERTY(BlueprintReadWrite)
	FString artistDisplayBio;
	UPROPERTY(BlueprintReadWrite)
	FString artistSuffix;
	UPROPERTY(BlueprintReadWrite)
	FString artistAlphaSort;
	UPROPERTY(BlueprintReadWrite)
	FString artistNationality;
	UPROPERTY(BlueprintReadWrite)
	FString artistBeginDate;
	UPROPERTY(BlueprintReadWrite)
	FString artistEndDate;
	UPROPERTY(BlueprintReadWrite)
	FString artistGender;
	UPROPERTY(BlueprintReadWrite)
	FString artistWikidata_URL;
	UPROPERTY(BlueprintReadWrite)
	FString artistULAN_URL;
	UPROPERTY(BlueprintReadWrite)
	FString objectDate;
	UPROPERTY(BlueprintReadWrite)
	int objectBeginDate;
	UPROPERTY(BlueprintReadWrite)
	int objectEndDate;
	UPROPERTY(BlueprintReadWrite)
	FString medium;
	UPROPERTY(BlueprintReadWrite)
	FString dimensions;
	// UPROPERTY(BlueprintReadWrite)
	// float dimensionsParsed;
	// UPROPERTY(BlueprintReadWrite)
	// FString measurements;
	UPROPERTY(BlueprintReadWrite)
	FString creditLine;
	UPROPERTY(BlueprintReadWrite)
	FString geographyType;
	UPROPERTY(BlueprintReadWrite)
	FString city;
	UPROPERTY(BlueprintReadWrite)
	FString state;
	UPROPERTY(BlueprintReadWrite)
	FString county;
	UPROPERTY(BlueprintReadWrite)
	FString country;
	UPROPERTY(BlueprintReadWrite)
	FString region;
	UPROPERTY(BlueprintReadWrite)
	FString subregion;
	UPROPERTY(BlueprintReadWrite)
	FString locale;
	UPROPERTY(BlueprintReadWrite)
	FString locus;
	UPROPERTY(BlueprintReadWrite)
	FString excavation;
	UPROPERTY(BlueprintReadWrite)
	FString river;
	UPROPERTY(BlueprintReadWrite)
	FString classification;
	UPROPERTY(BlueprintReadWrite)
	FString rightsAndReproduction;
	UPROPERTY(BlueprintReadWrite)
	FString linkResource;
	UPROPERTY(BlueprintReadWrite)
	FString metadataDate;
	UPROPERTY(BlueprintReadWrite)
	FString repository;
	UPROPERTY(BlueprintReadWrite)
	FString objectURL;
	UPROPERTY(BlueprintReadWrite)
	TArray<FTagItem> tags;
	
	UPROPERTY(BlueprintReadWrite)
	FString objectWikidata_URL;
	UPROPERTY(BlueprintReadWrite)
	bool isTimelineWork;
	UPROPERTY(BlueprintReadWrite)
	FString GalleryNumber;
};

void SaveTextureAsAsset(UTexture2D* Texture, const FString& PackagePath, const FString& AssetName);

FString GetExtensionFromImageFormat(EImageFormat Format);

UTexture2D* ImportImageAsAsset(const FString& FilePath, const FString& TargetPath, const FString& AssetName);

UMaterialInstanceConstant* CreateMaterialInstanceAsset(UMaterialInterface* BaseMaterial, const FString& PackagePath,
	const FString& InstanceName);