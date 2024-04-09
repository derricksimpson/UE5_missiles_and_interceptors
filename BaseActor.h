#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.generated.h"

UCLASS()
class METALMECHS_API ABaseActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseActor();

	UPROPERTY(EditDefaultsOnly)
	bool CanAttack;

	UPROPERTY(VisibleAnywhere);
	bool IsPlayerActor;

	UFUNCTION()
	void SetLocation(float X, float Y);

	UFUNCTION()
	void OnTakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable)
	bool IsDestroyed();

	UFUNCTION()
	void OnDestroyed();

	UFUNCTION(BlueprintImplementableEvent)
	void OnDestroyedBP();

	UFUNCTION()
	void PerformAttack(FVector AttackLocation, FVector AttackTileLocation, class ABaseIsland *TargetIsland);

	UFUNCTION(BlueprintCallable)
	void PerformIntercept(class AProjectile *targetProjectile);

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile();

	UFUNCTION()
	void ProjectileDestroyed();

	UFUNCTION(BlueprintCallable)
	void Upgrade();

	UFUNCTION(BlueprintCallable)
	uint8 GetBuildingLevel();

	UFUNCTION()
	void SetTileCoordinates(FVector Coords);

	UFUNCTION(BlueprintCallable)
	FVector GetTileCoordinates();

	void ResetInterceptor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent *SpawnPoint;

private:
	UPROPERTY(VisibleAnywhere)
	float LocationX;

	UPROPERTY(VisibleAnywhere)
	float LocationY;

	UPROPERTY(VisibleAnywhere)
	uint8 Level;

	UPROPERTY()
	float RespanDelaySeconds = 1.00F;

	UPROPERTY()
	float Health;

	UPROPERTY();
	bool IsTargeting;

	UPROPERTY();
	FVector TileCoords;

	UPROPERTY()
	class AProjectile *SpawnedProjectile;

	UPROPERTY()
	class AProjectileInterceptor *SpawnedInterceptorProjectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *UpgradeMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectile> ProjectileClass;

	// TODO: Refactor these into a Base Projectile - each which independently controls movement on Tick
	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AProjectileInterceptor> InterceptorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	bool ProjectileVisibleOnStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent *UpgradeSpawnPoint1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent *UpgradeSpawnPoint2;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UParticleSystem *DeathParticles;
	// Has a number of projectile slots (2 in some cases, for upgraded missile launchers)

	// Upgraded 0 = normal, 1 = Upgraded
	int Upgraded = 0;
};
