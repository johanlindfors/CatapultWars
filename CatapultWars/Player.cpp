#include "pch.h"

using namespace CatapultWars;

Player::Player()
: MinShotStrength(150)
, MaxShotStrength(400)
{

}

void Player::Initialize(ID3D11Device* device)
{
	m_score = 0;
}

void Player::Update(float timeTotal, float timeDelta)
{
	Catapult->Update(timeTotal, timeDelta);
}

void Player::Draw()
{
	Catapult->Draw();
}