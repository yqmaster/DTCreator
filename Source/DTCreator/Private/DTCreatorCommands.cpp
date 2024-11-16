#include "DTCreatorCommands.h"

#define LOCTEXT_NAMESPACE "FDTCreatorModule"

void FDTCreatorCommands::RegisterCommands()
{
	UI_COMMAND(PluginAction, "DTCreator", "Execute DTCreator action", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
