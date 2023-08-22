// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <unordered_map>

namespace Constants
{
	constexpr float Sqrt2 = 1.414213562373095;
	constexpr float Sqrt3 = 1.7320508075688773;
	constexpr float Pi = 3.1415926535897932;
	constexpr float Rad = 57.295779513082;
	constexpr int TileRadius = 100;

}


UENUM(BlueprintType)
enum class MapSize : uint8 {

	SMALL	UMETA(DisplayName = "Small"),
	MEDIUM	UMETA(DisplayName = "Medium"),
	LARGE	UMETA(DisplayName = "Large"),
	EXTRA_LARGE	UMETA(DisplayName = "Extra Large"),
};

UENUM(BlueprintType)
enum class PlayerID: uint8{

	_NONE     UMETA(DisplayName = "Neutral"),
	PLAYER1   UMETA(DisplayName = "Player 1"),
	PLAYER2   UMETA(DisplayName = "Player 2"),
	PLAYER3   UMETA(DisplayName = "Player 3"),
	PLAYER4   UMETA(DisplayName = "Player 4"),
	PLAYER5   UMETA(DisplayName = "Player 5"),
	PLAYER6   UMETA(DisplayName = "Player 6"),
	PLAYER7   UMETA(DisplayName = "Player 7"),
	PLAYER8   UMETA(DisplayName = "Player 8"),
};

UENUM(BlueprintType)
enum class TerrainType : uint8 {

	LAND     UMETA(DisplayName = "Land"),
	WATER   UMETA(DisplayName = "Water"),
	MOUNTAIN   UMETA(DisplayName = "Mountain"),
};

UENUM(BlueprintType)
enum class LandType : uint8 {

	FIELDS     UMETA(DisplayName = "Fields"),
	DESERT   UMETA(DisplayName = "Desert"),
	BURREN   UMETA(DisplayName = "Burren"),
	SNOW   UMETA(DisplayName = "Snow"),
	VOLCANIC   UMETA(DisplayName = "Volcanic"),
};



std::unordered_map<MapSize, std::pair<int, int>> GridSize{

	std::make_pair(MapSize::SMALL, std::make_pair(21,11)),
	std::make_pair(MapSize::MEDIUM, std::make_pair(31,16)),
	std::make_pair(MapSize::LARGE, std::make_pair(41,21)),
	std::make_pair(MapSize::EXTRA_LARGE, std::make_pair(51,26)),
};