// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetPaintingsWindow.h"
#include "Components/SinglePropertyView.h"
#include "MetPaintingWindowPropertyView.generated.h"

/**
 * 
 */
UCLASS()
class METPAINTINGLOADER3_API UMetPaintingWindowPropertyView : public USinglePropertyView
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable)
	void SetMetPaintingWindow(UMetPaintingsWindow* Window);
	
	
};
