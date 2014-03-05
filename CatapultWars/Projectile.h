#pragma once
#include "SimpleMath.h"

namespace CatapultGame {
	class Projectile
	{
	public:
		Projectile();
		void Initialize();
		void Draw();
		void Fire(float velocityX, float velocityY);
		bool UpdateProjectileFlightData(float timeTotal, float timeDelta, float wind, float gravity);

		DirectX::SimpleMath::Vector2 ProjectilePosition;
		DirectX::SimpleMath::Vector2 ProjectileStartPosition;
		float ProjectileTextureWidth;
		float ProjectileTextureHeight;
	};
}

