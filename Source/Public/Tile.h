// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"


// FORWARDS DECLARATIONS
class UTileComponent;
class UStaticMeshComponent;
class UCapsuleComponent;
class USceneComponent;
class UMaterialInstanceDynamic;
class UMaterialInstance;

UCLASS()
class GRANDSTRATEGY_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;





private:
	// COMPONENTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Root")
		USceneComponent* m_RootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Capsule")
		UCapsuleComponent* m_CapsuleComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Mesh")
		UStaticMeshComponent* m_StaticMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Tile")
		UTileComponent* m_TileComponent;


	// FIELDS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Tile Material")
		UMaterialInstance* m_TopMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Side Material")
		UMaterialInstance* m_SideMaterial;
		//UMaterialInstanceDynamic* m_SideMaterial;


	friend class ATileManager;
};
