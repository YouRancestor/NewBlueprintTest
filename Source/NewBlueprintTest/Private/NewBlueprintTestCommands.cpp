// Copyright Epic Games, Inc. All Rights Reserved.

#include "NewBlueprintTestCommands.h"

#define LOCTEXT_NAMESPACE "FNewBlueprintTestModule"

void FNewBlueprintTestCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "NewBlueprintTest", "Bring up NewBlueprintTest window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
