#include "pch.h"
#include "CatapultGame.h"
#include "DDSTextureLoader.h"

namespace CatapultWars{

	CatapultGame::CatapultGame() :
		m_minWind(0),
		m_maxWind(0)
	{
		//EnabledGestures = GestureType.FreeDrag | GestureType.DragComplete | GestureType.Tap;
	}

	void CatapultGame::CreateDeviceResources()
	{
		Direct3DBase::CreateDeviceResources();
	}

	void CatapultGame::CreateWindowSizeDependentResources()
	{
		Direct3DBase::CreateWindowSizeDependentResources();

		auto device = m_d3dDevice.Get();
		auto context = m_d3dContext.Get();

		m_spriteBatch.reset(new SpriteBatch(context));
		m_spriteBatch->SetRotation(DXGI_MODE_ROTATION_ROTATE90);

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
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\cloud2.dds", nullptr, m_cloud2Texture.ReleaseAndGetAddressOf())
			);
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\mountain.dds", nullptr, m_mountainTexture.ReleaseAndGetAddressOf())
			);
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\defeat.dds", nullptr, m_defeatTexture.ReleaseAndGetAddressOf())
			);
		DX::ThrowIfFailed(
			CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\victory.dds", nullptr, m_victoryTexture.ReleaseAndGetAddressOf())
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
		m_hudFont.reset(new SpriteFont(device, L"Assets\\Fonts\\TestHUDFont.spritefont"));

		// Define initial cloud position
		UINT cloud1TextureWidth, cloud1TextureHeight;
		DX::GetTextureSize(cloud1TextureRes.Get(), &cloud1TextureWidth, &cloud1TextureHeight);
		m_cloud1Position = Vector2(224 - cloud1TextureWidth, 32);
		m_cloud2Position = Vector2(64, 90);

		// Define initial HUD positions
		m_playerHUDPosition = Vector2(7, 7);
		m_computerHUDPosition = Vector2(613, 7);
		m_windArrowPosition = Vector2(345, 46);

		// Initialize human & AI players
		m_player = ref new Human();
		m_player->Initialize(device);
		m_player->Name = L"Player";

		m_computer = ref new AI();
		m_computer->Initialize(device);
		m_computer->Name = L"Phone";

		m_player->Enemy = m_computer;
		m_computer->Enemy = m_player;

		Start();
	}

	void CatapultGame::Start()
	{
		m_wind = Vector2(0, 0);
		m_isHumanTurn = false;
		m_changeTurn = true;
		m_computer->Catapult->CurrentState = CatapultState::Reset;
	}

	void CatapultGame::Update(float timeTotal, float timeDelta)
	{
		float elapsed = timeDelta;

		// Check it one of the players reached 5 and stop the game
		if ((m_player->Catapult->GameOver || m_computer->Catapult->GameOver) &&
			(m_gameOver == false))
		{
			m_gameOver = true;

			if (m_player->Score > m_computer->Score)
			{
				//AudioManager.PlaySound("gameOver_Win");
			}
			else
			{
				//AudioManager.PlaySound("gameOver_Lose");
			}

			return;
		}

		// If Reset flag raised and both catapults are not animating - 
		// active catapult finished the cycle, new turn!
		if ((m_player->Catapult->CurrentState == CatapultState::Reset ||
			m_computer->Catapult->CurrentState == CatapultState::Reset) &&
			!(m_player->Catapult->AnimationRunning ||
			m_computer->Catapult->AnimationRunning))
		{
			m_changeTurn = true;

			if (m_player->IsActive == true) //Last turn was a human turn?
			{
				m_player->IsActive = false;
				m_computer->IsActive = true;
				m_isHumanTurn = false;
				m_player->Catapult->CurrentState = CatapultState::Idle;
				m_computer->Catapult->CurrentState = CatapultState::Aiming;
			}
			else //It was an AI turn
			{
				m_player->IsActive = true;
				m_computer->IsActive = false;
				m_isHumanTurn = true;
				m_computer->Catapult->CurrentState = CatapultState::Idle;
				m_player->Catapult->CurrentState = CatapultState::Idle;
			}
		}

		if (m_changeTurn)
		{
			// Update wind
			m_wind = Vector2(rand() % 4 - 1, rand() % (m_maxWind + 1) + m_minWind);

			// Set new wind value to the players and 
			m_player->Catapult->Wind = m_wind.x > 0 ? m_wind.y : -m_wind.y;
			m_computer->Catapult->Wind = m_wind.x > 0 ? m_wind.y : -m_wind.y;
			m_changeTurn = false;
		}

		// Update the players
		m_player->Update(timeTotal, timeDelta);
		m_computer->Update(timeTotal, timeDelta);

		// Updates the clouds position
		UpdateClouds(elapsed);
	}

	void CatapultGame::UpdateClouds(float elapsed)
	{

	}

	void CatapultGame::Render()
	{
		const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
		const float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		m_d3dContext->ClearRenderTargetView(
			m_renderTargetView.Get(),
			white
			);

		m_d3dContext->ClearDepthStencilView(
			m_depthStencilView.Get(),
			D3D11_CLEAR_DEPTH,
			1.0f,
			0
			);
		m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

		m_spriteBatch->Begin();

		DrawBackground();

		m_spriteBatch->End();
	}

	void CatapultGame::DrawBackground()
	{
		m_spriteBatch->Draw(m_skyTexture.Get(), Vector2(0, 0), Colors::White);

		m_spriteBatch->Draw(m_cloud1Texture.Get(), m_cloud1Position, Colors::White);

		// Draw the Mountain
		m_spriteBatch->Draw(m_mountainTexture.Get(), Vector2(0, 0), Colors::White);

		// Draw Cloud #2
		m_spriteBatch->Draw(m_cloud2Texture.Get(), m_cloud2Position, Colors::White);

		// Draw the Castle, trees, and foreground 
		m_spriteBatch->Draw(m_foregroundTexture.Get(), Vector2(0, 0), Colors::White);
	}
}