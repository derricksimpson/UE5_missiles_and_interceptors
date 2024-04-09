
#include "TurretComponent.h"
#include "MetalMechs/BaseActor.h"
#include "MetalMechs/BaseIsland.h"
#include "MetalMechs/Projectile.h"

// Sets default values for this component's properties
UTurretComponent::UTurretComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	IsTargeting = false;
}

// Called when the game starts
void UTurretComponent::BeginPlay()
{
	Super::BeginPlay();
	// Set Owning actor
	Owner = Cast<ABaseActor>(GetOwner());
}

void UTurretComponent::SetComponentParts(USceneComponent *Turret, USceneComponent *Barrel)
{
	this->TurretComponent = Turret;
	this->BarrelComponent = Barrel;
}

void UTurretComponent::SetTarget(FVector NewTarget)
{
	Target = NewTarget;
}

ABaseActor *UTurretComponent::GetOwningActor()
{
	return Owner;
}

// Called every frame
void UTurretComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// For now, only detect on enemy island

	return;

	if (ticks % 60 == 0)
	{

		if (ticks > 24000)
		{
			ticks = 0;
		}
		/*

		if (Owner)
		{

			int len = Projectiles.Num();

			if (len > 0)
			{

				IsTargeting = true;
				UE_LOG(LogTemp, Warning, TEXT("Detected Projectiles %i"), Projectiles.Num());

				for (AProjectile* Projectile : Projectiles)
				{

					FVector OwnerLoc = Owner->GetActorLocation();
					FVector ProjLoc = Projectile->GetActorLocation();
					FRotator DirectionToTarget = (OwnerLoc - ProjLoc).Rotation();

					// If Both Components are the same
					if (TurretComponent == BarrelComponent)
					{
						this->TurretComponent->SetWorldRotation(FRotator(DirectionToTarget.Pitch, DirectionToTarget.Yaw, 0));
					}
					else
					{
						this->TurretComponent->SetWorldRotation(FRotator(0, DirectionToTarget.Yaw, 0));
						this->BarrelComponent->SetWorldRotation(FRotator(DirectionToTarget.Pitch, 0, 0));
					}

				}

			}
			else
			{
				IsTargeting = false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TurretComponent: Owner not found."));
		}
		*/
	}

	ticks++;
}
