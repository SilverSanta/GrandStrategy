// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// UE includes
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

// Custom include
#include "Globals.h"

// Generated include
#include "TileComponent.generated.h"



// FORWARD DECLARATIONS
class ATile;



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRANDSTRATEGY_API UTileComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTileComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;




	


private:

UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Tile Component")
	int TileIndex;
UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Tile Component")
	TerrainType TerrainType;
UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Tile Component")
	LandType LandType;
UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Tile Component")
	bool IsOccupied;	
UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Tile Component")
	PlayerID OwnedBy;
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Tile Component")
	TArray<ATile*> Neighbours{ nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Tile Component")
	int NeighboursCount;

friend class ATileManager;
};