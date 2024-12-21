// Fill out your copyright notice in the Description page of Project Settings.


#include "MetPaintingActor.h"

// Sets default values
AMetPaintingActor::AMetPaintingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	Frame = CreateDefaultSubobject<UStaticMeshComponent>("Frame");
	// SetRootComponent(Frame);
	Frame->SetupAttachment(RootComponent);
	// frame should be 1 meter by 1 meter as well. Scale it slightly more than the plane!

	BackgroundPlane = CreateDefaultSubobject<UStaticMeshComponent>("BackgroundPlane");
	BackgroundPlane->SetupAttachment(Frame);
	
	PaintingPlane = CreateDefaultSubobject<UStaticMeshComponent>("PaintingPlane");
	
	PaintingPlane->SetupAttachment(RootComponent);
	// PaintingPlane->SetRelativeRotation(	FRotator(0, 0, 90)); // do in blueprint!

	// Load the Plane Static Mesh from the Engine Content directory
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMesh(TEXT("/Engine/BasicShapes/Plane.Plane"));
    
	if (PlaneMesh.Succeeded())
	{
		PaintingPlane->SetStaticMesh(PlaneMesh.Object);
		UE_LOG(LogTemp, Log, TEXT("Successfully set PaintingPlane static mesh."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load PaintingPlane static mesh."));
	}
}

void AMetPaintingActor::SizePainting(float Width, float Height)
{
	FVector PaintingScale {Width/100, Height/100, 1};
	PaintingPlane->SetRelativeScale3D(PaintingScale);
	float FrameScaleOffset = 0.01;
	Frame->SetRelativeScale3D(PaintingScale + FVector(FrameScaleOffset));
}

void AMetPaintingActor::SetPaintingMaterial(UMaterialInterface* Mat)
{
	// UMaterialInstanceConstant is a subclass of UMaterialInterface, so this should work.
	PaintingPlane->SetMaterial(0, Mat);
}

