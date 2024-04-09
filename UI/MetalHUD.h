#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MetalHUD.generated.h"

/**
 *
 */
UCLASS()
class METALMECHS_API AMetalHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayerResources(float money, float energy);
};
