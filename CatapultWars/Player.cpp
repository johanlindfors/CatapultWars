#include "pch.h"
#include "Player.h"

using namespace CatapultGame;

Player::Player()
: MinShotStrength(150)
, MaxShotStrength(400)
{
	Enemy.setContainer(this);
	Enemy.setter(&Player::setEnemy);

	Score.setContainer(this);
	Score.setter(&Player::setScore);
	Score.getter(&Player::getScore);

	Name.setContainer(this);
	Name.setter(&Player::setName);
	Name.getter(&Player::getName);

	IsActive.setContainer(this);
	IsActive.setter(&Player::setIsActive);
	IsActive.getter(&Player::getIsActive);
}

void Player::Initialize()
{
	m_score = 0;
}

Player::~Player()
{
	if (m_enemy != nullptr)
		delete(m_enemy);
}

void Player::Update(float timeTotal, float timeDelta)
{
	m_enemy->Update(timeTotal, timeDelta);
}

void Player::Draw()
{
	m_enemy->Draw();
}