#include "MetPaintingsTypes.h"

#include "AssetToolsModule.h"
#include "IImageWrapper.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "EditorFramework/AssetImportData.h"
#include "Factories/TextureFactory.h"
#include "UObject/SavePackage.h"


// this doesn't seem to result in the texture actually being registered and appearing in the content browser. 
void SaveTextureAsAsset(UTexture2D* Texture, const FString& PackagePath, const FString& AssetName)
{
	if (!Texture)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveTextureAsAsset: Texture is null!"));
		return;
	}

	// Ensure the package path is valid
	if (PackagePath.IsEmpty() || AssetName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("SaveTextureAsAsset: Invalid package path or asset name!"));
		return;
	}
	// Create a unique package name
	FString UniquePackageName;
	FString UniqueAssetName;
	FAssetToolsModule& AssetToolsModule = FAssetToolsModule::GetModule();
	AssetToolsModule.Get().CreateUniqueAssetName(
		PackagePath + "/" + AssetName, TEXT(""),
		UniquePackageName, UniqueAssetName
	);

	UPackage* Package = CreatePackage(*UniquePackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT("SaveTextureAsAsset: Failed to create package!"));
		return;
	}

	// Rename the texture to place it into the package
	Texture->Rename(*UniqueAssetName, Package);

	// Mark the package as dirty and notify the asset registry
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(Texture);

	// Save the package to disk
	FString FilePath = FPackageName::LongPackageNameToFilename(UniquePackageName, FPackageName::GetAssetPackageExtension());

	// if (!UPackage::SavePackage(Package, Texture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *FilePath))
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("SaveTextureAsAsset: Failed to save package to disk!"));
	// 	return;
	// }

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Public | RF_Standalone;
	// UPackage::Save()
	if (!UPackage::SavePackage(Package, Texture, *FilePath, SaveArgs))
	{
		UE_LOG(LogTemp, Error, TEXT("SaveTextureAsAsset: Failed to save package to disk!"));
		return;
	}
	UE_LOG(LogTemp, Display, TEXT("SaveTextureAsAsset: Texture saved successfully at %s"), *FilePath);
}

FString GetExtensionFromImageFormat(EImageFormat Format)
{
	FString FileExtension;
	switch (Format)
	{
	case EImageFormat::PNG:
		FileExtension = TEXT(".png");
		break;
	case EImageFormat::JPEG:
		FileExtension = TEXT(".jpg");
		break;
	case EImageFormat::BMP:
		FileExtension = TEXT(".bmp");
		break;
	case EImageFormat::ICO:
		FileExtension = TEXT(".ico");
		break;
	case EImageFormat::EXR:
		FileExtension = TEXT(".exr");
		break;
	case EImageFormat::ICNS:
		FileExtension = TEXT(".icns");
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Unsupported image format"));
		return "";
	}
	return FileExtension;
}


// copied from: Plugins/Enterprise/DatasmithContent/Source/DatasmithContent/Private/DatasmithAssetImportData.cpp
// Helper function required since UObject doesn't have an interface to access AssetImportData
UAssetImportData* GetAssetImportData(UObject* Asset)
{
#if WITH_EDITOR
	if (UStaticMesh* Mesh = Cast<UStaticMesh>(Asset))
	{
		return Mesh->AssetImportData;
	}
	else if (UTexture* Texture = Cast<UTexture>(Asset))
	{
		return Texture->AssetImportData;
	}
	else if (UMaterialInterface* Material = Cast<UMaterialInterface>(Asset))
	{
		return Material->AssetImportData;
	}
#endif // WITH_EDITOR

	return nullptr;
}


/// NOTE: This doesn't link the imported texture with the source file, so it can't be modified and reimported.  
UTexture2D* ImportImageAsAsset(const FString& FilePath, const FString& TargetPath, const FString& AssetName)
{
	if (!FPaths::FileExists(FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("File does not exist: %s"), *FilePath);
		return nullptr;
	}

	// Ensure the target path is valid
	if (TargetPath.IsEmpty() || AssetName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid target path or asset name!"));
		return nullptr;
	}
	// Define the package name
	FString PackageName = FString::Printf(TEXT("%s/%s"), *TargetPath, *AssetName);

	// Ensure the package name is unique
	FString UniquePackageName;
	FString UniqueAssetName;

	const FAssetToolsModule& AssetToolsModule = FAssetToolsModule::GetModule();
	AssetToolsModule.Get().CreateUniqueAssetName(PackageName, TEXT(""), UniquePackageName, UniqueAssetName);
	
	UPackage* Package = CreatePackage(*UniquePackageName);
	if (!Package)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create package for: %s"), *FilePath);
		return nullptr;
	}

	UTextureFactory* TextureFactory = NewObject<UTextureFactory>();
	TextureFactory->AddToRoot(); // Prevent garbage collection during the process
	TextureFactory->SuppressImportOverwriteDialog();
	bool bOutOperationCanceled;
	UObject* ImportedAsset = TextureFactory->FactoryCreateFile(
		UTexture2D::StaticClass(),
		Package,
		*UniqueAssetName,
		RF_Public | RF_Standalone,
		FilePath,
		nullptr,
		GWarn,
		bOutOperationCanceled
	);
	// Check if the asset was imported successfully
	if (!ImportedAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to import texture: %s"), *FilePath);
		return nullptr;
	}

	// Set the source file path for reimporting
	if (UAssetImportData* ImportData = GetAssetImportData(ImportedAsset))
	{
		ImportData->UpdateFilenameOnly(FilePath);
	}

	
	// Mark the package dirty and notify the asset registry
	Package->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(ImportedAsset);

	UE_LOG(LogTemp, Log, TEXT("Texture imported successfully: %s"), *UniquePackageName);

	// Cast and return the imported texture
	return Cast<UTexture2D>(ImportedAsset);
}
