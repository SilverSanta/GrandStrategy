// Fill out your copyright notice in the Description page of Project Settings.


#include "TileManager.h"
#include "TileComponent.h"
#include "Tile.h"
#include "Kismet/GameplayStatics.h" 

#include "MyGameInstance.h"



#define COLLISION_TILE ECollisionChannel::ECC_GameTraceChannel1


// Public
void ATileManager::_Set_GridDimensions()
{
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME INSTANCE FOUND\n"));

		Columns = GridSize.at(MyGameInstance->World_Size).first;
		Rows = GridSize.at(MyGameInstance->World_Size).second;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GAME INSTANCE FOUND\n"));
	}
}
void ATileManager::_Set_GridDimensions(MapSize size)
{
	Columns = GridSize.at(size).first;
	Rows = GridSize.at(size).second;
}
void ATileManager::_Set_GridDimensions(int columns, int rows)
{
	Columns = columns;
	Rows = rows;
}
void ATileManager::_Set_TileType(ATile* tile, TerrainType type)
{
}


// Private functions
void ATileManager::GenerateGrid()
{
	// (1) GENERATE 2D ARRAY OF LOCATIONS
	TArray<FVector2D> LocationGrid2D = Create_ArrayOfTileLocations(Columns, Rows);

	// (2) GENERATE BIOM AND HEIGHT ARRAY
	TArray<float> HeightPerlinArray = Create_ArrayOfPerlinNoiseValues(LocationGrid2D, PerlinNoiseXOffsetHeight, PerlinNoiseYOffsetHeight, PerlinNoisePotencyHeight, PerlinNoiseLacunarityHeight);
	TArray<float> BiomePerlinArray = Create_ArrayOfPerlinNoiseValues(LocationGrid2D, PerlinNoiseXOffsetBiomes, PerlinNoiseYOffsetBiomes, PerlinNoisePotencyBiomes, PerlinNoiseLacunarityBiomes);

	// (3) LIMITERS
	// Height related
	float WaterPerlinLimiter = Get_ValueAtWhichPercentageIsMet(HeightPerlinArray, HowMuchWater);
	float MountainsPerlinLimiter = Get_ValueAtWhichPercentageIsMet(HeightPerlinArray, 1.f - HowManyMountains);

	// Biome related
	float SnowPerlinLimiter = Get_ValueAtWhichPercentageIsMet(BiomePerlinArray, HowManySnowLands);
	float BurrenPerlinLimiter = Get_ValueAtWhichPercentageIsMet(BiomePerlinArray, HowManySnowLands + HowManyBurrenLands);
	float DesertsPerlinLimiter = Get_ValueAtWhichPercentageIsMet(BiomePerlinArray, 1.f - HowManyVolcanos - HowManyDeserts);
	float VolcanosPerlinLimiter = Get_ValueAtWhichPercentageIsMet(BiomePerlinArray, 1.f - HowManyVolcanos);

	// (4) ARRAYS OF BIOMES AND TERRAIN TYPES
	TArray<TerrainType> ArrayOfTerrainTypes = Create_ArrayOfTerrainTypes(HeightPerlinArray, WaterPerlinLimiter, MountainsPerlinLimiter);
	TArray<LandType> ArrayOfBiomes = Create_ArrayOfBiomes(BiomePerlinArray, SnowPerlinLimiter, BurrenPerlinLimiter, DesertsPerlinLimiter, VolcanosPerlinLimiter);

	// (5) SPAWN TILES
	TArray<ATile*> TileGrid = Create_TileGrid(LocationGrid2D, ArrayOfTerrainTypes, ArrayOfBiomes);

	// (6) SET NEIGHBOURS
	Set_NeighboursForTilesInGrid(TileGrid);
}
TArray<FVector2D> ATileManager::Create_ArrayOfTileLocations(int columns, int rows)
{
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	std::pair<float, float> Coordinates;

	if (MyGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME INSTANCE FOUND\n"));

		Coordinates = std::make_pair(MyGameInstance->FirstTileCoordinates_X, MyGameInstance->FirstTileCoordinates_Y);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GAME INSTANCE FOUND\n"));
	}
	TArray<FVector2D> LocationGrid;
	int32 NewTileIndex{ 0 };

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			bool IsEven = (i % 2 == 0);
			float Offset = IsEven ? 0.f : 0.5f;
			FVector2D Location = FVector2D(Coordinates.first + Constants::Sqrt3 * Constants::TileRadius * (j + Offset), Coordinates.second + 1.5 * Constants::TileRadius * i);
			int32 ScopedNewTileIndex = NewTileIndex;
			LocationGrid.Add(Location);
		}
	}
	return LocationGrid;
}
TArray<float> ATileManager::Create_ArrayOfPerlinNoiseValues(TArray<FVector2D>& grid, float perlinNoiseXOffset, float perlinNoiseYOffset, float perlinNoisePotency, float perlinNoiseLacunarity)
{
	// Array of perlin noise (it ranges from ~0.15 to ~0.85)...
	TArray<float> ArrayOfPerlinValues;
	for (auto& Location : grid)
	{
		ArrayOfPerlinValues.Add(Create_SinglePerlinNoiseValue(Location, perlinNoiseXOffset, perlinNoiseYOffset, perlinNoisePotency, perlinNoiseLacunarity));
	}

	// Preparing the scalar and minimal value 
	float PerlinScalar, MinPerlin;
	{
		float MaxPerlin, RangeOfPerlin;

		TArray<float> SortedArray = ArrayOfPerlinValues;
		SortedArray.Sort();

		MinPerlin = SortedArray[0];
		MaxPerlin = SortedArray[SortedArray.Num() - 1];
		RangeOfPerlin = MaxPerlin - MinPerlin;
		PerlinScalar = 1.f / RangeOfPerlin;
	}

	// Application of the scalar and minimal value to each object
	for (auto& PerlinValue : ArrayOfPerlinValues)
	{
		PerlinValue = (PerlinValue - MinPerlin) * PerlinScalar;
	}

	// Return
	return ArrayOfPerlinValues;
}
TArray<TerrainType>ATileManager::Create_ArrayOfTerrainTypes(TArray<float>& varArray, float waterlimiter, float mountainlimiter)
{
	TArray<TerrainType> ListOfTerrainTypes;
	for (auto& PerlinValue : varArray)
	{
		if (PerlinValue <= waterlimiter)
		{
			ListOfTerrainTypes.Add(TerrainType::WATER);
		}
		else if (PerlinValue > mountainlimiter)
		{
			ListOfTerrainTypes.Add(TerrainType::MOUNTAIN);
		}
		else
		{
			ListOfTerrainTypes.Add(TerrainType::LAND);
		}
	}
	return ListOfTerrainTypes;
}
TArray<LandType>ATileManager::Create_ArrayOfBiomes(TArray<float>& varArray, float varSnowLimiter, float varBurrenLimiter, float varDesertLimiter, float varVolcanoLimiter)
{
	TArray<LandType> ListOfTerrainTypes;
	for (auto& PerlinValue : varArray)
	{
		if (PerlinValue <= varSnowLimiter)
		{
			ListOfTerrainTypes.Add(LandType::SNOW);
		}
		else if (PerlinValue > varSnowLimiter && PerlinValue <= varBurrenLimiter)
		{
			ListOfTerrainTypes.Add(LandType::BURREN);
		}
		else if (PerlinValue > varDesertLimiter && PerlinValue <= varVolcanoLimiter)
		{
			ListOfTerrainTypes.Add(LandType::DESERT);
		}
		else if (PerlinValue > varVolcanoLimiter)
		{
			ListOfTerrainTypes.Add(LandType::VOLCANIC);
		}
		else
		{
			ListOfTerrainTypes.Add(LandType::FIELDS);
		}
	}
	return ListOfTerrainTypes;
}
TArray<ATile*> ATileManager::Create_TileGrid(TArray<FVector2D>& locationArray, TArray<TerrainType> terraintypeArray, TArray<LandType> biomeArray)
{
	// Cleaning if any Tiles are present
	{
		TArray<AActor*> BufferGrid;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATile::StaticClass(), BufferGrid);
		for (auto tile : BufferGrid)
		{
			tile->Destroy();
		}
	}

	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	float Default_Z;

	if (MyGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("GAME INSTANCE FOUND\n"));

		Default_Z = MyGameInstance->FirstTileCoordinates_Z;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO GAME INSTANCE FOUND\n"));

		Default_Z = 0.f;
	}

	TArray<ATile*> Grid;

	for (int32 i = 0; i < locationArray.Num(); i++)
	{

		int32 Index = i;
		FVector2D Location = locationArray[i];
		
		TerrainType CurrentTerrainType = terraintypeArray[i];
		LandType CurrentLandType = biomeArray[i];

		float Location_Z = (CurrentTerrainType == TerrainType::WATER) ? (Default_Z - 10.f) : (Default_Z);
		FVector CurrentLocation = FVector(Location.X, Location.Y, Location_Z);

		ATile* newTile;
		switch (CurrentTerrainType)
		{
		case TerrainType::LAND:
			newTile = GetWorld()->SpawnActor<ATile>(Tile_Land, CurrentLocation, FRotator(0.f));
			break;
		case TerrainType::WATER:
			newTile = GetWorld()->SpawnActor<ATile>(Tile_Water, CurrentLocation, FRotator(0.f));
			break;
		case TerrainType::MOUNTAIN:
			newTile = GetWorld()->SpawnActor<ATile>(Tile_Mountain, CurrentLocation, FRotator(0.f));
			break;
		default:
			newTile = GetWorld()->SpawnActor<ATile>(Tile_Land, CurrentLocation, FRotator(0.f));
			break;
		}

		//Setting individual tiles		
		newTile->SetActorLocation(CurrentLocation);
		newTile->m_TileComponent->TileIndex = Index;
		newTile->m_TileComponent->TerrainType = CurrentTerrainType;
		newTile->m_TileComponent->LandType = CurrentLandType;


		if (CurrentTerrainType == TerrainType::LAND)
		{
			switch (CurrentLandType)
			{
			case LandType::BURREN:
				newTile->m_TopMaterial = Material_Burren;
				break;
			case LandType::DESERT:
				newTile->m_TopMaterial = Material_Desert;
				break;
			case LandType::FIELDS:
				newTile->m_TopMaterial = Material_Grass;
				break;
			case LandType::SNOW:
				newTile->m_TopMaterial = Material_Snow;
				break;
			case LandType::VOLCANIC:
				newTile->m_TopMaterial = Material_Volcanic;
				break;
			}
		}

		if (newTile->m_TopMaterial)
		{
			newTile->m_StaticMeshComponent->SetMaterial(0, newTile->m_TopMaterial);
		}

		Grid.Add(newTile);
	}
	return Grid;
}
float ATileManager::Create_SinglePerlinNoiseValue(FVector2D varVector2D, float perlinNoiseXOffset, float perlinNoiseYOffset, float perlinNoisePotency, float perlinNoiseLacunarity)
{
	FVector2D ModifiedPerlinPosition = FVector2D(varVector2D.X + perlinNoiseXOffset, varVector2D.Y + perlinNoiseYOffset);

	float PrimaryPotency = FMath::Pow(perlinNoisePotency, 0);
	float SecondaryPotency = FMath::Pow(perlinNoisePotency, 1);
	float TertiaryPotency = FMath::Pow(perlinNoisePotency, 2);
	float PotencyScalar = (1.f / PrimaryPotency) + (1.f / SecondaryPotency) + (1.f / TertiaryPotency);

	float PrimaryFrequency = PrimaryPotency * perlinNoiseLacunarity;
	float SecondaryFrequency = SecondaryPotency * perlinNoiseLacunarity;
	float TertiaryFrequency = TertiaryPotency * perlinNoiseLacunarity;

	float RawPrimaryPerlinNoiseHeightAtPoint = FMath::PerlinNoise2D(ModifiedPerlinPosition / PrimaryFrequency) * PrimaryPotency / PotencyScalar;
	float RawSecondaryPerlinNoiseHeightAtPoint = FMath::PerlinNoise2D(ModifiedPerlinPosition / SecondaryFrequency) * SecondaryPotency / PotencyScalar;
	float RawTertiaryPerlinNoiseHeightAtPoint = FMath::PerlinNoise2D(ModifiedPerlinPosition / TertiaryFrequency) * TertiaryPotency / PotencyScalar;

	float RawPerlinNoiseHeightAtPoint = RawPrimaryPerlinNoiseHeightAtPoint + RawSecondaryPerlinNoiseHeightAtPoint + RawTertiaryPerlinNoiseHeightAtPoint;
	float NormalizedPerlinNoiseAtPoint = (RawPerlinNoiseHeightAtPoint + 1.f) / 2.f;
	return NormalizedPerlinNoiseAtPoint;
}
float ATileManager::Get_ValueAtWhichPercentageIsMet(TArray<float>&varArray, float PercentageValue)
{
	TArray<float> SortedArray = varArray;
	SortedArray.Sort();
	int32 LengthOfTheArray = varArray.Num();
	float HowManyShouldmeetTheCriteriumFloat = LengthOfTheArray * PercentageValue;
	int32 HowManyShouldmeetTheCriteriumInt = FMath::FloorToInt32(HowManyShouldmeetTheCriteriumFloat);
	if (HowManyShouldmeetTheCriteriumInt <= varArray.Num())
	{
		return SortedArray[HowManyShouldmeetTheCriteriumInt - 1] + 0.000001;
	}
	else
	{
		return 0;
	}
}
void ATileManager::Set_NeighboursForTilesInGrid(TArray<ATile*>& grid)
{
	for (auto tile : grid)
	{
		//Getting ready to launch sweep function
		TArray< struct FHitResult > OutHits;
		FVector TileLocationCheckStart = FVector(tile->GetActorLocation().X, tile->GetActorLocation().Y, tile->GetActorLocation().Z - 10000);
		FVector TileLocationCheckEnd = FVector(tile->GetActorLocation().X, tile->GetActorLocation().Y, tile->GetActorLocation().Z + 10000);
		FRotator TileRotator = tile->GetActorRotation();
		FCollisionShape Sphere = FCollisionShape::MakeSphere(2 * Constants::TileRadius);
		//DrawDebugSphere(GetWorld(), tile->GetActorLocation(), m_TileRadius, 32, FColor::Red, true, 10.f);

		//Launching the sweep function to get all the colliding tiles (including itself, unfortunatelly)
		bool CollisionCheck = GetWorld()->UWorld::SweepMultiByChannel(OutHits, TileLocationCheckStart, TileLocationCheckEnd, FQuat::Identity, COLLISION_TILE, Sphere);

		//Getting the neighbour list -> we will be looping excluding itself
		TArray<ATile*> NeighbourList{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		int32 ValidNeighbourCounter{ 0 };
		for (auto const& Hit : OutHits)
		{
			//Checking name of the hit actor against name of each Tile in game -> OPTIMIZE
			FString TileName = tile->GetActorNameOrLabel();
			FString HitActorName = Hit.GetActor()->GetActorNameOrLabel();
			if (HitActorName != TileName)
			{
				for (auto element : grid)
				{
					if (element->GetActorNameOrLabel() == HitActorName)
					{
						//Creating a vector representing the rotation between a tile and its neighbour
						FVector ElementLocation = tile->GetActorLocation();
						FVector HitActorLocation = element->GetActorLocation();
						FVector RotationVector = HitActorLocation - ElementLocation;
						float YawRot = RotationVector.Rotation().Yaw;
						//UE_LOG(LogTemp, Display, TEXT("YAW: %f"), YawRot);
						//Logic for replacing nullptr with neighbouring tile pointer at appropriate edge(0 -> NW, 1 -> WW, 2 -> SW, 3 -> SE, 4 -> EE, 5 -> NE)
						if (YawRot <= -50 && YawRot > -70) { NeighbourList[5] = element; ValidNeighbourCounter++; }
						else if ((YawRot <= 0 && YawRot > -10) || (YawRot >= 0 && YawRot < 10)) { NeighbourList[4] = element; ValidNeighbourCounter++; }
						else if (YawRot >= 50 && YawRot < 70) { NeighbourList[3] = element; ValidNeighbourCounter++; }
						else if (YawRot >= 110 && YawRot < 130) { NeighbourList[2] = element; ValidNeighbourCounter++; }
						else if ((YawRot >= 170 && YawRot <= 180) || (YawRot <= -170 && YawRot >= -180)) { NeighbourList[1] = element; ValidNeighbourCounter++; }
						else if (YawRot <= -110 && YawRot > -130) { NeighbourList[0] = element; ValidNeighbourCounter++; }
					}
				}
			}
		}
		//Setting proper neighbour data in the tile
		tile->m_TileComponent->Neighbours = NeighbourList;
		tile->m_TileComponent->NeighboursCount = ValidNeighbourCounter;
	}
}



// Sets default values
ATileManager::ATileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATileManager::BeginPlay()
{	
	Super::BeginPlay();	

	_Set_GridDimensions();
	GenerateGrid();
}

// Called every frame
void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

