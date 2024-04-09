#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.h"

#include "Projectile.generated.h"

UCLASS()
class METALMECHS_API AProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UPROPERTY()
	bool IsMoving;

	UPROPERTY(EditAnywhere)
	FVector Target = FVector(10730.000000, 1550.000000, 75.000000);

	UPROPERTY()
	FVector TargetTile;

	UFUNCTION(BlueprintCallable)
	FVector GetTargetTile();

	UPROPERTY()
	class ABaseIsland *TargetIsland;

	TArray<FVector> TrajectoryArr;

	UFUNCTION()
	void Intercept();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CleanUp();

private:
	UPROPERTY()
	float TrajectoryTicks = 0.f;

	UPROPERTY()
	float Velocity = 0.000;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Acceleration = 34;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY()
	class AMetalMechsGameMode *GameMode;

	UPROPERTY()
	class ABaseActor *BaseActor;

	UPROPERTY()
	double DistanceToTravel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *BaseMesh;

	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
	class UParticleSystemComponent *TrailParticles;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Path Class", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> ProjectileTrailClass;

	UPROPERTY(EditDefaultsOnly, Category = "Trajectory", meta = (AllowPrivateAccess = "true"))
	// Sets the curve on the trajectory arch
	float CurveMultiplier = 14.5f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float BaseDamage = 1200;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float SplashDamage = 600;

	UFUNCTION(BlueprintCallable)
	void FireMissile();
};
