#include "pch.h"
#include "Projectile.h"
#include "Common\DirectXHelper.h"
#include "DDSTextureLoader.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace CatapultWars;
using namespace Platform;
using namespace Microsoft::WRL;
using namespace concurrency;

Projectile::Projectile(shared_ptr<SpriteBatch>& spriteBatch, vector<shared_ptr<Projectile>>& activeProjectiles, wstring textureName, Vector2 startPosition, float groundHitOffset, bool isRightPlayer, float gravity)
	: m_spriteBatch(spriteBatch)
	, m_activeProjectiles(activeProjectiles)
	, m_textureName(textureName)
	, m_isRightPlayer(isRightPlayer)
	, m_hitOffset(groundHitOffset)
	, m_gravity(gravity)
	, m_projectileRotation(0)
{	
	ProjectileStartPosition = startPosition;
}

task<void> Projectile::Initialize(ID3D11Device* device)
{
	return create_task([&,device]() {
		ComPtr<ID3D11Resource> res;
		DX::ThrowIfFailed(
			CreateWICTextureFromFile(device, m_textureName.c_str(), res.ReleaseAndGetAddressOf(), ProjectileTexture.ReleaseAndGetAddressOf())
			);

		DX::GetTextureSize(res.Get(), &m_textureWidth, &m_textureHeight);

		ProjectileTextureWidth = m_textureWidth;
		ProjectileTextureHeight = m_textureHeight;
	});
}

void Projectile::Update(double elapsedSeconds) {
	switch (State)
	{
	case CatapultWars::InFlight:
		UpdateProjectileFlight(elapsedSeconds);
		break;
	case CatapultWars::HitGround:
		UpdateProjectileHit(elapsedSeconds);
		break;	
	default:
		// Nothing to update in other states
		break;
	}
}

void Projectile::UpdateProjectileHit(double elapsedSeconds) {
	if (HitAnimation == nullptr) {
		return;
	}

	if (HitAnimation->IsActive == false) {
		State = ProjectileState::Destroyed;
		return;
	}

	HitAnimation->Update();
}

void Projectile::UpdateProjectileFlight(double elapsedSeconds) {
	UpdateProjectileFlightData(elapsedSeconds, Wind, m_gravity);
}

void Projectile::Draw()
{
	UINT width = 0, height = 0; 

	switch (State)
	{
	case CatapultWars::InFlight:
		m_spriteBatch->Draw(ProjectileTexture.Get(), ProjectilePosition, nullptr, Colors::White, m_projectileRotation, Vector2(m_textureWidth / 2, m_textureHeight / 2), 1.0f, SpriteEffects::SpriteEffects_None, 0.0f);
		break;
	case CatapultWars::HitGround:
		if (HitAnimation != nullptr) {
			HitAnimation->Draw(m_spriteBatch, ProjectileHitPosition, SpriteEffects::SpriteEffects_None);
		}
		break;
	default:
		// Nothing to draw in this case
		break;
	}
}

void Projectile::UpdateProjectileFlightData(double elapsedSeconds, float wind, float gravity)
{
	bool groundHit = false;
	m_flightTime += elapsedSeconds;

	int direction = m_isRightPlayer ? -1 : 1;

	float previousXPosition = ProjectilePosition.x;
	float previousYPosition = ProjectilePosition.y;

	ProjectilePosition.x = ProjectileStartPosition.x +
		(direction * m_projectileInitialVelocity.x * m_flightTime) +
		0.5f * (8 * wind * (float)pow(m_flightTime, 2));

	CurrentVelocity.x = m_projectileInitialVelocity.x + 8 * Wind * m_flightTime;

	ProjectilePosition.y = ProjectileStartPosition.y -
		(m_projectileInitialVelocity.y * m_flightTime) +
		0.5f * (gravity * (float)pow(m_flightTime, 2));

	CurrentVelocity.y = m_projectileInitialVelocity.y - m_gravity * m_flightTime;

	m_projectileRotation += (m_projectileInitialVelocity.x * 0.5f) * XM_PI / 180;
	
	if (ProjectilePosition.y >= 332 + m_hitOffset)
	{
		ProjectilePosition.x = previousXPosition;
		ProjectilePosition.y = previousYPosition;

		ProjectileHitPosition = Vector2(previousXPosition, 332);

		State = ProjectileState::HitGround;
	}	
}

void Projectile::Fire(float velocityX, float velocityY)
{
	ProjectilePosition = ProjectileStartPosition;
	m_projectileInitialVelocity.x = velocityX;
	m_projectileInitialVelocity.y = velocityY;
	CurrentVelocity.x = velocityX;
	CurrentVelocity.y = velocityY;

	// Reset calculation variables
	m_flightTime = 0;
	State = ProjectileState::InFlight;
	HitHandled = false;
}