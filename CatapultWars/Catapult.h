#pragma once
#include "property.hpp"

namespace CatapultGame {

	enum CatapultState {
		Idle = 0x0,
		Aiming = 0x1,
		Firing = 0x2,
		ProjectileFlying = 0x4,
		ProjectileHit = 0x8,
		Hit = 0x10,
		Reset = 0x20,
		Stalling = 0x40
	};

	class Catapult
	{
	public:
		Catapult();
		~Catapult();

		void Initialize();
		void Update(float timeTotal, float timeDelta);
		void Draw();

		property<Catapult, CatapultState, READ_WRITE> CurrentState;
		property<Catapult, bool, READ_WRITE> AnimationRunning;
		property<Catapult, float, READ_WRITE> ShotStrength;
		property<Catapult, float, READ_WRITE> ShotVelocity;
	};

}