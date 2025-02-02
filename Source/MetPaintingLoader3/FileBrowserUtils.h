// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FileBrowserUtils.generated.h"

/**
 * 
 */
UCLASS()
class METPAINTINGLOADER3_API UFileBrowserUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static void OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, TArray<FString>& OutFileNames);

	UFUNCTION(BlueprintCallable)
	static void OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolder);

	static FString SanitizeFilename(const FString& InputString);
};
