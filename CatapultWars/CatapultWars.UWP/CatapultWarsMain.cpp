#include "pch.h"
#include "CatapultWarsMain.h"
#include "Common\DirectXHelper.h"

using namespace std;
using namespace DX;
using namespace CatapultWars;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace Microsoft::WRL;

// Loads and initializes application assets when the application is loaded.
CatapultWarsMain::CatapultWarsMain(const shared_ptr<DeviceResources>& deviceResources) 
	: m_deviceResources(deviceResources)
	, m_minWind(0)
	, m_maxWind(2) {
	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_fpsTextRenderer = make_unique<SampleFpsTextRenderer>(m_deviceResources);

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	//m_timer.SetFixedTimeStep(true);
	//m_timer.SetTargetElapsedSeconds(1.0 / 60);

	CreateWindowSizeDependentResources();
}

CatapultWarsMain::~CatapultWarsMain() {
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void CatapultWarsMain::CreateWindowSizeDependentResources() {
	auto device = m_deviceResources->GetD3DDevice();
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_spriteBatch.reset(new SpriteBatch(context));
#if (WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP)
	m_spriteBatch->SetRotation(DXGI_MODE_ROTATION_ROTATE90);
#else
	m_spriteBatch->SetRotation(m_deviceResources->ComputeDisplayRotation());
#endif
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\sky.dds", nullptr, m_skyTexture.ReleaseAndGetAddressOf())
		);
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\gameplay_screen.dds", nullptr, m_foregroundTexture.ReleaseAndGetAddressOf())
		);
	ComPtr<ID3D11Resource> cloud1TextureRes;
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\cloud1.dds", cloud1TextureRes.ReleaseAndGetAddressOf(), m_cloud1Texture.ReleaseAndGetAddressOf())
		);
	ComPtr<ID3D11Resource> cloud2TextureRes;
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\cloud2.dds", cloud2TextureRes.ReleaseAndGetAddressOf(), m_cloud2Texture.ReleaseAndGetAddressOf())
		);
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\mountain.dds", nullptr, m_mountainTexture.ReleaseAndGetAddressOf())
		);
	ComPtr<ID3D11Resource> defeatTextureRes;
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\defeat.dds", defeatTextureRes.ReleaseAndGetAddressOf(), m_defeatTexture.ReleaseAndGetAddressOf())
		);
	ComPtr<ID3D11Resource> victoryTextureRes;
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\victory.dds", victoryTextureRes.ReleaseAndGetAddressOf(), m_victoryTexture.ReleaseAndGetAddressOf())
		);
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\HUD\\hudBackground.dds", nullptr, m_hudBackgroundTexture.ReleaseAndGetAddressOf())
		);
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\HUD\\windArrow.dds", nullptr, m_windArrowTexture.ReleaseAndGetAddressOf())
		);
	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\HUD\\ammoType.dds", nullptr, m_ammoTypeTexture.ReleaseAndGetAddressOf())
		);

	// Load font
	//m_hudFont.reset(new SpriteFont(device, L"Assets\\Fonts\\TestHUDFont.spritefont"));

	// Define initial cloud position

	DX::GetTextureSize(cloud1TextureRes.Get(), &m_cloud1TextureWidth, &m_cloud1TextureHeight);
	DX::GetTextureSize(cloud2TextureRes.Get(), &m_cloud2TextureWidth, &m_cloud2TextureHeight);
	DX::GetTextureSize(defeatTextureRes.Get(), &m_defeatTextureWidth, &m_defeatTextureHeight);
	DX::GetTextureSize(victoryTextureRes.Get(), &m_victoryTextureWidth, &m_victoryTextureHeight);

	m_cloud1Position = Vector2(224 - m_cloud1TextureWidth, 32);
	m_cloud2Position = Vector2(64, 90);

	// Define initial HUD positions
	m_playerHUDPosition = Vector2(7, 7);
	m_computerHUDPosition = Vector2(613, 7);
	m_windArrowPosition = Vector2(345, 46);

	// Initialize human & AI players
	m_player = std::make_shared<Human>();
	m_player->Initialize(device, m_spriteBatch);
	m_player->SetName(L"Player");

	m_computer = std::make_shared<AI>();
	m_computer->Initialize(device, m_spriteBatch);
	m_computer->SetName(L"Phone");

	m_player->SetEnemy(m_computer);
	m_computer->SetEnemy(m_player);

	m_viewportWidth = 800;
	m_viewportHeight = 480;

	Start();
}

void CatapultWarsMain::Start() {
	m_wind = Vector2(0, 0);
	m_isHumanTurn = false;
	m_changeTurn = true;
	m_computer->GetCatapult()->CurrentState = CatapultState::Reset;
}

