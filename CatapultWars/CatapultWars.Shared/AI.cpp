#include "pch.h"
#include "AI.h"

using namespace CatapultWars;
using namespace concurrency;

AI::AI()
{
	Catapult = ref new CatapultWars::Catapult(
		L"Assets\\Textures\\Catapults\\Red\\redIdle\\redIdle.png",
		Vector2(600,332),DirectX::SpriteEffects::SpriteEffects_FlipHorizontally, true, true);
	// (game, screenSpriteBatch,
}

task<void> AI::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager)
{	
	return create_task(Catapult->Initialize(device, spriteBatch, audioManager));
}

void AI::Update(double elapsedSeconds)
{
	if (Catapult->CurrentState == CatapultState::Aiming &&
		!Catapult->AnimationRunning)
	{
		// Fire at a random strength
		float shotVelocity = rand() % ((int)MaxShotStrength-(int)MinShotStrength)+(int)MinShotStrength;

		Catapult->ShotStrength = (shotVelocity / MaxShotStrength);
		Catapult->ShotVelocity = shotVelocity;
	}
	Player::Update(elapsedSeconds);
}

void AI::Draw()
{
	Player::Draw();
}
