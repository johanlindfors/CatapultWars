#include "pch.h"
#include "BackgroundScreen.h"

using namespace CatapultWars;

BackgroundScreen::BackgroundScreen(ScreenManager^ manager)
	: GameScreen(manager)
{
	TransitionOnTime = 0;
	TransitionOffTime = 0.5;
}

void BackgroundScreen::LoadContent(ID3D11Device* device) {
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\title_screen.png", nullptr, m_background.ReleaseAndGetAddressOf())
		);
}

void BackgroundScreen::Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, double elapsedSeconds) {
	spriteBatch->Begin();
	spriteBatch->Draw(m_background.Get(), Vector2(0,0), Color(1.0,1.0,1.0,TransitionAlpha));

	spriteBatch->End();
}