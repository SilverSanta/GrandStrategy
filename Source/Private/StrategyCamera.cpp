// Fill out your copyright notice in the Description page of Project Settings.


#include "StrategyCamera.h"

#include <algorithm>

#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

#define COLLISION_CAMERAPLATFORM ECollisionChannel::ECC_GameTraceChannel2



// Sets default values
AStrategyCamera::AStrategyCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// COMPONENTS
	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	m_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRING ARM"));
	m_CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	// ATTACHMENTS
	m_RootComponent->SetupAttachment(RootComponent);
	m_SpringArmComponent->SetupAttachment(m_RootComponent);
	m_CameraComponent->SetupAttachment(m_SpringArmComponent);
}

// Called when the game starts or when spawned
void AStrategyCamera::BeginPlay()
{
	Super::BeginPlay();

	Init();	
}

// Called every frame
void AStrategyCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// DRAGGING LOGIC -> WORKS EVERY TICK
	if (CameraMode == CameraModeEnum::DRAGGING)
	{
		Dragging();
	}	
}

// Called to bind functionality to input
void AStrategyCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// BINDING
	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AStrategyCamera::LeftMouseButton_Pressed_CPP);
	InputComponent->BindAction("LeftMouseButton", IE_Released, this, &AStrategyCamera::LeftMouseButton_Released_CPP);

	InputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &AStrategyCamera::MiddleMouseButton_Pressed_CPP);
	InputComponent->BindAction("MiddleMouseButton", IE_Released, this, &AStrategyCamera::MiddleMouseButton_Released_CPP);

	InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &AStrategyCamera::RightMouseButton_Pressed_CPP);
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &AStrategyCamera::RightMouseButton_Released_CPP);

	InputComponent->BindAxis("MoveForwards", this, &AStrategyCamera::MoveForwards_CPP);
	InputComponent->BindAxis("MoveRight", this, &AStrategyCamera::MoveRight_CPP);
	InputComponent->BindAxis("Zoom_InOut", this, &AStrategyCamera::Zoom_InOut_CPP);
	InputComponent->BindAxis("RotateQE", this, &AStrategyCamera::RotateQE_CPP);

	InputComponent->BindAxis("MouseMovement_Horizontal", this, &AStrategyCamera::MouseMovement_Horizontal_CPP);
	InputComponent->BindAxis("MouseMovement_Vertical", this, &AStrategyCamera::MouseMovement_Vertical_CPP);
}

void AStrategyCamera::Init()
{
	Init_RootElevation();
	Init_SpringArmLength();
	Init_Pitch(VerticalRotation_InitialPitch);
	Init_Yaw(HorizontalRotation_InitialYaw);
	Init_MouseCursor();
}

void AStrategyCamera::Init_RootElevation()
{
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (MyGameInstance)
	{
		FVector ActorLocation = GetActorLocation();

		float Elevation = MyGameInstance->Get_FirstTileCoordinates_Z();
		FVector ActualLocation = FVector(this->GetActorLocation().X, this->GetActorLocation().X, Elevation);
		this->SetActorLocation(ActualLocation);
		UE_LOG(LogTemp, Warning, TEXT("ELEVATION: %f"), this->GetActorLocation().Z);
	}
}

void AStrategyCamera::Init_SpringArmLength()
{
	m_SpringArmComponent->TargetArmLength = SpringArm_DefaultLength;
	m_SpringArmComponent->bDoCollisionTest = false;
	UE_LOG(LogTemp, Warning, TEXT("SPRING ARM : %f"), m_SpringArmComponent->TargetArmLength);
}

void AStrategyCamera::Init_Pitch(float PitchValue)
{
	PitchValue = std::clamp(PitchValue, VerticalRotation_Min, VerticalRotation_Max);
	FRotator Rotation = FRotator(PitchValue, 0.f, 0.f);
	m_SpringArmComponent->SetRelativeRotation(Rotation);
	UE_LOG(LogTemp, Warning, TEXT("INIT PITCH : %f"), m_SpringArmComponent->GetRelativeRotation().Pitch);
}

void AStrategyCamera::Init_Yaw(float YawValue)
{
	FRotator CurrentRotation = this->GetActorRotation();
	this->SetActorRotation(FRotator(CurrentRotation.Pitch, YawValue, CurrentRotation.Roll));
	UE_LOG(LogTemp, Warning, TEXT("INIT YAW : %f"), this->GetActorRotation().Yaw);
}

void AStrategyCamera::Init_MouseCursor()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex); // 0 refers to the first player
	
	if (PlayerController)
	{
		PlayerController->EnableInput(PlayerController);
		PlayerController->bShowMouseCursor = true;
	}	
}

