#pragma once
namespace CatapultGame {
	class Projectile
	{
	public:
		Projectile();
		void Initialize();
		void Draw();
		void Fire(float velocityX, float velocityY);
		bool UpdateProjectileFlightData(float timeTotal, float timeDelta, float wind, float gravity);

		DirectX::XMFLOAT2 ProjectilePosition;
		DirectX::XMFLOAT2 ProjectileStartPosition;
		float ProjectileTextureWidth;
		float ProjectileTextureHeight;
	};
}

