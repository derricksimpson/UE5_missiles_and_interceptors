#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurretComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class METALMECHS_API UTurretComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTurretComponent();

	UFUNCTION(BluePrintCallable)
	void SetTarget(FVector NewTarget);

	UFUNCTION(BluePrintCallable)
	class ABaseActor *GetOwningActor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY();
	class ABaseActor *Owner;

	UFUNCTION(BlueprintCallable)
	void SetComponentParts(USceneComponent *Turret, USceneComponent *Barrel);

private:
	UPROPERTY()
	FVector Target;

	// counts ticks - allowing us to do things less frequently
	UINT ticks;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Targeting", meta = (AllowPrivateAccess = "true"))
	bool IsTargeting;

	UPROPERTY()
	USceneComponent *TurretComponent;

	UPROPERTY()
	USceneComponent *BarrelComponent;
};
