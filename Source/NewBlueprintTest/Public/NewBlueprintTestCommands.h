// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "NewBlueprintTestStyle.h"

class FNewBlueprintTestCommands : public TCommands<FNewBlueprintTestCommands>
{
public:

	FNewBlueprintTestCommands()
		: TCommands<FNewBlueprintTestCommands>(TEXT("NewBlueprintTest"), NSLOCTEXT("Contexts", "NewBlueprintTest", "NewBlueprintTest Plugin"), NAME_None, FNewBlueprintTestStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};