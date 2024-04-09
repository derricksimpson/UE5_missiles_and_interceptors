#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.h"
#include "ProjectileInterceptor.generated.h"

UCLASS()
class METALMECHS_API AProjectileInterceptor : public ABaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileInterceptor();

	UPROPERTY()
	bool IsMoving;

	UPROPERTY()
	FVector Target;

	UPROPERTY()
	class AProjectile *TargetProjectile;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CleanUp();

private:
	bool IsActive = false;

	int32 ticks = 0;

	UPROPERTY()
	float TrajectoryTicks = 0;

	UPROPERTY()
	float Velocity = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Physics", meta = (AllowPrivateAccess = "true"))
	float Acceleration = .01;

	UPROPERTY()
	FVector StartLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *BaseMesh;
};
