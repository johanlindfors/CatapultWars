#include "pch.h"
#include "Human.h"

namespace CatapultWars {
	Human::Human()
		: m_maxDragDelta(Vector2(480, 800).Length())
		, m_catapultPosition(Vector2(140,332))
	{

		Catapult = ref new CatapultWars::Catapult(
			L"Assets\\Textures\\Catapults\\Blue\\blueIdle\\blueIdle.dds",
			m_catapultPosition, DirectX::SpriteEffects::SpriteEffects_None, false);
	}

	void Human::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch)
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Assets\\Textures\\HUD\\arrow.dds", nullptr, m_arrow.ReleaseAndGetAddressOf()));

		Catapult->Initialize(device, spriteBatch);

		Player::Initialize(device, spriteBatch);

		m_spriteBatch = spriteBatch;
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
		Catapult->ShotVelocity = MinShotStrength + Catapult->ShotStrength *
			(MaxShotStrength - MinShotStrength);
		Catapult->Fire(Catapult->ShotVelocity);
		Catapult->CurrentState = CatapultState::Firing;
		m_delta = Vector2(0, 0);

		ResetDragState();
	}

	void Human::Update()
	{
		Player::Update();
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