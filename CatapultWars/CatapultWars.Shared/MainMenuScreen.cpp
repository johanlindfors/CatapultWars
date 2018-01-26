#include "pch.h"
#include "MainMenuScreen.h"

using namespace CatapultWars;
using namespace DirectX::SimpleMath;

MainMenuScreen::MainMenuScreen(ScreenManager^ manager)
	: MenuScreen(manager,"")
{
	IsPopup = true;

	MenuEntry^ startSinglePlayerGameMenuEntry = ref new MenuEntry("player vs ai");
	MenuEntry^ startTwoPlayerGameMenuEntry = ref new MenuEntry("player vs player");
	MenuEntry^ exitMenuEntry = ref new MenuEntry("exit");

	MenuEntries.push_back(startSinglePlayerGameMenuEntry);
	MenuEntries.push_back(startTwoPlayerGameMenuEntry);
	MenuEntries.push_back(exitMenuEntry);
}

/// <summary>
/// Allows the screen the chance to position the menu entries. By default
/// all menu entries are lined up in a vertical list, centered on the screen.
/// </summary>
void MainMenuScreen::UpdateMenuEntryLocations() {
	MenuScreen::UpdateMenuEntryLocations();

	for (auto entry : MenuEntries)
	{
		Vector2 position = entry->Position;
		position.y += 60;
		entry->Position = position;
	}
}