#include "DTCreatorFunctionLibrary.h"
#include "Editor/UnrealEd/Public/FileHelpers.h"
#include "Editor/UnrealEd/Public/ObjectTools.h"
#include "Editor/UnrealEd/Public/PackageTools.h"
#include "Editor/UnrealEd/Public/Subsystems/EditorAssetSubsystem.h"
#include "UObject/GCObjectScopeGuard.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "DTCreatorSetting.h"
#include "KDataTableFactory.h"

#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 4)
#include "Blueprint/BlueprintExceptionInfo.h"
#else
#include "UObject/Script.h"
#endif

#define LOCTEXT_NAMESPACE "DTCreatorFunctionLibrary"

void UDTCreatorFunctionLibrary::CreateDataTables()
{
	UE_LOG(LogTemp, Log, TEXT("Start create DataTables"));
	UEditorAssetSubsystem* EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	if (EditorAssetSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Get UEditorAssetSubsystem get failed"));
		return;
	}

	UDTCreatorSetting* CreatorSetting = GetMutableDefault<UDTCreatorSetting>();
	if (CreatorSetting == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DTCreatorSetting get failed, Please check ProjectSetting"));
		return;
	}

	if (CreatorSetting->bEnable == false)
	{
		return;
	}

	FString ExportFolderPath = CreatorSetting->ExportFolderPath.Path;
	UKDataTableFactory* DataTableFactory = NewObject<UKDataTableFactory>();
	FGCObjectScopeGuard DontGCDataTableFactory(DataTableFactory);

	TArray<UPackage*> PackagesToSave;
	for (const FDTCreatorSettingInfo& Setting : CreatorSetting->Settings)
	{
		UClass* HelperClass = Setting.HelperClass;
		if (!HelperClass)
		{
			UE_LOG(LogTemp, Warning, TEXT("HelperClass is empty, Please check ProjectSetting"));
			continue;
		}

		UDTCreatorHelperBase* HelperObject = NewObject<UDTCreatorHelperBase>(GetTransientPackage(), HelperClass);
		check(HelperObject);
		FGCObjectScopeGuard DontGCHelperObject(HelperObject);

		UScriptStruct* Struct = Setting.Struct;
		if (Struct == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Struct is empty, Please check ProjectSetting"));
			continue;
		}

		DataTableFactory->SetStruct(Setting.Struct);

		// Create DataTable
		UDataTable* DataTable = Cast<UDataTable>(CreateAssetByFactory(Setting.ExportFileName, ExportFolderPath, DataTableFactory));
		if (DataTable == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Create DataTable failed, Please check ProjectSetting ExportFileName:%s ,ExportFolderPath:%s"), *Setting.ExportFileName, *ExportFolderPath);
			continue;
		}

		if (Setting.bEnable)
		{
			TArray<FDirectoryPath> FolderPaths;
			FolderPaths.Add(Setting.CollectFolderPath);
			FolderPaths.Append(Setting.CollectFolderPaths);

			TArray<FString> AllAssets;
			for (const FDirectoryPath FolderPath : FolderPaths)
			{
				AllAssets.Append(EditorAssetSubsystem->ListAssets(FolderPath.Path, true, false));
			}

			for (const FString& Asset : AllAssets)
			{
				FString PackageName;
				if (Asset.Split(".", &PackageName, nullptr, ESearchCase::CaseSensitive, ESearchDir::FromEnd))
				{
					UObject* FindAss = EditorAssetSubsystem->LoadAsset(PackageName);
					HelperObject->MakeDataTableRow(DataTable, FindAss);
				}
			}

			HelperObject->PreFinishMakeDataTable(DataTable);
		}

		PackagesToSave.Add(DataTable->GetOutermost());
	}
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false, NULL, true, false);
	UE_LOG(LogTemp, Log, TEXT("Finish create DataTables"));
}


void UDTCreatorFunctionLibrary::AddDataTableRowByUserDefinedStruct(UDataTable* const DataTable, const FName& RowName, const FTableRowBase& RowData)
{
	if (!DataTable)
	{
		UE_LOG(LogDataTable, Error, TEXT("AddDataTableRow - The DataTable is invalid."));
		return;
	}

	DataTable->AddRow(RowName, RowData);
}

