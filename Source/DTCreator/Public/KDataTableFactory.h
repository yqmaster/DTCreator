#pragma once

#include "CoreMinimal.h"
#include "Factories/DataTableFactory.h"
#include "KDataTableFactory.generated.h"

/**
 *
 */
UCLASS(hidecategories = Object)
class DTCREATOR_API UKDataTableFactory : public UDataTableFactory
{
	GENERATED_BODY()

	// Constructor declaration
	UKDataTableFactory(const FObjectInitializer& ObjectInitializer);

public:
	void SetStruct(UScriptStruct* NewStruct);
};
