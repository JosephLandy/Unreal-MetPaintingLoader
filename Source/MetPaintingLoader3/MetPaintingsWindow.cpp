// Fill out your copyright notice in the Description page of Project Settings.


#include "MetPaintingsWindow.h"

#include "EditorUtilityWidgetComponents.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"


void UMetPaintingsWindow::SaveImageSourcePath(FString Path)
{
	GConfig->SetString(TEXT("MetPaintings"), TEXT("ImageSourcePath"), *Path, GEditorPerProjectIni);
}

void UMetPaintingsWindow::SaveAssetImportPath(FString Path)
{
	GConfig->SetString(TEXT("MetPaintings"), TEXT("AssetImportPath"), *Path, GEditorPerProjectIni);
}

FString UMetPaintingsWindow::LoadImageSourcePath()
{
	FString Result;
	GConfig->GetString(TEXT("MetPaintings"), TEXT("ImageSourcePath"), Result, GEditorPerProjectIni);
	return Result;
}

FString UMetPaintingsWindow::LoadAssetImportPath()
{
	return GConfig->GetStr(TEXT("MetPaintings"), TEXT("ImageSourcePath"), GEditorPerProjectIni);
}

void UMetPaintingsWindow::NativeConstruct()
{
	Super::NativeConstruct();

	FString ImagePath = LoadImageSourcePath();

	FString AssetPath = LoadAssetImportPath();

	if (!ImagePath.IsEmpty())
	{
		ImageSourcePath = ImagePath;
	}

	
}

void UMetPaintingsWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}



