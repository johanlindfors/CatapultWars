#include "pch.h"
#include "AI.h"

using namespace CatapultWars;
using namespace concurrency;

AI::AI() {
	Catapult = ref new CatapultWars::Catapult(
		L"Assets\\Textures\\Catapults\\Red\\redIdle\\redIdle.png",
		Vector2(600, 332), DirectX::SpriteEffects::SpriteEffects_FlipHorizontally, true, false);
	// (game, screenSpriteBatch,
}

task<void> AI::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager) {
	return create_task(Catapult->Initialize(device, spriteBatch, audioManager));
}

void AI::Update(double elapsedSeconds) {
	if (Catapult->CurrentState == CatapultState::Aiming &&
		!Catapult->AnimationRunning)
	{
		// Fire at a random strength
		float shotVelocity = rand() % ((int)MaxShotVelocity - (int)MinShotVelocity) + (int)MinShotVelocity;
		float shotAngle = MinShotAngle + rand() % 100 / 100 * (MaxShotAngle - MinShotAngle);

		Catapult->ShotStrength = (shotVelocity / MaxShotVelocity);
		Catapult->ShotVelocity = shotVelocity;
		Catapult->ShotAngle = shotAngle;
	}
	Player::Update(elapsedSeconds);
}

void AI::Draw() {
	Player::Draw();
}
