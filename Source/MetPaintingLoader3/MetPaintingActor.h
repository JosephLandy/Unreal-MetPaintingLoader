// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MetPaintingsTypes.h"
#include "GameFramework/Actor.h"
#include "MetPaintingActor.generated.h"

UCLASS()
class METPAINTINGLOADER3_API AMetPaintingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMetPaintingActor();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PaintingPlane;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* BackgroundPlane;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Frame;

	UPROPERTY(BlueprintReadOnly)
	FMetPaintingInfo PaintingInfo;

	void SizePainting(float Width, float Height);

	void SetPaintingMaterial(UMaterialInterface* Mat);

protected:

public:

};
