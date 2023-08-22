// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Globals.h"
#include "Chaos/Pair.h"

#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GRANDSTRATEGY_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance(){}

	UFUNCTION(BlueprintCallable)
		float Get_FirstTileCoordinates_Z();
	UFUNCTION(BlueprintCallable)
		FVector2D Get_TopLeftCorner_Coordinates();
	UFUNCTION(BlueprintCallable)
		FVector2D Get_BotRightCorner_Coordinates();


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "WORLD|SIZE")
		MapSize World_Size = MapSize::SMALL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "WORLD|COORDINATES")
		float FirstTileCoordinates_X = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "WORLD|COORDINATES")
		float FirstTileCoordinates_Y = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "WORLD|COORDINATES")
		float FirstTileCoordinates_Z = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "CAMERA_PLATFORM|COORDINATES")
		FVector2D TopLeftCorner_Coordinates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "CAMERA_PLATFORM|COORDINATES")
		FVector2D BotRightCorner_Coordinates;



	friend class ATileManager;
	friend class ACamera_RotationPlatform;
};
