// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MetalMechsGameMode.generated.h"

class ABaseIsland;

/**
 * 
 */
UCLASS()
class METALMECHS_API AMetalMechsGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetUserAction(FString Action);

	UFUNCTION()
	void SetPlayerIsland(ABaseIsland* Island);
	
	UFUNCTION()
	void SetEnemyIsland(ABaseIsland* Island);

	UFUNCTION(BlueprintCallable)
	ABaseIsland* GetPlayerIsland();

	UFUNCTION(BlueprintCallable)
	ABaseIsland* GetEnemyIsland();

private:
	UPROPERTY(VisibleAnywhere)
	FString UserAction = "Idle";

	UPROPERTY()
	class ABaseIsland* PlayerIsland;

	UPROPERTY()
	class ABaseIsland* EnemyIsland;

};
