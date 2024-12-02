// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingMouseOverInfoBox.h"

#include "Components/CanvasPanel.h"


void UPaintingMouseOverInfoBox::NativeConstruct()
{
	Super::NativeConstruct();
	CanvasParent = Cast<UCanvasPanel>(GetParent());
}

void UPaintingMouseOverInfoBox::SetPaintingItem(UMetPaintingItem* Item)
{
	bPaintingInfoSet = true;

	PaintingItem = Item;
}

void UPaintingMouseOverInfoBox::ResetPaintingInfo()
{
	PaintingItem = nullptr;
}