// Updates the application state once per frame.
void CatapultWarsMain::Update() {
	// Update scene objects.
	m_timer.Tick([&]() {
		// Check it one of the players reached 5 and stop the game
		if ((m_player->GetCatapult()->GameOver || m_computer->GetCatapult()->GameOver) &&
			(m_gameOver == false)) {
			m_gameOver = true;

			if (m_player->GetScore() > m_computer->GetScore()) {
				//AudioManager.PlaySound("gameOver_Win");
			} else {
				//AudioManager.PlaySound("gameOver_Lose");
			}

			return;
		}

		// If Reset flag raised and both catapults are not animating - 
		// active catapult finished the cycle, new turn!
		if ((m_player->GetCatapult()->CurrentState == CatapultState::Reset ||
			m_computer->GetCatapult()->CurrentState == CatapultState::Reset) &&
			!(m_player->GetCatapult()->AnimationRunning ||
			m_computer->GetCatapult()->AnimationRunning)) {
			m_changeTurn = true;

			if (m_player->GetIsActive() == true) //Last turn was a human turn?
			{
				m_player->SetIsActive(false);
				m_computer->SetIsActive(true);
				m_isHumanTurn = false;
				m_player->GetCatapult()->CurrentState = CatapultState::Idle;
				m_computer->GetCatapult()->CurrentState = CatapultState::Aiming;
			} else //It was an AI turn
			{
				m_player->SetIsActive(true);
				m_computer->SetIsActive(false);
				m_isHumanTurn = true;
				m_computer->GetCatapult()->CurrentState = CatapultState::Idle;
				m_player->GetCatapult()->CurrentState = CatapultState::Idle;
			}
		}

		if (m_changeTurn) {
			// Update wind
			m_wind = Vector2(rand() % 4 - 1, rand() % (m_maxWind + 1) + m_minWind);

			// Set new wind value to the players and 
			m_player->GetCatapult()->SetWind(m_wind.x > 0 ? m_wind.y : -m_wind.y);
			m_computer->GetCatapult()->SetWind(m_wind.x > 0 ? m_wind.y : -m_wind.y);
			m_changeTurn = false;
		}

		double elapsedSeconds = m_timer.GetElapsedSeconds();

		// Update the players
		m_player->Update(elapsedSeconds);
		m_computer->Update(elapsedSeconds);

		// Updates the clouds position
		UpdateClouds(elapsedSeconds);

		//m_fpsTextRenderer->Update(m_timer);
	});
}

void CatapultWarsMain::HandleInput(int x, int y) {
	if (m_isHumanTurn &&
		(m_player->GetCatapult()->CurrentState == CatapultState::Idle ||
		m_player->GetCatapult()->CurrentState == CatapultState::Aiming)) {
		m_player->HandleInput(x, y);
	}
}

void CatapultWarsMain::IsTouchDown(bool isTouchDown) {
	m_isDragging = isTouchDown;
	if (m_isHumanTurn && !isTouchDown &&
		(m_player->GetCatapult()->CurrentState == CatapultState::Idle ||
		m_player->GetCatapult()->CurrentState == CatapultState::Aiming)) {
		m_player->HandleRelease();
	}
}


// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool CatapultWarsMain::Render() {
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0) {
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	//float dpi = m_deviceResources->GetScalingFactor();
	//XMMATRIX matrix = XMMATRIX(dpi, 0, 0, 0, 0, dpi, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1);
	//m_spriteBatch->Begin(SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, matrix);
	
	m_spriteBatch->Begin();

	DrawBackground();
	DrawComputer();
	DrawPlayer();
	//DrawHud();

	m_spriteBatch->End();

	//m_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.
void CatapultWarsMain::OnDeviceLost() {
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void CatapultWarsMain::OnDeviceRestored() {
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

void CatapultWarsMain::DrawBackground() {
	m_spriteBatch->Draw(m_skyTexture.Get(), Vector2(0, 0), Colors::White);

	m_spriteBatch->Draw(m_cloud1Texture.Get(), m_cloud1Position, Colors::White);

	// Draw the Mountain
	m_spriteBatch->Draw(m_mountainTexture.Get(), Vector2(0, 0), Colors::White);

	// Draw Cloud #2
	m_spriteBatch->Draw(m_cloud2Texture.Get(), m_cloud2Position, Colors::White);

	// Draw the Castle, trees, and foreground 
	m_spriteBatch->Draw(m_foregroundTexture.Get(), Vector2(0, 0), Colors::White);
}


void CatapultWarsMain::DrawHud() {
	if (m_gameOver) {
		if (m_player->GetScore() > m_computer->GetScore()) {
			m_spriteBatch->Draw(m_victoryTexture.Get(), Vector2(m_viewportWidth / 2 - m_victoryTextureWidth / 2, m_viewportHeight / 2 - m_victoryTextureHeight / 2), Colors::White);
		} else {
			m_spriteBatch->Draw(m_defeatTexture.Get(), Vector2(m_viewportWidth / 2 - m_defeatTextureWidth / 2, m_viewportHeight / 2 - m_defeatTextureHeight / 2), Colors::White);
		}


		/*ScreenManager.SpriteBatch.Draw(
			texture,
			new Vector2(ScreenManager.Game.GraphicsDevice.Viewport.Width / 2 - texture.Width / 2,
			ScreenManager.Game.GraphicsDevice.Viewport.Height / 2 - texture.Height / 2),
			Color.White);*/
	} else {
		// Draw Player Hud
		m_spriteBatch->Draw(m_hudBackgroundTexture.Get(), m_playerHUDPosition, Colors::White);
		m_spriteBatch->Draw(m_ammoTypeTexture.Get(), m_playerHUDPosition + Vector2(33, 35), Colors::White);
		DrawString(m_hudFont, m_player->GetScore().ToString()->Data(), m_playerHUDPosition + Vector2(123, 35), Colors::White);
		DrawString(m_hudFont, m_player->GetName(), m_playerHUDPosition + Vector2(40, 1), Colors::Blue);

		// Draw Computer Hud
		m_spriteBatch->Draw(m_hudBackgroundTexture.Get(), m_computerHUDPosition, Colors::White);
		m_spriteBatch->Draw(m_ammoTypeTexture.Get(), m_computerHUDPosition + Vector2(33, 35), Colors::White);
		DrawString(m_hudFont, m_computer->GetScore().ToString()->Data(), m_computerHUDPosition + Vector2(123, 35), Colors::White);
		DrawString(m_hudFont, m_computer->GetName(), m_computerHUDPosition + Vector2(40, 1), Colors::Red);

		// Draw Wind direction
		wchar_t* text = L"WIND";
		auto size = m_hudFont->MeasureString(text);
		Vector2 windarrowScale = Vector2(m_wind.y / 10, 1);
		m_spriteBatch->Draw(m_windArrowTexture.Get(),
			m_windArrowPosition, nullptr, Colors::White, 0, Vector2(0, 0),
			windarrowScale, m_wind.x > 0 ? SpriteEffects::SpriteEffects_None : SpriteEffects::SpriteEffects_FlipHorizontally, 0);

		DrawString(m_hudFont, text, m_windArrowPosition - Vector2(0, XMVectorGetY(size)), Colors::Black);
		if (m_wind.y == 0) {
			text = L"NONE";
			DrawString(m_hudFont, text, m_windArrowPosition, Colors::Black);
		}

		if (m_isHumanTurn) {
			// Prepare human prompt message
			text = !m_isDragging ?
				L"Drag Anywhere to Fire" : L"Release to Fire!";
			size = m_hudFont->MeasureString(text);
		} else {
			// Prepare AI message
			text = L"I'll get you yet!";
			size = m_hudFont->MeasureString(text);
		}

		DrawString(m_hudFont, text,
			Vector2(
			m_viewportWidth / 2 - XMVectorGetX(size) / 2,
			m_viewportHeight - XMVectorGetY(size)),
			Colors::Green);
	}
}

void CatapultWarsMain::DrawComputer() {
	if (!m_gameOver)
		m_computer->Draw();
}

void CatapultWarsMain::DrawPlayer() {
	if (!m_gameOver)
		m_player->Draw();
}

void CatapultWarsMain::DrawString(std::shared_ptr<SpriteFont> font, const wchar_t* text, Vector2 position, FXMVECTOR color) {
	font->DrawString(m_spriteBatch.get(), text, position, Colors::Black, 0, Vector2(0, 0), Vector2(1, 1), SpriteEffects::SpriteEffects_None, 0);
	font->DrawString(m_spriteBatch.get(), text, position + Vector2(1, 1), color, 0, Vector2(0, 0), Vector2(1, 1), SpriteEffects::SpriteEffects_None, 0);
}

void CatapultWarsMain::DrawString(std::shared_ptr<SpriteFont> font, const wchar_t* text, Vector2 position, FXMVECTOR color, float fontScale) {
	font->DrawString(m_spriteBatch.get(), text, position, Colors::Black, 0, Vector2(0, 0), Vector2(1, 1), SpriteEffects::SpriteEffects_None, 0);
	font->DrawString(m_spriteBatch.get(), text, position + Vector2(1, 1), color, 0, Vector2(0, 0), Vector2(fontScale, fontScale), SpriteEffects::SpriteEffects_None, 0);
}

void CatapultWarsMain::UpdateClouds(double elapsedTime) {
	// Move the clouds according to the wind
	int windDirection = m_wind.x > 0 ? 1 : -1;

	m_cloud1Position += Vector2(24.0f, 0.0f) * elapsedTime * windDirection * m_wind.y;
	if (m_cloud1Position.x > m_viewportWidth)
		m_cloud1Position.x = -(int)m_cloud1TextureWidth * 2.0f;
	else if (m_cloud1Position.x < -(int)m_cloud1TextureWidth * 2.0f)
		m_cloud1Position.x = m_viewportWidth;

	m_cloud2Position += Vector2(16.0f, 0.0f) * elapsedTime * windDirection * m_wind.y;
	if (m_cloud2Position.x > m_viewportWidth)
		m_cloud2Position.x = -(int)m_cloud2TextureWidth * 2.0f;
	else if (m_cloud2Position.x < -(int)m_cloud2TextureWidth * 2.0f)
		m_cloud2Position.x = m_viewportWidth;
}