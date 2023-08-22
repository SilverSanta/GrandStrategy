// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "StrategyCamera.generated.h"


// FORWARD DECLARATIONS
class USceneComponent;
class USpringArmComponent;
class UCameraComponent;

UENUM()
enum class CameraModeEnum :uint8
{
	DEFAULT     UMETA(DisplayName = "Default"),
	ROTATING	UMETA(DisplayName = "Rotating"),
	DRAGGING	UMETA(DisplayName = "Dragging"),
};

UCLASS()
class GRANDSTRATEGY_API AStrategyCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AStrategyCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// INIT
public:
	UFUNCTION(BlueprintCallable)
		void Init();
private:
		void Init_SpringArmLength();
		void Init_RootElevation();
		void Init_Pitch(float PitchValue);
		void Init_Yaw(float YawValue);
		void Init_MouseCursor();


	// MOVEMENT
public:
	UFUNCTION(BlueprintCallable)
		void Update_PositionBy(float XAxisValue, float YAxisValue);
private:
	void Check_PositionForBoundary();
	void Update_Speed_LerpToSpringArmLength();


	// DRAGGING
public:
	UFUNCTION(BlueprintCallable)
		void Start_Dragging();
	UFUNCTION(BlueprintCallable)
		void Stop_Dragging();
	UFUNCTION(BlueprintCallable)
		void Dragging();

	// HORIZONTAL ROTATION
public:
	UFUNCTION(BlueprintCallable)
		void Rotate_Horizontally(float axisValue, float Sensitivity);
private:
	void Update_YawBy(float AngleChange);


	// VERTICAL ROTATION
public:
	UFUNCTION(BlueprintCallable)
		void Rotate_Vertically(float axisValue);
private:
	void Update_PitchBy(float AngleChange);


	// ZOOM
public:
	UFUNCTION(BlueprintCallable)
		void Zoom_In();
	UFUNCTION(BlueprintCallable)
		void Zoom_Out();
private:
	void Increase_ZoomFactor();
	void Decrease_ZoomFactor();
	void Update_SpringArmLength();


	// CPP INPUT FUNCTIONS
private:
	void LeftMouseButton_Pressed_CPP();
	void LeftMouseButton_Released_CPP();

	void MiddleMouseButton_Pressed_CPP();
	void MiddleMouseButton_Released_CPP();

	void RightMouseButton_Pressed_CPP();
	void RightMouseButton_Released_CPP();

	void MoveForwards_CPP(float value);
	void MoveRight_CPP(float value);
	void Zoom_InOut_CPP(float value);
	void RotateQE_CPP(float value);

	void MouseMovement_Horizontal_CPP(float value);
	void MouseMovement_Vertical_CPP(float value);


	// COMPONENTS
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Root")
		USceneComponent* m_RootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Spring Arm")
		USpringArmComponent* m_SpringArmComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Camera")
		UCameraComponent* m_CameraComponent;


	// CAMERA MODE
	CameraModeEnum CameraMode = CameraModeEnum::DEFAULT;


	// PLAYER INDEX
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "PLAYER")
		int PlayerIndex = 0;


	// ZOOM
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ZOOM")
		float SpringArm_DefaultLength = 1200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ZOOM")
		float ZoomFactor_Current = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ZOOM")
		float ZoomFactor_Max = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ZOOM")
		float ZoomFactor_Min = 0.25f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ZOOM")
		float ZoomFactor_Multiplier = 1.5f;


	// MOVEMENT SPEED
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "MOVEMENT SPEED")
		float CameraSpeed_Current = 600;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "MOVEMENT SPEED")
		float CameraSpeed_Max = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "MOVEMENT SPEED")
		float CameraSpeed_Min = 400;


	// HORIZONTAL ROTATION
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|HORIZONTAL")
		float HorizontalRotation_Speed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|HORIZONTAL")
		FVector HorizontalRotation_HitPoint;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|HORIZONTAL")
		float HorizontalRotation_MouseSensitivity = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|HORIZONTAL")
		float HorizontalRotation_KeyboardSensitivity = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|HORIZONTAL")
		float HorizontalRotation_InitialYaw = 0.f;


	// VERTICAL ROTATION
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|VERTICAL")
		float VerticalRotation_InitialPitch = -30.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|VERTICAL")
		float VerticalRotation_Max = -15.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|VERTICAL")
		float VerticalRotation_Min = -60.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "ROTATION|VERTICAL")
		float VerticalRotation_Sensitivity = 1000.f;


	// DRAGGING
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DRAGGING")
		FVector2D Dragging_ReferencePoint = FVector2D(0.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DRAGGING")
		FVector Dragging_StartingLocation = FVector(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "DRAGGING")
		float Dragging_Sensitivity = 0.5f;
};
