#include "pch.h"
#include "AI.h"

using namespace CatapultWars;

AI::AI()
{
	SetCatapult(std::make_shared<CatapultWars::Catapult>(
		L"Assets\\Textures\\Catapults\\Red\\redIdle\\redIdle.dds",
		Vector2(600,332),DirectX::SpriteEffects::SpriteEffects_FlipHorizontally, true));
}

void AI::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch)
{	
	GetCatapult()->Initialize(device, spriteBatch);

	Player::Initialize(device, spriteBatch);
}

void AI::Update(double elapsedSeconds)
{
	if (GetCatapult()->CurrentState == CatapultState::Aiming &&
		!GetCatapult()->AnimationRunning)
	{
		// Fire at a random strength
		float shotVelocity = rand() % ((int)MaxShotStrength-(int)MinShotStrength)+(int)MinShotStrength;

		GetCatapult()->ShotStrength = (shotVelocity / MaxShotStrength);
		GetCatapult()->ShotVelocity = shotVelocity;
	}
	Player::Update(elapsedSeconds);
}

void AI::Draw()
{
	Player::Draw();
}
