#include "pch.h"
#include "Human.h"
#include <math.h>

namespace CatapultWars {


	Human::Human(PlayerSide playerSide)
		: m_maxDragDelta(Vector2(480, 800).Length())
		, m_spriteEffects(SpriteEffects::SpriteEffects_None)
	{
		m_playerSide = playerSide;
		String^ idleTextureName = "";

		if (playerSide == PlayerSide::Left) {
			m_catapultPosition = Vector2(140, 332);
			idleTextureName = L"Assets\\Textures\\Catapults\\Blue\\blueIdle\\blueIdle.png";
		} else {
			m_catapultPosition = Vector2(600, 332);
			m_spriteEffects = SpriteEffects::SpriteEffects_FlipHorizontally;
			idleTextureName = L"Assets\\Textures\\Catapults\\Blue\\blueIdle\\redIdle.png";
		}
		Catapult = ref new CatapultWars::Catapult(
			L"Assets\\Textures\\Catapults\\Blue\\blueIdle\\blueIdle.png",
			m_catapultPosition, m_spriteEffects, playerSide == PlayerSide::Left ? false : true, true);
	}

	concurrency::task<void> Human::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager)
	{
		DX::ThrowIfFailed(CreateWICTextureFromFile(device, L"Assets\\Textures\\HUD\\arrow.png", nullptr, m_arrow.ReleaseAndGetAddressOf()));
		m_spriteBatch = spriteBatch;

		return Catapult->Initialize(device, spriteBatch, audioManager);
	}

	void Human::HandleInput(int x, int y)
	{
		Catapult->CurrentState = CatapultState::Aiming; 
		m_delta = Vector2(x, y);
		auto deltaLength = m_delta.Length();
		Catapult->ShotStrength =  deltaLength / m_maxDragDelta;
		float baseScale = 0.001f;
		m_arrowScale = baseScale * deltaLength;
		IsDragging = true;
	}

	void Human::HandleRelease()
	{
		Catapult->ShotVelocity = MinShotVelocity + Catapult->ShotStrength *
			(MaxShotVelocity - MinShotVelocity);
		//Catapult->Fire(Catapult->ShotVelocity, Catapult->ShotAngle);
		Catapult->CurrentState = CatapultState::Firing;
		Catapult->ShotAngle = MaxShotAngle;
		m_delta = Vector2(0, 0);

		ResetDragState();
	}

	void Human::Update(double elapsedSeconds)
	{
		Player::Update(elapsedSeconds);
	}

	void Human::Draw()
	{
		if (IsDragging)
			DrawDragArrow(m_arrowScale);

		Player::Draw();
	}

	void Human::DrawDragArrow(float arrowScale)
	{
		m_spriteBatch->Draw(m_arrow.Get(), m_catapultPosition + Vector2(0, -40),
			nullptr, Colors::Blue, 0, Vector2(0, 0), Vector2(m_arrowScale, 0.1f), SpriteEffects::SpriteEffects_None, 0);
	}

	void Human::ResetDragState()
	{
		//m_firstSample = nullptr;
		//m_prevSample = nullptr;
		IsDragging = false;
		m_arrowScale = 0;
		Catapult->ShotStrength = 0;
	}
}