void AStrategyCamera::Check_PositionForBoundary()
{
	UMyGameInstance* MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (MyGameInstance)
	{
		FVector ActorLocation = GetActorLocation();

		FVector2D TopLeft = MyGameInstance->Get_TopLeftCorner_Coordinates();
		FVector2D BotRight = MyGameInstance->Get_BotRightCorner_Coordinates();

		float ActualX = std::clamp(ActorLocation.X, TopLeft.X, BotRight.X);
		float ActualY = std::clamp(ActorLocation.Y, TopLeft.Y, BotRight.Y);

		FVector ActualLocation = FVector(ActualX, ActualY, ActorLocation.Z);
		this->SetActorLocation(ActualLocation);
	}
}

void AStrategyCamera::Update_Speed_LerpToSpringArmLength()
{
	// Minimal speed <-> Minimal SpringArm

	float MinSpringArmLength = SpringArm_DefaultLength / ZoomFactor_Max;
	float MaxSpringArmLength = SpringArm_DefaultLength / ZoomFactor_Min;
	float CurrentSpringArmLength = m_SpringArmComponent->TargetArmLength;

	CameraSpeed_Current = CameraSpeed_Min + (CameraSpeed_Max - CameraSpeed_Min) * (CurrentSpringArmLength) / (MaxSpringArmLength- MinSpringArmLength);
}

void AStrategyCamera::Update_PositionBy(float XAxisValue, float YAxisValue)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float ChangeInPosition_X = XAxisValue * DeltaTime * CameraSpeed_Current;
	float ChangeInPosition_Y = YAxisValue * DeltaTime * CameraSpeed_Current;

	FVector ChangeInPosition = FVector(ChangeInPosition_X, ChangeInPosition_Y, 0.f);

	this->AddActorLocalOffset(ChangeInPosition);

	Check_PositionForBoundary();
}

void AStrategyCamera::Update_YawBy(float AngleChange)
{
	FRotator AddedRotation = FRotator(0.f, AngleChange, 0.f);
	this->AddActorLocalRotation(AddedRotation);
}

void AStrategyCamera::Start_Dragging()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex); // 0 refers to the first player

	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(COLLISION_CAMERAPLATFORM, false, HitResult);
		if (PlayerController->GetHitResultUnderCursor(COLLISION_CAMERAPLATFORM, false, HitResult))
		{
			Dragging_ReferencePoint = FVector2D(HitResult.Location.X, HitResult.Location.Y);
			CameraMode = CameraModeEnum::DRAGGING;
			Dragging_StartingLocation = GetActorLocation();
		}
	}
}

void AStrategyCamera::Stop_Dragging()
{
	CameraMode = CameraModeEnum::DEFAULT;
	Dragging_ReferencePoint = FVector2D(0.f, 0.f);
	Dragging_StartingLocation = FVector(0.f, 0.f, 0.f);
}

void AStrategyCamera::Dragging()
{
	FHitResult HitResult;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), PlayerIndex);
	if (PlayerController->GetHitResultUnderCursor(COLLISION_CAMERAPLATFORM, false, HitResult))
	{
		UE_LOG(LogTemp, Warning, TEXT("DRAGGING"));
		FVector2D Dragging_DraggedPoint = FVector2D(HitResult.Location.X, HitResult.Location.Y);
		FVector2D Normal = Dragging_ReferencePoint - Dragging_DraggedPoint;
		Normal.Normalize();
		float Distance = FVector2D::Distance(Dragging_DraggedPoint, Dragging_ReferencePoint);

		FVector ChangeInLocation = FVector(Dragging_Sensitivity * Distance * Normal, 0.f);
		FVector NewLocation = Dragging_StartingLocation + ChangeInLocation;

		this->SetActorLocation(NewLocation);
		Check_PositionForBoundary();
	}	
}

void AStrategyCamera::Rotate_Horizontally(float AxisValue, float Sensitivity)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float Value = DeltaTime * Sensitivity * AxisValue;

	Update_YawBy(Value);
}

void AStrategyCamera::Update_PitchBy(float AngleChange)
{
	float CurrentPitch = m_SpringArmComponent->GetComponentRotation().Pitch;
	
	float AddedPitch;
	if (CurrentPitch + AngleChange > VerticalRotation_Max)
	{
		AddedPitch = VerticalRotation_Max - CurrentPitch;
	}
	else if (CurrentPitch + AngleChange < VerticalRotation_Min)
	{
		AddedPitch = VerticalRotation_Min - CurrentPitch;
	}
	else
	{
		AddedPitch = AngleChange;
	}
	
	FRotator AddedRotation = FRotator(AddedPitch, 0.f, 0.f);
	m_SpringArmComponent->AddRelativeRotation(AddedRotation);
}

void AStrategyCamera::Rotate_Vertically(float axisValue)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	float AngleChange = axisValue * VerticalRotation_Sensitivity * DeltaTime;

	Update_PitchBy(AngleChange);
}

