#pragma once

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

namespace CatapultWars {
	class Projectile
	{
	public:
		Projectile(shared_ptr<SpriteBatch> spriteBatch, wchar_t* textureName, Vector2 startPosition, float groundHitOffset, bool isAI, float gravity);
		void Initialize(ID3D11Device* device);
		void Draw();
		void Fire(float velocityX, float velocityY);
		bool UpdateProjectileFlightData(float timeTotal, float timeDelta, float wind, float gravity);

		Vector2 ProjectilePosition;
		Vector2 ProjectileStartPosition;
		Vector2 ProjectileHitPosition;
		float ProjectileTextureWidth;
		float ProjectileTextureHeight;
		ComPtr<ID3D11ShaderResourceView> ProjectileTexture;

	private:
		wchar_t*					m_textureName;
		Vector2						m_projectileVelocity;
		float						m_projectileInitialVelocityY;
		Vector2						m_projectileRotationPosition;
		float						m_projectileRotation;
		float						m_flightTime;
		bool						m_isAI;
		float						m_hitOffset;
		float						m_gravity;
		shared_ptr<SpriteBatch>		m_spriteBatch;
		UINT						m_textureWidth;
		UINT						m_textureHeight;
	};
}

