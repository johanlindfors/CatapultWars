#include "pch.h"

using namespace CatapultWars;

Player::Player()
: MinShotStrength(150)
, MaxShotStrength(400)
, m_score(0)
{

}

void Player::Update(double elapsedSeconds)
{
	Catapult->Update(elapsedSeconds);
}

void Player::Draw()
{
	Catapult->Draw();
}