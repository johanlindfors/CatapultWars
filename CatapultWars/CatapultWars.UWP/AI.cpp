#include "pch.h"
#include "AI.h"

using namespace std;
using namespace CatapultWars;
using namespace DirectX;
using namespace DirectX::SimpleMath;

AI::AI()
{
	SetCatapult(make_shared<Catapult>(
		L"Assets\\Textures\\Catapults\\Red\\redIdle\\redIdle.dds",
		Vector2(600,332), 
		SpriteEffects::SpriteEffects_FlipHorizontally, 
		true));
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
