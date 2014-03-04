#pragma once
namespace CatapultGame {
	class Projectile
	{
	public:
		Projectile();
		void Initialize();
		void Draw();
		void Fire(float velocityX, float velocityY);

		DirectX::XMFLOAT2 ProjectilePosition;
		float ProjectileTextureWidth;
		float ProjectileTextureHeight;
	};
}

