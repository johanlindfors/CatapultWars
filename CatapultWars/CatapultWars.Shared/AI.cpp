#include "pch.h"
#include "AI.h"

using namespace std;
using namespace CatapultWars;
using namespace concurrency;
using namespace DirectX;
using namespace DirectX::SimpleMath;

AI::AI()
{
	Catapult = make_shared<CatapultWars::Catapult>(
		L"Assets\\Textures\\Catapults\\Red\\redIdle\\redIdle.png",
		Vector2(600,332), 
		SpriteEffects::SpriteEffects_FlipHorizontally, 
		true, 
		false);
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
