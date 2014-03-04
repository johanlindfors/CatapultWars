#include "pch.h"
#include "AI.h"
#include "Catapult.h"

using namespace CatapultGame;

AI::AI()
{
	Catapult = new CatapultGame::Catapult();
	// (game, screenSpriteBatch,
	//	"Textures/Catapults/Red/redIdle/redIdle",
	//	new Vector2(600, 332), SpriteEffects.FlipHorizontally, true);
}

AI::~AI()
{
}

void AI::Initialize()
{	
	Catapult->Initialize();

	Player::Initialize();
}

void AI::Update(float timeTotal, float timeDelta)
{
	if (Catapult->CurrentState == CatapultState::Aiming &&
		!Catapult->AnimationRunning)
	{
		// Fire at a random strength
		float shotVelocity = rand() % ((int)MaxShotStrength-(int)MinShotStrength)+(int)MinShotStrength;

		Catapult->ShotStrength = (shotVelocity / MaxShotStrength);
		Catapult->ShotVelocity = shotVelocity;
	}
	Player::Update(timeTotal, timeDelta);
}

void AI::Draw()
{
	Player::Draw();
}
