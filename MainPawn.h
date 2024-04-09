// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainPawn.generated.h"

UCLASS()
class METALMECHS_API AMainPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetCameraMovementEnabled(bool IsEnabled);

	UFUNCTION(BlueprintCallable)
	void SetAttackingBuilding(ABuilding* Building);

	UFUNCTION(BlueprintCallable)
	ABuilding* GetAttackingBuilding();

	UFUNCTION(BlueprintCallable)
	void SetAttackPosition(FVector Position);
	
private:

	float MouseX;

	float MouseY;

	float PrevMouseX = 0;
	
	float PrevMouseY = 0;

	bool IsMouseDown = false;

	bool IsCameraMovementEnabled = true;

	UPROPERTY(VisibleAnywhere)
	class ABuilding* AttackingBuilding;
	
	UPROPERTY(VisibleAnywhere, Category="Debug Info")
	uint64 TotalTicks = 0;
	
	UPROPERTY(VisibleAnywhere, Category="Camera Control")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category="Camera Control")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category="Drop Zone")
	TSubclassOf<AActor> DecalClass;
	
	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(VisibleAnywhere, Category="Camera Control")
	float MouseWheelSpeed = 100;
	
private:
	
	void MouseClick();

	void MouseClickUp();
	
	void MouseWheel(float Value);

};
