#include "pch.h"

using namespace std;
using namespace DirectX;
using namespace CatapultWars;

Player::Player()
	: MinShotStrength(150)
	, MaxShotStrength(400)
	, Score(0)
	, Health(100)
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