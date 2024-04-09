#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MultiSpawnArrayComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class METALMECHS_API UMultiSpawnArrayComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMultiSpawnArrayComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<USceneComponent *> SpawnPoints;
};
