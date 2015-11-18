#pragma once
#include "MenuEntry.h"
#include "GameScreen.h"

namespace CatapultWars {

	ref class MenuEntry;

	ref class MenuScreen : GameScreen {

	internal:
		std::vector<MenuEntry^> MenuEntries;
		MenuScreen(ScreenManager^ manager, String^ menuTitle);
		void Update(double elapsedSeconds, bool coveredByOtherScreen, bool otherScreenHasFocus) override;
		void Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, double elapsedSeconds) override;
		virtual void UpdateMenuEntryLocations();
		const int m_menuEntryPadding = 25;
		int m_selectedEntry = 0;
		String^ m_menuTitle;
		
	};
}