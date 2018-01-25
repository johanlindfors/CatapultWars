#include "pch.h"

using namespace std;
using namespace DirectX;
using namespace CatapultWars;

void Player::Initialize(ID3D11Device* device, shared_ptr<SpriteBatch>& spriteBatch)
{
	Score = 0;
}

void Player::Update(double elapsedSeconds)
{
	Catapult->Update(elapsedSeconds);
}

void Player::Draw()
{
	Catapult->Draw();
}