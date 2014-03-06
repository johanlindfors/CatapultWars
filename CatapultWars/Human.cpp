#include "pch.h"
#include "Human.h"

namespace CatapultWars {
	Human::Human() :
		m_maxDragDelta(Vector2(480, 800).Length())
	{
		m_catapultPosition = Vector2(140, 332);

		Catapult = ref new CatapultWars::Catapult(
			L"Assets\\Textures\\Catapults\\Blue\\blueIdle\\blueIdle.dds",
			m_catapultPosition, DirectX::SpriteEffects::SpriteEffects_None, false);
	}

	void Human::Initialize(ID3D11Device* device)
	{
		DX::ThrowIfFailed(CreateDDSTextureFromFile(device, L"Assets\\Textures\\HUD\\arrow.dds", nullptr, m_arrow.ReleaseAndGetAddressOf()));

		Catapult->Initialize(device);

		Player::Initialize(device);
	}

	void Human::HandleInput()
	{

	}

	void Human::Update(float timeTotal, float timeDelta)
	{

	}

	void Human::Draw()
	{
		if (IsDragging)
			DrawDragArrow(m_arrowScale);

		Player::Draw();
	}

	void Human::DrawDragArrow(float arrowScale)
	{
		//m_spriteBatch
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