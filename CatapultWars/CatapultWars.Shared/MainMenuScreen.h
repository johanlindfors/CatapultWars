#pragma once
#include "MenuEntry.h"
#include "MenuScreen.h"

namespace CatapultWars {

	ref class MenuEntry;

	ref class MainMenuScreen : MenuScreen {

	internal:		
		MainMenuScreen(ScreenManager^ manager);
		void UpdateMenuEntryLocations() override;
	};
}