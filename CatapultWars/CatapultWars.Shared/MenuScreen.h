#pragma once
#include "GameScreen.h"

ref class MenuEntry;

namespace CatapultWars {

	ref class MenuScreen : GameScreen {

	internal:
		std::vector<MenuEntry^> MenuEntries;
		MenuScreen(String^ menuTitle);
		void Update(double elapsedSeconds, bool coveredByOtherScreen, bool otherScreenHasFocus) override;

	private:
		const int m_menuEntryPadding = 25;
		int m_selectedEntry = 0;
		String^ m_menuTitle;
		
	};
}