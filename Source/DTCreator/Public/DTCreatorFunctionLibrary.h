#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Editor/UnrealEd/Classes/Factories/Factory.h"
#include "DTCreatorFunctionLibrary.generated.h"

/**
 *
 */
UCLASS()
class DTCREATOR_API UDTCreatorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void CreateDataTables();

	/** Add a row to a Data Table with the provided name and data. */
	UFUNCTION(BlueprintCallable, CustomThunk, Category = "Editor Scripting | DataTable", meta = (AutoCreateRefTerm = "RowName", CustomStructureParam = "RowData"))
	static void AddDataTableRowByUserDefinedStruct(UDataTable* const DataTable, const FName& RowName, const FTableRowBase& RowData);
	DECLARE_FUNCTION(execAddDataTableRowByUserDefinedStruct);

	UFUNCTION(BlueprintCallable)
	static void EmptyDataTableTable(UDataTable* const DataTable);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<UObject*> FindObjectReferences(UObject* Object);

	UFUNCTION(BlueprintCallable, meta = (ObjectClass = "/Script/Engine.Object", DeterminesOutputType = "ObjectClass"))
	static TArray<UObject*> FindObjectReferencesByClass(UObject* Object, TSubclassOf<UObject> ObjectClass);

	// 因为生命周期过早所以使用这个函数来替代 GetMutableDefaultObject
	UFUNCTION(BlueprintPure, meta = (DeterminesOutputType = "Class", DynamicOutputParam = "DefaultObject"))
	static void GetDTCreatorMutableDefaultObject(TSubclassOf<UObject> Class, UObject*& DefaultObject);
public:
	static bool CanCreateAsset(const FString& AssetName, const FString& PackageName, const FText& OperationText);
	static UObject* CreateAssetByFactory(const FString& AssetName, const FString& PackagePath, UFactory* Factory);
};
