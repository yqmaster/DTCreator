#include "KDataTableFactory.h"

UKDataTableFactory::UKDataTableFactory(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bCreateNew = false;
}

void UKDataTableFactory::SetStruct(UScriptStruct* NewStruct)
{
	this->Struct = NewStruct;
}
