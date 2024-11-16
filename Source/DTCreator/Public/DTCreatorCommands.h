#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "DTCreatorStyle.h"

class FDTCreatorCommands : public TCommands<FDTCreatorCommands>
{
public:

	FDTCreatorCommands()
		: TCommands<FDTCreatorCommands>(TEXT("DTCreator"), NSLOCTEXT("Contexts", "DTCreator", "DTCreator Plugin"), NAME_None, FDTCreatorStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > PluginAction;
};
