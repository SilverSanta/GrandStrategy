// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera_RotationPlatform.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "Globals.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


#define COLLISION_CAMERAPLATFORM ECollisionChannel::ECC_GameTraceChannel2

// Sets default values
ACamera_RotationPlatform::ACamera_RotationPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Definition of components
	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));

	// Setting attachments
	m_RootComponent->SetupAttachment(RootComponent);
	m_BoxComponent->SetupAttachment(m_RootComponent);
}

// Called when the game starts or when spawned
void ACamera_RotationPlatform::BeginPlay()
{
	Super::BeginPlay();

	Set_PlatformDimensions();

	m_BoxComponent->bHiddenInGame = false;	
}

// Called every frame
void ACamera_RotationPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACamera_RotationPlatform::Set_PlatformDimensions()
{	
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME INSTANCE FOUND\n"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GAME INSTANCE FOUND\n"));
	}

	// MARGIN OFFSET
	float MarginOffset_Dim_X = MarginInTiles * Constants::TileRadius * Constants::Sqrt3;
	float MarginOffset_Dim_Y = MarginInTiles * Constants::TileRadius * 1.5f;

	// GRID
	int Columns = (MyGameInstance) ? (GridSize.at(MyGameInstance->World_Size).first) : (3);
	int Rows = (MyGameInstance) ? (GridSize.at(MyGameInstance->World_Size).second) : (3);

	// BOX DIMENSIONS;
	float Dim_X = 0.5f * ((Columns + 0.5f) * Constants::TileRadius * Constants::Sqrt3) + MarginOffset_Dim_X;
	float Dim_Y = 0.5f * ((Rows)*Constants::TileRadius * 1.5f + Constants::TileRadius * 0.5f) + MarginOffset_Dim_Y;
	float Dim_Z = 2.f;

	// ROOT INIT
	float Origin_X = (MyGameInstance) ? (MyGameInstance->FirstTileCoordinates_X - (Constants::TileRadius * Constants::Sqrt3 * 0.5f) + Dim_X - MarginOffset_Dim_X) : (Dim_X);
	float Origin_Y = (MyGameInstance) ? (MyGameInstance->FirstTileCoordinates_Y - (Constants::TileRadius) + Dim_Y - MarginOffset_Dim_Y) : (Dim_Y);
	float Origin_Z = MyGameInstance->FirstTileCoordinates_Z + Dim_Z;


	FVector BoxDimensions = FVector(Dim_X, Dim_Y, Dim_Z);
	FVector Origin = FVector(Origin_X, Origin_Y, Origin_Z);

	Boundary_North = Origin.Y - BoxDimensions.Y;
	Boundary_South = Origin.Y + BoxDimensions.Y;
	Boundary_West = Origin.X - BoxDimensions.X;
	Boundary_East = Origin.X + BoxDimensions.X;

	MyGameInstance->TopLeftCorner_Coordinates = FVector2D(Boundary_West, Boundary_North);
	MyGameInstance->BotRightCorner_Coordinates = FVector2D(Boundary_East, Boundary_South);

	m_RootComponent->SetWorldLocation(Origin);
	m_BoxComponent->SetBoxExtent(BoxDimensions, true);

	m_BoxComponent->ShapeColor = FColor::Blue;

	//DRAW DEBUG
	//DrawDebugBox(GetWorld(), Origin, BoxDimensions, FColor::Red, true);
}

