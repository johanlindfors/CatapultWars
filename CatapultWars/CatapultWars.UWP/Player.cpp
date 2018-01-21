#include "pch.h"

using namespace CatapultWars;

Player::Player()
: MinShotStrength(150)
, MaxShotStrength(400)
{

}

void Player::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch)
{
	m_score = 0;
}

void Player::Update(double elapsedSeconds)
{
	Catapult->Update(elapsedSeconds);
}

void Player::Draw()
{
	Catapult->Draw();
}