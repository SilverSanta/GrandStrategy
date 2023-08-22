// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Globals.h"



#include "TileManager.generated.h"

class ATile;
class UMaterialInstance;
class USceneComponent;
class UBoxComponent;


UCLASS()
class GRANDSTRATEGY_API ATileManager : public AActor
{
	GENERATED_BODY()
	

public:
	void _Set_GridDimensions();
	void _Set_GridDimensions(MapSize size);
	void _Set_GridDimensions(int columns, int rows);
	void _Set_TileType(ATile* tile, TerrainType type); // Supposed to also set appropriate parameters and materials



public:	
	// Sets default values for this actor's properties
	ATileManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	void GenerateGrid();
	TArray<FVector2D> Create_ArrayOfTileLocations(int columns, int rows);
	TArray<float> Create_ArrayOfPerlinNoiseValues(TArray<FVector2D>& varGrid, float perlinNoiseXOffset, float perlinNoiseYOffset, float perlinNoisePotency, float perlinNoiseLacunarity);	
	TArray<TerrainType> Create_ArrayOfTerrainTypes(TArray<float>& varArray, float waterlimiter, float mountainlimiter);
	TArray<LandType> Create_ArrayOfBiomes(TArray<float>& varArray, float varSnowLimiter, float varBurrenLimiter, float varDesertLimiter, float varVolcanoLimiter);
	TArray<ATile*> Create_TileGrid(TArray<FVector2D>& locationArray, TArray<TerrainType> terraintypeArray, TArray<LandType> biomeArray);
	float Create_SinglePerlinNoiseValue(FVector2D varVector2D, float perlinNoiseXOffset, float perlinNoiseYOffset, float perlinNoisePotency, float perlinNoiseLacunarity);
	float Get_ValueAtWhichPercentageIsMet(TArray<float>& varArray, float PercentageValue);
	void Set_NeighboursForTilesInGrid(TArray<ATile*>& grid);


public:
	// GRID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", MakeEditWidget = true), Category = "GRID")
		TArray<ATile*> GridOfTiles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", MakeEditWidget = true), Category = "GRID")
		int Columns;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", MakeEditWidget = true), Category = "GRID")
		int Rows;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", MakeEditWidget = true), Category = "GRID")
		int Elevation;

private:	
	// HEIGHTMAP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "HEIGHT|Perlin Noise")
		float PerlinNoisePotencyHeight = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "HEIGHT|Perlin Noise")
		float PerlinNoiseLacunarityHeight = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "HEIGHT|Perlin Noise")
		float PerlinNoisePersistencyHeight = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "HEIGHT|Perlin Noise")
		float PerlinNoiseXOffsetHeight = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "HEIGHT|Perlin Noise")
		float PerlinNoiseYOffsetHeight = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "HEIGHT|Distribution")
		float HowMuchWater = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "HEIGHT|Distribution")
		float HowManyMountains = 0.2f;


	// BIOMES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Perlin Noise")
		float PerlinNoisePotencyBiomes = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Perlin Noise")
		float PerlinNoiseLacunarityBiomes = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Perlin Noise")
		float PerlinNoisePersistencyBiomes = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Perlin Noise")
		float PerlinNoiseXOffsetBiomes = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Perlin Noise")
		float PerlinNoiseYOffsetBiomes = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Distribution")
		float HowManyVolcanos = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Distribution")
		float HowManyDeserts = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Distribution")
		float HowManyFields = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Distribution")
		float HowManyBurrenLands = 0.15f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "BIOMES|Distribution")
		float HowManySnowLands = 0.1f;


	// TILE BP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "TILE BP")
		TSubclassOf<ATile> Tile_Land;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "TILE BP")
		TSubclassOf<ATile> Tile_Water;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "TILE BP")
		TSubclassOf<ATile> Tile_Mountain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "TILE BP")
		UMaterialInstance* Material_Grass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "TILE BP")
		UMaterialInstance* Material_Desert;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "TILE BP")
		UMaterialInstance* Material_Snow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "TILE BP")
		UMaterialInstance* Material_Volcanic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "TILE BP")
		UMaterialInstance* Material_Burren;
};
