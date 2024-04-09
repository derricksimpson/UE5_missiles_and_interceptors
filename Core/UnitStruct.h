#pragma once

#include "Engine/DataTable.h"

#include "UnitStruct.generated.h"

USTRUCT(BlueprintType)
struct FUnitStruct : public FTableRowBase
{

	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AActor> ClassToSpawn;

	FUnitStruct(): ClassToSpawn(nullptr)
	{
	}
};
