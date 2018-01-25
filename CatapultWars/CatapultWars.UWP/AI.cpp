#include "pch.h"
#include "AI.h"

using namespace std;
using namespace CatapultWars;
using namespace DirectX;
using namespace DirectX::SimpleMath;

AI::AI()
{
	Catapult = make_shared<CatapultWars::Catapult>(
		L"Assets\\Textures\\Catapults\\Red\\redIdle\\redIdle.dds",
		Vector2(600,332), 
		SpriteEffects::SpriteEffects_FlipHorizontally, 
		true);
}

void AI::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch)
{	
	Catapult->Initialize(device, spriteBatch);

	Player::Initialize(device, spriteBatch);
}

void AI::Update(double elapsedSeconds)
{
	if (Catapult->CurrentState == CatapultState::Aiming && !Catapult->AnimationRunning)
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
