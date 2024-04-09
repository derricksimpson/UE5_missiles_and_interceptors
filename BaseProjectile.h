// Copyright 2021 to 2070 - Derrick Simpson - Sawmill Road Games - No-one else may use, modify, reverse engineer this code in any way.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

UCLASS()
class METALMECHS_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	/// <summary>
	/// Fires Blueprint to set up particle systems
	/// </summary>
	UFUNCTION(BlueprintImplementableEvent)
	void OnFireProjectile();

	UFUNCTION(BlueprintImplementableEvent)
	void OnBeginDestroy();

	void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Explosion Class", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> ExplosionClass;

	virtual void CleanUp();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
