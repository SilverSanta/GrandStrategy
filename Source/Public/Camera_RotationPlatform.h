// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


#include "Camera_RotationPlatform.generated.h"

// FORWARD DECLARATIONS
class UBoxComponent;
class USceneComponent;


UCLASS()
class GRANDSTRATEGY_API ACamera_RotationPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACamera_RotationPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	void Set_PlatformDimensions();



private:
	// MEMBERS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "MARGIN IN TILES")
		float MarginInTiles = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "BOUNDARY")
		float Boundary_North;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "BOUNDARY")
		float Boundary_South;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "BOUNDARY")
		float Boundary_West;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "BOUNDARY")
		float Boundary_East;


private:
	// COMPONENTS
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Root")
		USceneComponent* m_RootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Box")
		UBoxComponent* m_BoxComponent;
};
