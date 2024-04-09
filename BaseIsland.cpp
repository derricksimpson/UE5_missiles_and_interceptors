
#include "BaseIsland.h"

#include "MainPawn.h"
#include "MetalMechsGameMode.h"
#include "Projectile.h"
#include "Actors/Building.h"
#include "Core/UnitStruct.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MetalHUD.h"
#include "MainPawn.h"

// Sets default values
ABaseIsland::ABaseIsland()
{
	PrimaryActorTick.bCanEverTick = true;

	IslandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Island Mesh"));
	RootComponent = IslandMesh;

	IslandMesh->OnBeginCursorOver.AddDynamic(this, &ABaseIsland::CustomOnBeginMouseOver);

	BoundsStart = CreateDefaultSubobject<USceneComponent>(TEXT("Start Bounds"));
	BoundsStart->SetupAttachment(RootComponent);

	BoundsEnd = CreateDefaultSubobject<USceneComponent>(TEXT("End Bounds"));
	BoundsEnd->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseIsland::BeginPlay()
{
	Super::BeginPlay();

	DecalInstance = GetWorld()->SpawnActor<AActor>(SelectionDecalClass, FVector::Zero(), FRotator::ZeroRotator);
	DecalInstance->SetActorHiddenInGame(true);

	if (GetWorld()->GetFirstPlayerController() != nullptr)
	{
		MainPawn = Cast<AMainPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
		MetalHud = Cast<AMetalHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	}

	GameMode = Cast<AMetalMechsGameMode>(GetWorld()->GetAuthGameMode());

	if (IsPlayerIsland)
	{
		LoadMap(Map);
		if (GameMode)
		{
			GameMode->SetPlayerIsland(this);
		}
	}
	else
	{
		LoadMap(EnemyMap);
		if (GameMode)
		{
			GameMode->SetEnemyIsland(this);
		}
	}

	UpdateResources();

	FTimerDelegate Delegate;

	Delegate.BindUFunction(this, "ProcessBuildings");

	// Process Buildings every 2 second
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_ProcessBuildings, Delegate, 2, true);
}

void ABaseIsland::LoadMap(FString TargetMap)
{
	// Find Map Extent
	const FVector Origin = BoundsStart->GetComponentLocation();
	const FVector Extent = BoundsEnd->GetComponentLocation();

	TArray<FString> MapArray;
	TargetMap.ParseIntoArray(MapArray, TEXT(" "));

	if (MapArray.Num() < 100)
	{
		UE_LOG(LogTemp, Warning, TEXT("Map Array could not be loaded. What's going on?"));
		return;
	}

	int Index = 0;

	if (!DataTableRef)
	{
		return;
	}
	TMap<FName, uint8 *> DT = DataTableRef->GetRowMap();

	// Look for "B" - (our Base) to make sure the datatable loaded successfully
	if (!DT.Contains(FName("B")))
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find B (A Base) in the map"));
		return;
	}

	FRotator InitRotation = IsPlayerIsland ? FRotator::ZeroRotator : FRotator(0, 180, 0);

	int Tile_X = 0;

	for (int X = Origin.X; X < Extent.X; X += 500)
	{
		int Tile_Y = 0;

		for (int Y = Origin.Y; Y < Extent.Y; Y += 500)
		{
			// GetWorld()->SpawnActor<AActor>(SelectionDecalClass, FVector(X + 250, Y + 250, 0), FRotator::ZeroRotator);
			if (Index < MapArray.Num())
			{
				FName UnitName = FName(MapArray[Index]);

				if (UnitName != "0" && DT.Contains(UnitName)) // If Not Empty - and DataTable has mapping
				{
					const FUnitStruct *Building = reinterpret_cast<FUnitStruct *>(DT[UnitName]);

					AActor *ActorInstance = GetWorld()->SpawnActor<AActor>(Building->ClassToSpawn,
																		   FVector(X + 250, Y + 250, 0),
																		   InitRotation);

					ABuilding *BuildingActor = Cast<ABuilding>(ActorInstance);

					BuildingActor->SetTileCoordinates(FVector(Tile_X, Tile_Y, 0.0f));

					BuildingActor->SetOwner(this);

					if (!IsPlayerIsland && IsEnemyHidden)
					{
						BuildingActor->SetActorHiddenInGame(true);
					}

					BuildingActor->SetIsEnemy(!IsPlayerIsland);
					BuildingActor->SetLocation(X, Y);

					// Store Building References
					if (ActorInstance)
					{
						Buildings.Add(BuildingActor);
					}
				}
			}
			Tile_Y++;
			Index++;
		}
		Tile_X++;
	}
}

