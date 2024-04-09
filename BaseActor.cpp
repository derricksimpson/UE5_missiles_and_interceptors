
#include "BaseActor.h"
#include "BaseIsland.h"
#include "Projectile.h"
#include "ProjectileInterceptor.h"

#include "Kismet/GameplayStatics.h"
#include "MetalMechs/MetalMechsGameMode.h"

// Sets default values
ABaseActor::ABaseActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent = BaseMesh; //->SetupAttachment(RootComponent);

	UpgradeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Upgraded Mesh"));
	UpgradeMesh->SetupAttachment(RootComponent);
	UpgradeMesh->SetVisibility(false);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Spawn Point"));
	SpawnPoint->SetupAttachment(BaseMesh);

	UpgradeSpawnPoint1 = CreateDefaultSubobject<USceneComponent>(TEXT("Upgrade Spawn Point 1"));
	UpgradeSpawnPoint1->SetupAttachment(BaseMesh);

	UpgradeSpawnPoint2 = CreateDefaultSubobject<USceneComponent>(TEXT("Upgrade Spawn Point 2"));
	UpgradeSpawnPoint2->SetupAttachment(BaseMesh);

	ProjectileVisibleOnStart = false;
	Level = 1;
	Health = 100;
	IsTargeting = false;
}

void ABaseActor::SpawnProjectile()
{
	if (Upgraded == 0)
	{
		if (ProjectileClass)
		{
			AProjectile *MyProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());

			MyProjectile->SetOwner(this);

			SpawnedProjectile = MyProjectile;
		}
	}
}

void ABaseActor::Upgrade()
{
	Level++;
}

uint8 ABaseActor::GetBuildingLevel()
{
	return Level;
}

void ABaseActor::SetTileCoordinates(FVector Coords)
{
	TileCoords = Coords;
}

FVector ABaseActor::GetTileCoordinates()
{
	return TileCoords;
}

void ABaseActor::ProjectileDestroyed()
{
	FTimerHandle TimerHandle_SpawnDelay;
	FTimerDelegate Delegate;

	Delegate.BindUFunction(this, "SpawnProjectile");

	// 20 Seconds with no upgrades - we need to allow for faster building / upgrading
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_SpawnDelay, Delegate, RespanDelaySeconds, false);
}

// Called when the game starts or when spawned
void ABaseActor::BeginPlay()
{
	Super::BeginPlay();

	if (ProjectileVisibleOnStart)
	{
		SpawnProjectile();
	}
}

void ABaseActor::SetLocation(float X, float Y)
{
	LocationX = X;
	LocationY = Y;
}

void ABaseActor::OnTakeDamage(float DamageAmount)
{
	Health -= DamageAmount;

	if (Health <= 0)
	{
		Health = 0;

		OnDestroyed();
	}
}

bool ABaseActor::IsDestroyed()
{
	return Health <= 0;
}

void ABaseActor::OnDestroyed()
{
	OnDestroyedBP();
}

void ABaseActor::PerformAttack(FVector AttackLocation, FVector AttackTileLocation, ABaseIsland *TargetIsland)
{

	if (SpawnedProjectile)
	{
		SpawnedProjectile->Target = AttackLocation;
		SpawnedProjectile->TargetTile = AttackTileLocation;
		SpawnedProjectile->TargetIsland = TargetIsland;
		SpawnedProjectile->OnFireProjectile();
	}
}

// TODO _ Refactor out into Interceptor Actor
void ABaseActor::ResetInterceptor()
{
	SpawnedInterceptorProjectile = nullptr;
}

// TODO _ Refactor out into Interceptor Actor
void ABaseActor::PerformIntercept(AProjectile *targetProjectile)
{
	if (IsDestroyed())
	{
		return;
	}

	if (!SpawnedInterceptorProjectile)
	{

		if (InterceptorClass)
		{

			AProjectileInterceptor *Interceptor = GetWorld()->SpawnActor<AProjectileInterceptor>(InterceptorClass, SpawnPoint->GetComponentLocation(), SpawnPoint->GetComponentRotation());

			Interceptor->SetOwner(this);
			Interceptor->TargetProjectile = targetProjectile;

			SpawnedInterceptorProjectile = Interceptor;

			Interceptor->Fire();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Interceptor class not configured - fixnow"));
		}
	}
}

// Called every frame
void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
