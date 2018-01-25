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

Projectile::Projectile(shared_ptr<SpriteBatch>& spriteBatch, wstring textureName, Vector2 startPosition, float groundHitOffset, bool isAI, float gravity)
	: m_spriteBatch(spriteBatch)
	, m_textureName(textureName)
	, m_isAI(isAI)
	, m_hitOffset(groundHitOffset)
	, m_gravity(gravity)
	, m_projectileRotation(0)
{	
	ProjectileStartPosition = startPosition;
}

void Projectile::Initialize(ID3D11Device* device)
{
	ComPtr<ID3D11Resource> res;
	DX::ThrowIfFailed(
		CreateWICTextureFromFile(device, m_textureName.c_str(), res.ReleaseAndGetAddressOf(), ProjectileTexture.ReleaseAndGetAddressOf())
		);
		
	DX::GetTextureSize(res.Get(), &m_textureWidth, &m_textureHeight);

	ProjectileTextureWidth = m_textureWidth;
	ProjectileTextureHeight = m_textureHeight;
}

void Projectile::Draw()
{
	UINT width = 0, height = 0; 
	m_spriteBatch->Draw(ProjectileTexture.Get(), ProjectilePosition, nullptr, Colors::White, m_projectileRotation, Vector2(m_textureWidth / 2, m_textureHeight / 2), 1.0f, SpriteEffects::SpriteEffects_None, 0.0f);
}

bool Projectile::UpdateProjectileFlightData(double elapsedSeconds, float wind, float gravity)
{
	bool groundHit = false;
	m_flightTime += elapsedSeconds;

	int direction = m_isAI ? -1 : 1;

	float previousXPosition = ProjectilePosition.x;
	float previousYPosition = ProjectilePosition.y;

	ProjectilePosition.x = ProjectileStartPosition.x +
		(direction * m_projectileVelocity.x * m_flightTime) +
		0.5f * (8 * wind * (float)pow(m_flightTime, 2));

	ProjectilePosition.y = ProjectileStartPosition.y -
		(m_projectileVelocity.y * m_flightTime) +
		0.5f * (gravity * (float)pow(m_flightTime, 2));

	m_projectileRotation += (m_projectileVelocity.x * 0.5f) * XM_PI / 180;
	
	if (ProjectilePosition.y >= 332 + m_hitOffset)
	{
		ProjectilePosition.x = previousXPosition;
		ProjectilePosition.y = previousYPosition;

		ProjectileHitPosition = Vector2(previousXPosition, 332);

		groundHit = true;
	}
	else
	{
		groundHit = false;
	}
	return groundHit;
}

void Projectile::Fire(float velocityX, float velocityY)
{
	m_projectileVelocity.x = velocityX;
	m_projectileVelocity.y = velocityY;
	m_projectileInitialVelocityY = m_projectileVelocity.y;

	// Reset calculation variables
	m_flightTime = 0;
}