void AStrategyCamera::Zoom_In()
{	
	// (1) Increase zoom factor
	Increase_ZoomFactor();

	// (2) Calculate SpringArm length
	Update_SpringArmLength();

	// (3) Update Speed
	Update_Speed_LerpToSpringArmLength();
}

void AStrategyCamera::Zoom_Out()
{
	// (1) Decrease zoom factor
	Decrease_ZoomFactor();

	// (2) Calculate SpringArm length
	Update_SpringArmLength();

	// (3) Update Speed
	Update_Speed_LerpToSpringArmLength();
}

void AStrategyCamera::Increase_ZoomFactor()
{
	ZoomFactor_Current *= ZoomFactor_Multiplier;
	//auto Zoom = FMath::Clamp(ZoomFactor_Current, ZoomFactor_Min, ZoomFactor_Max);
	ZoomFactor_Current = std::clamp(ZoomFactor_Current, ZoomFactor_Min, ZoomFactor_Max);
	UE_LOG(LogTemp, Warning, TEXT("Current zoomfactor: %f"), ZoomFactor_Current);
}

void AStrategyCamera::Decrease_ZoomFactor()
{
	ZoomFactor_Current /= ZoomFactor_Multiplier;
	//auto Zoom = FMath::Clamp(ZoomFactor_Current, ZoomFactor_Min, ZoomFactor_Max);
	ZoomFactor_Current = std::clamp(ZoomFactor_Current, ZoomFactor_Min, ZoomFactor_Max);
	UE_LOG(LogTemp, Warning, TEXT("Current zoomfactor: %f"), ZoomFactor_Current);
}

void AStrategyCamera::Update_SpringArmLength()
{
	m_SpringArmComponent->TargetArmLength = SpringArm_DefaultLength / ZoomFactor_Current;
}

void AStrategyCamera::LeftMouseButton_Pressed_CPP()
{
	UE_LOG(LogTemp, Warning, TEXT("LMB PRESSED"));
}

void AStrategyCamera::LeftMouseButton_Released_CPP()
{
	UE_LOG(LogTemp, Warning, TEXT("LMB RELEASED"));
}

void AStrategyCamera::MiddleMouseButton_Pressed_CPP()
{
	if(CameraMode == CameraModeEnum::DEFAULT)
	{
		CameraMode = CameraModeEnum::DRAGGING;
		Start_Dragging();
	}
}

void AStrategyCamera::MiddleMouseButton_Released_CPP()
{
	CameraMode = CameraModeEnum::DEFAULT;
	Stop_Dragging();
}

void AStrategyCamera::RightMouseButton_Pressed_CPP()
{
	if (CameraMode == CameraModeEnum::DEFAULT)
	{
		CameraMode = CameraModeEnum::ROTATING;
	}
}

void AStrategyCamera::RightMouseButton_Released_CPP()
{
	CameraMode = CameraModeEnum::DEFAULT;
}

void AStrategyCamera::MoveForwards_CPP(float value)
{
	if (CameraMode == CameraModeEnum::DEFAULT)
	{
		if (value > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("FORWARDS"));
		}
		else if (value < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("BACKWARDS"));
		}

		Update_PositionBy(value, 0.f);
	}
}

void AStrategyCamera::MoveRight_CPP(float value)
{
	if (CameraMode == CameraModeEnum::DEFAULT)
	{
		if (value > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("RIGHT"));
		}
		else if (value < 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("LEFT"));
		}

		Update_PositionBy(0.f, value);
	}
}

void AStrategyCamera::Zoom_InOut_CPP(float value)
{
	if (CameraMode == CameraModeEnum::DEFAULT)
	{
		if (value > 0)
		{
			Zoom_In();
		}
		else if (value < 0)
		{
			Zoom_Out();
		}
	}
}

void AStrategyCamera::RotateQE_CPP(float value)
{
	if (CameraMode == CameraModeEnum::DEFAULT)
	{
		Rotate_Horizontally(value, HorizontalRotation_KeyboardSensitivity);
	}	
}

void AStrategyCamera::MouseMovement_Horizontal_CPP(float value)
{
	if (CameraMode == CameraModeEnum::ROTATING)
	{
		Rotate_Horizontally(value, HorizontalRotation_MouseSensitivity);
	}
}

void AStrategyCamera::MouseMovement_Vertical_CPP(float value)
{
	/*

	if (value > 0)
{
	UE_LOG(LogTemp, Warning, TEXT("Y ++"));
}
else if (value < 0)
{
	UE_LOG(LogTemp, Warning, TEXT("Y --"));
}
*/

	if (CameraMode == CameraModeEnum::ROTATING)
	{
		Rotate_Vertically(value);
	}
}
