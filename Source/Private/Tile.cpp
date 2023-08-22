// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "TileComponent.h"






// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Definition of components
	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	m_CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision Component"));
	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	m_TileComponent = CreateDefaultSubobject<UTileComponent>(TEXT("TileComponent"));

	// Setting attachments

	SetRootComponent(m_RootComponent);
	//m_RootComponent->SetupAttachment(RootComponent);
	m_CapsuleComponent->SetupAttachment(m_RootComponent);
	m_StaticMeshComponent->SetupAttachment(m_RootComponent);


	// Setting the material


	/*
		if(m_TopMaterial)
	{
		m_StaticMeshComponent->SetMaterial(0, m_TopMaterial);
	}
	if (m_SideMaterial)
	{		
		m_StaticMeshComponent->SetMaterial(1, m_SideMaterial);
	}
	if (m_SideMaterial)
	{
		m_StaticMeshComponent->SetMaterialByName("Material_002", m_SideMaterial);
	}
	
	*/

}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();	

	
	/*
	// Setting the material
	if (m_TopMaterial)
	{
		m_StaticMeshComponent->SetMaterial(0, m_TopMaterial);
	}
	if (m_SideMaterial)
	{
		m_StaticMeshComponent->SetMaterial(1, m_SideMaterial);
	}
	if (m_SideMaterial)
	{
		m_StaticMeshComponent->SetMaterialByName("Material_002", m_SideMaterial);
	}
	*/

}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

