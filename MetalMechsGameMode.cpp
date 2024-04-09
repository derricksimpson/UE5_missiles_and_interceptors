// Fill out your copyright notice in the Description page of Project Settings.


#include "MetalMechsGameMode.h"

void AMetalMechsGameMode::SetUserAction(FString Action)
{
	UserAction = Action;
}

void AMetalMechsGameMode::SetPlayerIsland(ABaseIsland* Island)
{
	PlayerIsland = Island;
}

void AMetalMechsGameMode::SetEnemyIsland(ABaseIsland* Island)
{
	EnemyIsland = Island;
}

ABaseIsland* AMetalMechsGameMode::GetPlayerIsland()
{
	return PlayerIsland;
}

ABaseIsland* AMetalMechsGameMode::GetEnemyIsland()
{
	return EnemyIsland;
}
