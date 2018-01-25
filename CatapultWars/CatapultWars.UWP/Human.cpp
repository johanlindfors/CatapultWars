#include "pch.h"
#include "Human.h"

using namespace DX;
using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace CatapultWars;


Human::Human()
	: m_maxDragDelta(Vector2(480, 800).Length())
	, m_catapultPosition(Vector2(140, 332))
{

	SetCatapult(make_shared<Catapult>(
		L"Assets\\Textures\\Catapults\\Blue\\blueIdle\\blueIdle.dds",
		m_catapultPosition, SpriteEffects::SpriteEffects_None, false));
}

void Human::Initialize(ID3D11Device* device, shared_ptr<SpriteBatch>& spriteBatch)
{
	ThrowIfFailed(CreateDDSTextureFromFile(device, L"Assets\\Textures\\HUD\\arrow.dds", nullptr, m_arrow.ReleaseAndGetAddressOf()));

	GetCatapult()->Initialize(device, spriteBatch);

	Player::Initialize(device, spriteBatch);

	m_spriteBatch = spriteBatch;
}

void Human::HandleInput(int x, int y)
{
	GetCatapult()->CurrentState =  CatapultState::Aiming;
	m_delta = Vector2(x, y);
	auto deltaLength = m_delta.Length();
	GetCatapult()->ShotStrength = deltaLength / m_maxDragDelta;
	float baseScale = 0.001f;
	m_arrowScale = baseScale * deltaLength;
	IsDragging = true;
}

void Human::HandleRelease()
{
	GetCatapult()->ShotVelocity = MinShotStrength + GetCatapult()->ShotStrength *
		(MaxShotStrength - MinShotStrength);
	GetCatapult()->Fire(GetCatapult()->ShotVelocity);
	GetCatapult()->CurrentState = CatapultState::Firing;
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
	GetCatapult()->ShotStrength = 0;
}