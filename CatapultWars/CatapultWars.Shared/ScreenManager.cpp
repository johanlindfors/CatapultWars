#include "pch.h"
#include "ScreenManager.h"
#include "Common\DeviceResources.h"

using namespace CatapultWars;

ScreenManager::ScreenManager(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	: m_deviceResources(deviceResources)
{ }

void ScreenManager::Initialize() {
	
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_spriteBatch.reset(new SpriteBatch(context));
#if (WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP)
	m_spriteBatch->SetRotation(DXGI_MODE_ROTATION_ROTATE90);
#else
	m_spriteBatch->SetRotation(m_deviceResources->ComputeDisplayRotation());
#endif

	m_isInitialized = true;
}

void ScreenManager::LoadContent(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch) {
	m_spriteBatch = spriteBatch;

	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\blank.png", nullptr, m_blankTexture.ReleaseAndGetAddressOf())
		);

	m_font.reset(new SpriteFont(device, L"Assets\\Fonts\\TestHudFont.spritefont"));

	for (auto screen : m_screens) {
		screen->LoadContent(device);
	}
}

void ScreenManager::UnloadContent() {
	for (auto screen : m_screens) {
		screen->UnloadContent();
	}
}

void ScreenManager::Update(double elapsedSeconds) {
	m_screensToUpdate.clear();

	for (auto screen : m_screens) {
		m_screensToUpdate.push_back(screen);
	}

	bool otherScreenHasFocus = true;
	bool coveredByOtherScreen = false;

	while (m_screensToUpdate.size() > 0) {
		//auto screen = m_screensToUpdate[0];
		//m_screensToUpdate.erase(m_screensToUpdate.begin());
		auto screen = m_screensToUpdate[m_screensToUpdate.size() - 1];
		m_screensToUpdate.pop_back();

		screen->Update(elapsedSeconds, otherScreenHasFocus, coveredByOtherScreen);

		if (screen->State == ScreenState::TransitionOn ||
			screen->State == ScreenState::Active) {
			// If this is the first active screen we came across,
			// give it a chance to handle input.
			if (!otherScreenHasFocus)
			{
				screen->HandleInput(m_input);

				otherScreenHasFocus = true;
			}

			// If this is an active non-popup, inform any subsequent
			// screens that they are covered by it.
			if (!screen->IsPopup)
				coveredByOtherScreen = true;
		}

		if (m_traceEnabled) {
			TraceScreens();
		}
	}
}

void ScreenManager::Draw(double elapsedSeconds) {
	for (auto screen : m_screens) {
		if (screen->State != ScreenState::Hidden) {
			screen->Draw(m_spriteBatch, elapsedSeconds);
		}
	}
}

void ScreenManager::TraceScreens() {
	std::vector<String^> screenNames;

	for (auto screen : m_screens) {
		screenNames.push_back(screen->GetType()->ToString());
	}

	//Debug.WriteLine(string.Join(", ", screenNames.ToArray()));
}

void ScreenManager::AddScreen(GameScreen^ screen) {
	screen->Manager = this;
	screen->IsExiting = false;

	if (m_isInitialized) {
		screen->LoadContent(m_deviceResources->GetD3DDevice());
	}

	m_screens.push_back(screen);
}

void ScreenManager::RemoveScreen(GameScreen^ screen) {
	if (m_isInitialized) {
		screen->UnloadContent();
	}

	for (int i = 0; i < m_screens.size(); i++)
	{
		if (m_screens.at(i) == screen) {
			m_screens.erase(m_screens.begin() + i);
		}
	}

	for (int i = 0; i < m_screensToUpdate.size(); i++)
	{
		if (m_screensToUpdate.at(i) == screen) {
			m_screensToUpdate.erase(m_screensToUpdate.begin() + i);
		}
	}
}

vector<GameScreen^> ScreenManager::GetScreens() {
	return m_screens;
}

void ScreenManager::FadeBackBufferToBlack(float alpha) {
	// TODO
	
	//m_spriteBatch->Begin();
	//m_spriteBatch->Draw(m_blankTexture.Get(),
	//	new Rectangle(0, 0, viewport.Width, viewport.Height),
	//	Color.Black * alpha);
	//m_spriteBatch->End();
}

void ScreenManager::ScreenManager::SerializeState() {
	// TODO
}

void DeserializeState() {
	// TODO
}

void ScreenManager::DeleteState() {
	// TODO
}