#include "pch.h"

using namespace CatapultWars;

Player::Player()
	: m_score(0)
	, m_health(100)
	, Weapon(WeaponType::Normal)
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