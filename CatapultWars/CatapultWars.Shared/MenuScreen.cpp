#include "pch.h"
#include "MenuScreen.h"

using namespace Platform;
using namespace DirectX;
using namespace CatapultWars;
using namespace DirectX::SimpleMath;

MenuScreen::MenuScreen(ScreenManager^ manager, String^ menuTitle)
	: GameScreen(manager)
	, m_menuTitle(menuTitle)
{
	TransitionOnTime = 0.5;
	TransitionOffTime = 0.5;
}

/// <summary>
/// Allows the screen the chance to position the menu entries. By default
/// all menu entries are lined up in a vertical list, centered on the screen.
/// </summary>
void MenuScreen::UpdateMenuEntryLocations() {
	// Make the menu slide into place during transitions, using a
	// power curve to make things look more interesting (this makes
	// the movement slow down as it nears the end).
	float transitionOffset = (float)MathHelper::Pow(TransitionPosition, 2);

	// start at Y = 175; each X value is generated per entry
	DirectX::SimpleMath::Vector2 position(0.0f, 175.0f);

	for (auto menuEntry : MenuEntries)
	{
		// each entry is to be centered horizontally
		int viewPortWidth = 0;
		auto viewport = m_screenManager->DeviceResources->GetScreenViewport();
		auto orientation = m_screenManager->DeviceResources->ComputeDisplayRotation();
		if (orientation == DXGI_MODE_ROTATION::DXGI_MODE_ROTATION_ROTATE90 || orientation == DXGI_MODE_ROTATION::DXGI_MODE_ROTATION_ROTATE270) {
			viewPortWidth = viewport.Height;		
		} else {
			viewPortWidth = viewport.Width;
		}
		
		auto menuEntryWidth = m_screenManager->Font->MeasureString(menuEntry->Text->Data());

		position.x = (viewPortWidth / 2) - (menuEntryWidth.m128_f32[0] / 2);

		if (State == ScreenState::TransitionOn)
			position.x -= transitionOffset * 256;
		else
			position.x += transitionOffset * 512;

		// set the entry's position
		menuEntry->Position = position;

		// move down for the next entry the size of this entry plus our padding
		position.y += m_screenManager->Font->GetLineSpacing() + (m_menuEntryPadding * 2);
	}
}

void MenuScreen::Update(double elapsedSeconds, bool coveredByOtherScreen, bool otherScreenHasFocus) {
	__super::Update(elapsedSeconds, coveredByOtherScreen, otherScreenHasFocus);
	
	for (int i = 0; i < MenuEntries.size(); i++)
	{
		bool isSelected = false;// IsActive && (i == m_selectedEntry);

		MenuEntries[i]->Update(this, isSelected, elapsedSeconds);
	}
}

void MenuScreen::Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, double elapsedSeconds) {

	UpdateMenuEntryLocations();

	auto font = m_screenManager->Font;
	auto deviceResources = m_screenManager->DeviceResources;

	auto viewPort = deviceResources->GetScreenViewport();
	if (font == nullptr)
		return;

	spriteBatch->Begin();
	
	for (int i = 0; i < MenuEntries.size(); i++)
	{
		auto menuEntry = MenuEntries[i];

		bool isSelected = IsActive() && (i == m_selectedEntry);

		menuEntry->Draw(spriteBatch, this, isSelected, elapsedSeconds);
	}

	Vector2 titlePosition(viewPort.Width / 2, 80);
	Vector2 titleOrigin = font->MeasureString(m_menuTitle->Data());
	Color titleColor(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);
	//titleColor *= TransitionAlpha;
	float titleScale = 1.25f;

	font->DrawString(spriteBatch.get(), m_menuTitle->Data(), titlePosition, titleColor, 0, titleOrigin, titleScale, SpriteEffects::SpriteEffects_None, 0);

	spriteBatch->End();
}