void ABaseIsland::ProcessBuildings()
{
	for (ABuilding *Building : Buildings)
	{
		Energy += Building->GetEnergyGenerated();
		Money += Building->GetMoneyGenerated();
	}

	UpdateResources();
}

void ABaseIsland::UpdateResources()
{
	if (IsPlayerIsland && MetalHud)
	{
		MetalHud->SetPlayerResources(Money, Energy);
	}
}

// Called every frame
void ABaseIsland::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABaseIsland::IsPlayersIsland()
{
	return IsPlayerIsland;
}

void ABaseIsland::OnMouseMove(FVector HitLocation)
{

	LastMouseHitLocation = HitLocation;

	// Only display Attacking Decal if player is attacking
	if (!IsPlayerIsland)
	{
		if (MainPawn && !MainPawn->GetAttackingBuilding())
		{
			if (DecalInstance)
			{
				DecalInstance->SetActorHiddenInGame(true);
			}
			return;
		}
	}

	const int X_Tile = static_cast<int>((HitLocation.X + ((HitLocation.X <= 0) ? -250 : 250)) / 500);
	const int Y_Tile = static_cast<int>((HitLocation.Y + ((HitLocation.Y <= 0) ? -250 : 250)) / 500);

	HitLocation.X = (X_Tile * 500);
	HitLocation.Y = (Y_Tile * 500);

	// Get instance to our Decal Class
	if (DecalInstance)
	{
		DecalInstance->SetActorHiddenInGame(false);
		DecalInstance->SetActorLocation(HitLocation);
	}
}

FUintVector2 ABaseIsland::GetTileCoordinates(FVector Location)
{
	// NOTE: Once we have maps of different sizes and shapes, this offset will need to be adjusted;

	constexpr int xOffset = 5;
	constexpr int yOffset = 6;

	const FVector Loc = GetActorLocation();

	int X_Tile = static_cast<int>((Location.X + ((Location.X <= 0) ? -250 : 250) - Loc.X) / 500) + xOffset;
	int Y_Tile = static_cast<int>((Location.Y + ((Location.Y <= 0) ? -250 : 250) - Loc.Y) / 500) + yOffset;

	return FUintVector2(X_Tile, Y_Tile);
}

void ABaseIsland::AttackHere()
{
	AttackLocation.X = LastMouseHitLocation.X;
	AttackLocation.Y = LastMouseHitLocation.Y;
	AttackLocation.Z = LastMouseHitLocation.Z;

	// TODO: refactor out into GetTileCoordinates (FVec)

	const FVector Origin = BoundsStart->GetComponentLocation();

	const FVector Loc = GetActorLocation();

	int X_Tile = static_cast<int>((AttackLocation.X + ((AttackLocation.X <= 0) ? -250 : 250) - Loc.X) / 500) + 5;
	int Y_Tile = static_cast<int>((AttackLocation.Y + ((AttackLocation.Y <= 0) ? -250 : 250) - Loc.Y) / 500) + 6;

	if (MainPawn && MainPawn->GetAttackingBuilding())
	{
		// Call Attack Action on Building - At Target Location
		MainPawn->GetAttackingBuilding()->PerformAttack(AttackLocation, FVector(X_Tile, Y_Tile, 0), GameMode->GetEnemyIsland());

		// Clear Attacking Building
		MainPawn->SetAttackingBuilding(nullptr);

		GameMode->GetEnemyIsland()->HideAttackDecal();
	}
}

void ABaseIsland::OnExplosion(FVector Target, float BaseDamage, float SplashDamage)
{
	// Iterate over building and apply Damage
	for (ABuilding *Building : Buildings)
	{
		float distance = FVector::Distance(Building->GetActorLocation(), Target);

		if (distance < 510)
		{
			Building->OnTakeDamage(BaseDamage);
		}
		else if (distance < 1100)
		{
			Building->OnTakeDamage(SplashDamage);
		}
	}
}

void ABaseIsland::AddProjectile(AProjectile *Projectile)
{
	if (this->IsPlayerIsland)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerIsland Projectile added"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyIsland Projectile added"));
	}

	Projectiles.Add(Projectile);
}

void ABaseIsland::RemoveProjectile(AProjectile *Projectile)
{
	Projectiles.Remove(Projectile);
}

TArray<AProjectile *> ABaseIsland::GetProjectiles()
{
	return Projectiles;
}

void ABaseIsland::HideAttackDecal()
{
	DecalInstance->SetActorHiddenInGame(true);
}

void ABaseIsland::CustomOnBeginMouseOver(UPrimitiveComponent *TouchedComponent)
{

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, TEXT("Mouse Over"));
	}
}
