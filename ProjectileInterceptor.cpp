#include "ProjectileInterceptor.h"
#include "Projectile.h"
#include "BaseActor.h"

// Sets default values
AProjectileInterceptor::AProjectileInterceptor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AProjectileInterceptor::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectileInterceptor::CleanUp()
{
	Super::CleanUp();

	auto BaseActor = Cast<ABaseActor>(GetOwner());

	if (BaseActor)
	{
		BaseActor->ResetInterceptor();
	}
}

// Called every frame
void AProjectileInterceptor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive)
	{
		if (TargetProjectile)
		{

			FVector ToTargetVector = GetActorLocation() - TargetProjectile->GetActorLocation();

			FRotator rotor = ToTargetVector.Rotation();

			rotor = rotor.Add(0, 90, 0);

			SetActorRotation(rotor);

			if (ToTargetVector.Length() < 8000)
			{

				FVector Next = FMath::Lerp(GetActorLocation(), TargetProjectile->GetActorLocation(), TrajectoryTicks);

				SetActorLocation(Next);

				TrajectoryTicks += (Velocity * DeltaTime);

				// Detonate to hopefully explode the other projectile
				if (ToTargetVector.Length() < 400)
				{
					IsActive = false;

					// Create Explosion
					// TODO: Refactor to base cls=as
					if (ExplosionClass)
					{
						GetWorld()->SpawnActor<AActor>(ExplosionClass, GetActorLocation(), FRotator::ZeroRotator);
					}

					// 90% change of destroying
					// TODO: Pick this up dynamically from the environment based on nearby radar installations
					float random = FMath::RandRange(0, 100);

					if (random < 50.0)
					{
						TargetProjectile->Intercept();
						TargetProjectile = nullptr;
					}

					CleanUp();

					OnBeginDestroy();
				}

				Velocity += Acceleration;
			}
		}
	}

	ticks++;
}

void AProjectileInterceptor::Fire()
{
	Super::Fire();

	this->OnFireProjectile();

	// Causes tick loop to activate
	IsActive = true;
}