DEFINE_FUNCTION(UDTCreatorFunctionLibrary::execAddDataTableRowByUserDefinedStruct)
{
	P_GET_OBJECT(UDataTable, DataTable);
	P_GET_PROPERTY(FNameProperty, RowName);

	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentPropertyContainer = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);

	const FTableRowBase* const RowData = reinterpret_cast<FTableRowBase*>(Stack.MostRecentPropertyAddress);
	const FStructProperty* const StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);

	P_FINISH;

	if (!DataTable)
	{
		FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("AddDataTableRow", "MissingTableInput", "Failed to resolve the DataTable parameter.")
		);
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
	else if (StructProp && RowData)
	{
		const UScriptStruct* const RowType = StructProp->Struct;
		const UScriptStruct* const TableType = DataTable->GetRowStruct();

		// If the row type is compatible with the table type ...
		const bool bMatchesTableType = RowType == TableType;
		if (bMatchesTableType || (RowType->IsChildOf(TableType) && FStructUtils::TheSameLayout(RowType, TableType)))
		{
			P_NATIVE_BEGIN;
			AddDataTableRowByUserDefinedStruct(DataTable, RowName, *RowData);
			P_NATIVE_END;
		}
		else
		{
			FBlueprintExceptionInfo ExceptionInfo(
				EBlueprintExceptionType::AccessViolation,
				NSLOCTEXT("AddDataTableRow", "IncompatibleProperty", "The data table type is incompatible with the RowData parameter.")
			);
			FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
		}
	}
	else
	{
		FBlueprintExceptionInfo ExceptionInfo(
			EBlueprintExceptionType::AccessViolation,
			NSLOCTEXT("AddDataTableRow", "MissingOutputProperty", "Failed to resolve the RowData parameter.")
		);
		FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
	}
}

void UDTCreatorFunctionLibrary::EmptyDataTableTable(UDataTable* const DataTable)
{
	if (DataTable)
	{
		DataTable->EmptyTable();
	}
}

TArray<UObject*> UDTCreatorFunctionLibrary::FindObjectReferences(UObject* Object)
{
	TArray<UObject*> ReturnObjects;
	if (Object)
	{
		FReferenceFinder ReferenceFinder(ReturnObjects, nullptr, false, false, true);
		ReferenceFinder.FindReferences(Object);
	}

	return ReturnObjects;
}

TArray<UObject*> UDTCreatorFunctionLibrary::FindObjectReferencesByClass(UObject* Object, TSubclassOf<UObject> ObjectClass)
{
	TArray<UObject*> ReturnObjects;

	if (UClass* TargetClass = ObjectClass.Get())
	{
		for (UObject* FindObject : FindObjectReferences(Object))
		{
			if (FindObject && FindObject->IsA(TargetClass))
			{
				ReturnObjects.Add(FindObject);
			}
		}
	}

	return ReturnObjects;
}

bool UDTCreatorFunctionLibrary::CanCreateAsset(const FString& AssetName, const FString& PackageName, const FText& OperationText)
{
	FText Reason;
	// CheckName
	if (!FName(*AssetName).IsValidObjectName(Reason) || !FPackageName::IsValidLongPackageName(PackageName, /*bIncludeReadOnlyRoots=*/false, &Reason))
	{
		UE_LOG(LogTemp, Warning, TEXT("Object name or package name is invalid, reason: %s"), *Reason.ToString());
		return false;
	}

	// Remove existing Data
	if (UPackage* ExistingPkg = LoadPackage(nullptr, *PackageName, LOAD_None))
	{
		if (UObject* ExistingObject = StaticFindObject(UObject::StaticClass(), ExistingPkg, *AssetName))
		{
			bool bDeleteSucceeded = ObjectTools::DeleteSingleObject(ExistingObject, false);
			if (!bDeleteSucceeded)
			{
				UE_LOG(LogTemp, Warning, TEXT("Delete object failed for package: %s"), *PackageName);
				return false;
			}

			// Force GC so we can cleanly create a new asset (and not do an 'in place' replacement)
			CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS);
		}
	}

	// Create empty Package
	UPackage* Pkg = CreatePackage(*PackageName);
	Pkg->MarkAsFullyLoaded();

	return true;
}

UObject* UDTCreatorFunctionLibrary::CreateAssetByFactory(const FString& AssetName, const FString& PackagePath, UFactory* Factory)
{
	if (!ensure(Factory))
	{
		return nullptr;
	}

	FGCObjectScopeGuard DontGCFactory(Factory);
	// Sanitize PackageName
	const FString PackageName = UPackageTools::SanitizePackageName(PackagePath + TEXT("/") + AssetName);
	// Make sure we can create the asset without conflicts
	if (!UDTCreatorFunctionLibrary::CanCreateAsset(AssetName, PackageName, LOCTEXT("CreateANewObject", "Create a new object")))
	{
		return nullptr;
	}

	UClass* ClassToUse = Factory->GetSupportedClass();
	UPackage* Pkg = CreatePackage(*PackageName);
	EObjectFlags Flags = RF_Public | RF_Standalone | RF_Transactional;
	UObject* NewObj = Factory->FactoryCreateNew(ClassToUse, Pkg, FName(*AssetName), Flags, nullptr, GWarn, NAME_None);

	if (NewObj)
	{
		// Notify the asset registry
		FAssetRegistryModule::AssetCreated(NewObj);
		// Mark the package dirty...
		Pkg->MarkPackageDirty();
	}

	return NewObj;
}

#undef LOCTEXT_NAMESPACE