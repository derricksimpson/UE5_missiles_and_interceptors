#include "Projectile.h"
#include "BaseIsland.h"
#include "Actors/Building.h"
#include "MetalMechsGameMode.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Skeletal Mesh"));
	BaseMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();

	GameMode = Cast<AMetalMechsGameMode>(GetWorld()->GetAuthGameMode());
	BaseActor = Cast<ABaseActor>(GetOwner());
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMoving)
	{
		FVector Next = FMath::Lerp(StartLocation, Target, TrajectoryTicks);

		Next.Z += FMath::Sin(TrajectoryTicks * UE_PI) * CurveMultiplier;

		// FRotator Rot = FRotator( ((-45 - FMath::Tan(TrajectoryTicks)  * 180 / UE_PI) + TrajectoryTicks * 2.0f), 0, -90);
		FRotator Rot = FRotator((-45 - (FMath::Tan(TrajectoryTicks) * 180 / UE_PI)), 0, -90);

		SetActorLocation(Next);

		SetActorRotation(Rot);

		TrajectoryTicks += (Velocity * DeltaTime);

		Velocity += Acceleration / DistanceToTravel;

		// When reached destination
		if (TrajectoryTicks > 1)
		{
			IsMoving = false;

			// Create Explosion
			// TODO: Refactor to base cls=as
			if (ExplosionClass)
			{
				GetWorld()->SpawnActor<AActor>(ExplosionClass, GetActorLocation(), FRotator::ZeroRotator);
			}

			if (TargetIsland != nullptr)
			{
				TargetIsland->OnExplosion(Target, BaseDamage, SplashDamage);
			}

			CleanUp();

			this->OnBeginDestroy();
		}
	}
}

void AProjectile::CleanUp()
{
	Super::CleanUp();

	// Tell owner building to Rewpan this projectile
	ABuilding *OwnerBuilding = Cast<ABuilding>(GetOwner());

	if (OwnerBuilding)
	{
		OwnerBuilding->ProjectileDestroyed();
		UE_LOG(LogTemp, Warning, TEXT("Call Projectile destoryed on ABuilding"));
	}

	if (BaseActor->IsPlayerActor)
	{
		GameMode->GetPlayerIsland()->RemoveProjectile(this);
	}
	else
	{
		GameMode->GetEnemyIsland()->RemoveProjectile(this);
	}
}

void AProjectile::Intercept()
{
	IsMoving = false;
	CleanUp();
	this->OnBeginDestroy();
}

void AProjectile::Fire()
{
	Super::Fire();
	FireMissile();
}

FVector AProjectile::GetTargetTile()
{
	return TargetTile;
}

void AProjectile::FireMissile()
{
	if (!BaseActor)
	{
		BaseActor = Cast<ABaseActor>(GetOwner());
	}

	if (BaseActor->IsPlayerActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding projectile to player island list"));
		GameMode->GetPlayerIsland()->AddProjectile(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Adding projectile to ENEMY island list"));
		GameMode->GetEnemyIsland()->AddProjectile(this);
	}

	DistanceToTravel = FVector::Dist(StartLocation, Target);
	IsMoving = true;
	TrajectoryTicks = 0;
	Velocity = 0;
}
