// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPawn.h"

#include "BaseIsland.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMainPawn::AMainPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void AMainPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->SetShowMouseCursor(true);
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller not found. Did you forget to possess or make this the start pawn?"));
	}
}

// Called every frame
void AMainPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

// Called to bind functionality to input
void AMainPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Capture Mouse Wheel for sliding the Spring arm in/out 
	PlayerInputComponent->BindAxis(TEXT("MouseWheel"), this, &AMainPawn::MouseWheel);
	//
	// PlayerInputComponent->BindAction(TEXT("MouseButton"), IE_Pressed, this, &AMainPawn::MouseClick);
	// PlayerInputComponent->BindAction(TEXT("MouseButton"), IE_Released, this, &AMainPawn::MouseClickUp);
	// Take the provided UObject stuff, create some, kind of internal
	// data structure...
}

void AMainPawn::SetCameraMovementEnabled(bool IsEnabled) {
	IsCameraMovementEnabled = IsEnabled;
}

void AMainPawn::SetAttackingBuilding(ABuilding* Building)
{
	AttackingBuilding = Building;
}

ABuilding* AMainPawn::GetAttackingBuilding()
{
	return AttackingBuilding;
}

void AMainPawn::SetAttackPosition(FVector Position)
{
	
}

void AMainPawn::MouseClick()
{
	IsMouseDown = true;
}

void AMainPawn::MouseClickUp()
{
	IsMouseDown = false;
}

void AMainPawn::MouseWheel(float Value)
{
	// Move Camera Closer / Farther based on scroll wheel movement.
	SpringArm->TargetArmLength += Value * -MouseWheelSpeed;

	if (SpringArm->TargetArmLength < 100)
	{
		SpringArm->TargetArmLength = 100;
	}

	if (!PlayerController || !IsCameraMovementEnabled)
	{ 
		return;
	}

	// TODO: we may not want to
	
	int Width;
	int Height;

	PlayerController->GetMousePosition(MouseX, MouseY);

	// TODO: only update when the viewport changes
	PlayerController->GetViewportSize(Width, Height);

	const double DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	const int Speed = .05 * (SpringArm->TargetArmLength / 2);

	// Border influence sizing

	// TODO: Check for movement since last frame - and possibly skip
	constexpr int XBorder = 100;
	constexpr int YBorder = 60;

	int Vertical = 0;
	int Horizontal = 0;

	// get vertical influence
	if (MouseY < YBorder)
	{
		Vertical = -(YBorder - MouseY);
	}
	else if (MouseY > Height - YBorder)
	{
		Vertical = -(Height - YBorder - MouseY);
	}

	if (MouseX < XBorder)
	{
		Horizontal = -(XBorder - MouseX);
	}
	else if (MouseX > Width - XBorder)
	{
		Horizontal = -(Width - XBorder - MouseX);
	}

	FVector CameraMovement = (Camera->GetRightVector() * Horizontal * Speed * DeltaTime) + (Camera->GetForwardVector() * -Vertical * Speed * DeltaTime);
	CameraMovement.Z = 0;
	
	//AddActorWorldOffset(FVector(Horizontal * Speed * DeltaTime, Vertical * Speed * DeltaTime, 0.0f), false);
	AddActorWorldOffset(CameraMovement, false);

	FVector loc = GetActorLocation();

	if (loc.X < -4000) {
		loc.X = -4000;
	}

	if (loc.X > 14200) {
		loc.X = 14200;
	}

	if (loc.Y < -5044) {
		loc.Y = -5044;
	}

	if (loc.Y > 3952) {
		loc.Y = 3952;
	}

	SetActorLocation(loc);

	if (PrevMouseX == MouseX && PrevMouseY == MouseY)
	{
		return;
	}
	
	PrevMouseX = MouseX;
	PrevMouseY = MouseY;
	

	// Check for movement on our main island and show the decal for the grid location
	FHitResult hitResult;

	if (PlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, hitResult))
	{
		//DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 25.0f, 8, FColor::Red, false);

		if (hitResult.GetActor()->Tags.Num() > 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Actor name %s"), *hitResult.GetActor()->Tags[0].ToString());
		
			if (DecalClass && (hitResult.GetActor()->Tags[0] == "Island"))
			{
				ABaseIsland* Island = Cast<ABaseIsland>(hitResult.GetActor());
		
				Island->OnMouseMove(hitResult.Location);
			}

		}

	}

}
