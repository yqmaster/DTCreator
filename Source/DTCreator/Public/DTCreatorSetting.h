#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DeveloperSettings.h"
#include "DTCreatorSetting.generated.h"


UCLASS(Blueprintable, Abstract, EditInlineNew, CollapseCategories)
class DTCREATOR_API UDTCreatorHelperBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void MakeDataTableRow(UDataTable* DataTable, UObject* FileObject);

	virtual void MakeDataTableRow_Implementation(UDataTable* DataTable, UObject* FileObject) { }

	UFUNCTION(BlueprintNativeEvent)
	void PreFinishMakeDataTable(UDataTable* DataTable);

	virtual void PreFinishMakeDataTable_Implementation(UDataTable* DataTable) { }
};



USTRUCT(BlueprintType)
struct DTCREATOR_API FDTCreatorSettingInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	bool bEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	FString ExportFileName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (LongPackageName))
	FDirectoryPath CollectFolderPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TObjectPtr<class UScriptStruct> Struct;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<UDTCreatorHelperBase> HelperClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings", meta = (LongPackageName))
	TArray<FDirectoryPath> CollectFolderPaths;
};

/**
 *
 */
UCLASS(config = Game, defaultconfig)
class DTCREATOR_API UDTCreatorSetting : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	bool bEnable = true;

	UPROPERTY(Config, EditAnywhere, Category = "Settings", meta = (LongPackageName))
	FDirectoryPath ExportFolderPath = TEXT("/Game/Generated/DataTable");

	UPROPERTY(Config, EditAnywhere, Category = "Settings")
	TArray<FDTCreatorSettingInfo> Settings;
};
