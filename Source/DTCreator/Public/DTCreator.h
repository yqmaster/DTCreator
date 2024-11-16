#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "HAL/IConsoleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FDTCreatorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();
	void CreateDataTables();

private:
	TSharedPtr<class FUICommandList> PluginCommands;
	TUniquePtr<FAutoConsoleCommand> ConsoleCommand;
};
