#include "DTCreatorStyle.h"
#include "DTCreator.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateStyleRegistry.h"
#include "Slate/SlateGameResources.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleMacros.h"

#define RootToContentDir Style->RootToContentDir

TSharedPtr<FSlateStyleSet> FDTCreatorStyle::StyleInstance = nullptr;

void FDTCreatorStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FDTCreatorStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
	ensure(StyleInstance.IsUnique());
	StyleInstance.Reset();
}

FName FDTCreatorStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("DTCreatorStyle"));
	return StyleSetName;
}


const FVector2D Icon16x16(16.0f, 16.0f);
const FVector2D Icon20x20(20.0f, 20.0f);

TSharedRef< FSlateStyleSet > FDTCreatorStyle::Create()
{
	TSharedRef< FSlateStyleSet > Style = MakeShareable(new FSlateStyleSet("DTCreatorStyle"));
	Style->SetContentRoot(IPluginManager::Get().FindPlugin("DTCreator")->GetBaseDir() / TEXT("Resources"));

	Style->Set("DTCreator.PluginAction", new IMAGE_BRUSH_SVG(TEXT("ButtonIcon"), Icon20x20));
	return Style;
}

void FDTCreatorStyle::ReloadTextures()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
	}
}

const ISlateStyle& FDTCreatorStyle::Get()
{
	return *StyleInstance;
}
