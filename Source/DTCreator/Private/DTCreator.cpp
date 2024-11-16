#include "DTCreator.h"
#include "DTCreatorStyle.h"
#include "DTCreatorCommands.h"
#include "Misc/MessageDialog.h"
#include "ToolMenus.h"
#include "DTCreatorFunctionLibrary.h"

static const FName DTCreatorTabName("DTCreator");

#define LOCTEXT_NAMESPACE "FDTCreatorModule"

void FDTCreatorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FDTCreatorStyle::Initialize();
	FDTCreatorStyle::ReloadTextures();

	FDTCreatorCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FDTCreatorCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FDTCreatorModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FDTCreatorModule::RegisterMenus));

	// Create console command
	ConsoleCommand = MakeUnique<FAutoConsoleCommand>(
		TEXT("CreateDataTable"),
		TEXT("Create DataTable"),
		FConsoleCommandDelegate::CreateRaw(this, &FDTCreatorModule::CreateDataTables)
	);

	// Create DataTables after engine init
	FCoreDelegates::OnPostEngineInit.AddRaw(this, &FDTCreatorModule::CreateDataTables);
}

void FDTCreatorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FDTCreatorStyle::Shutdown();

	FDTCreatorCommands::Unregister();
}

void FDTCreatorModule::PluginButtonClicked()
{
	CreateDataTables();
}

void FDTCreatorModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("DTCreatorWindowLayout");
			Section.AddMenuEntryWithCommandList(FDTCreatorCommands::Get().PluginAction, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("DTCreatorPluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FDTCreatorCommands::Get().PluginAction));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

void FDTCreatorModule::CreateDataTables()
{
	UDTCreatorFunctionLibrary::CreateDataTables();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDTCreatorModule, DTCreator)