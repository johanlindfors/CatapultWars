#include "pch.h"

using namespace CatapultWars;

void Player::Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch)
{
	m_score = 0;
}

void Player::Update(double elapsedSeconds)
{
	m_catapult->Update(elapsedSeconds);
}

void Player::Draw()
{
	m_catapult->Draw();
}