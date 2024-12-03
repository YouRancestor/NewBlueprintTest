// Copyright Epic Games, Inc. All Rights Reserved.

#include "NewBlueprintTest.h"
#include "NewBlueprintTestStyle.h"
#include "NewBlueprintTestCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "SNewBlueprintView.h"

static const FName NewBlueprintTestTabName("NewBlueprintTest");

#define LOCTEXT_NAMESPACE "FNewBlueprintTestModule"

void FNewBlueprintTestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FNewBlueprintTestStyle::Initialize();
	FNewBlueprintTestStyle::ReloadTextures();

	FNewBlueprintTestCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FNewBlueprintTestCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FNewBlueprintTestModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FNewBlueprintTestModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(NewBlueprintTestTabName, FOnSpawnTab::CreateRaw(this, &FNewBlueprintTestModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FNewBlueprintTestTabTitle", "NewBlueprintTest"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FNewBlueprintTestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FNewBlueprintTestStyle::Shutdown();

	FNewBlueprintTestCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(NewBlueprintTestTabName);
}

TSharedRef<SDockTab> FNewBlueprintTestModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FNewBlueprintTestModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("NewBlueprintTest.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SNewBlueprintView)
		];
}

void FNewBlueprintTestModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(NewBlueprintTestTabName);
}

void FNewBlueprintTestModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FNewBlueprintTestCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FNewBlueprintTestCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNewBlueprintTestModule, NewBlueprintTest)