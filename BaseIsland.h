#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseIsland.generated.h"

UCLASS(Blueprintable)
class METALMECHS_API ABaseIsland : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseIsland();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class AMainPawn *MainPawn;
	// UFUNCTION(BlueprintImplementableEvent, Category="Level", meta = (DisplayName = "Place Unit on Board"))
	// void PlaceUnit(float X, float Y, FString& ID);

	UPROPERTY()
	class AMetalHUD *MetalHud;

	UPROPERTY()
	class AMetalMechsGameMode *GameMode;

	UFUNCTION()
	void ProcessBuildings();

	UFUNCTION()
	void UpdateResources();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Unit Data Table") //, meta=(AllowPrivateAccess = "true"))
	class UDataTable *DataTableRef;

	UPROPERTY(EditAnywhere, Category = "Island Configuration")
	bool IsPlayerIsland;

	UFUNCTION(BlueprintCallable)
	bool IsPlayersIsland();

	UPROPERTY()
	bool IsEnemyHidden = false;

	UFUNCTION(BlueprintCallable)
	void OnMouseMove(FVector HitLocation);

	UFUNCTION(BlueprintCallable)
	void AttackHere();

	UFUNCTION()
	void OnExplosion(FVector Target, float BaseDamage, float SplashDamage);

	UFUNCTION()
	void AddProjectile(AProjectile *Projectile);

	UFUNCTION()
	void RemoveProjectile(AProjectile *Projectile);

	UFUNCTION(BlueprintCallable)
	TArray<AProjectile *> GetProjectiles();

	UFUNCTION(BlueprintCallable)
	void HideAttackDecal();

private:
	UPROPERTY(VisibleAnywhere)
	TArray<class AProjectile *> Projectiles;

	// TODO: Refactor into separate function
	UPROPERTY(VisibleAnywhere)
	FTimerHandle TimerHandle_ProcessBuildings;

	UPROPERTY(VisibleAnywhere)
	bool IsClient = true;

	UPROPERTY(VisibleAnywhere)
	float Money = 0.0f;

	UPROPERTY(VisibleAnywhere)
	float Energy = 0.0f;

	UPROPERTY(VisibleAnywhere)
	int TickCount = 0;

	UPROPERTY(VisibleAnywhere)
	int MapNumber = 1;

	UPROPERTY()
	FVector AttackLocation;

	UPROPERTY()
	FVector AttackTileLocation;

	UPROPERTY()
	FVector LastMouseHitLocation;

	UPROPERTY(VisibleAnywhere)
	FVector TileCoordinates;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *IslandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boundaries", meta = (AllowPrivateAccess = "true"))
	USceneComponent *BoundsStart;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boundaries", meta = (AllowPrivateAccess = "true"))
	USceneComponent *BoundsEnd;

	UFUNCTION()
	void CustomOnBeginMouseOver(UPrimitiveComponent *TouchedComponent);

	UFUNCTION()
	void LoadMap(FString TargetMap);

	UFUNCTION()
	FUintVector2 GetTileCoordinates(FVector Location);

	UPROPERTY(EditAnywhere, Category = "Selection Decal", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> SelectionDecalClass;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AActor> DecalInstance;

	UPROPERTY()
	TArray<class ABuilding *> Buildings;

	UPROPERTY(VisibleAnywhere)
	FString Map =
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 I 0 0 0 0 0 0 0 I 0 0 "
		"0 I R I 0 0 0 0 0 I R I 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 M 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 B 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 F 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 I R I 0 0 0 0 0 I R I 0 "
		"0 0 I 0 0 0 0 0 0 0 I 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 ";

	// No Base Present, as it will be placed randomly
	FString EnemyMap =
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 I 0 0 0 0 0 0 0 I 0 0 "
		"0 I R I 0 0 0 0 0 I R I 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 "
		"0 I R I 0 0 0 0 0 I R I 0 "
		"0 0 I 0 0 0 0 0 0 0 I 0 0 "
		"0 0 0 0 0 0 0 0 0 0 0 0 0 ";
};
