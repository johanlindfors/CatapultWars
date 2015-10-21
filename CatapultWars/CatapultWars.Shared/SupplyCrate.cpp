#include "pch.h"
#include <SupplyCrate.h>

using namespace CatapultWars;

SupplyCrate::SupplyCrate(std::shared_ptr<SpriteBatch>& spriteBatch, String^ idleTextureName, Vector2 catapultCenterPosition, bool isAI)
	: m_lastUpdateState(CrateState::Idle)
{
	m_idleTextureName = idleTextureName;
	m_catapultCenter = catapultCenterPosition;
	m_isAI = isAI;
	m_spriteBatch = spriteBatch;
}

concurrency::task<void> SupplyCrate::Initialize(ID3D11Device* device) {
	m_animationRunning = false;
	m_currentState = CrateState::Idle;



	return concurrency::create_task([&, device]() {
		ComPtr<ID3D11Resource> res;
		DX::ThrowIfFailed(
			CreateWICTextureFromFile(device, m_textureName->Data(), res.ReleaseAndGetAddressOf(), Texture.ReleaseAndGetAddressOf())
			);

		DX::GetTextureSize(res.Get(), &m_textureWidth, &m_textureHeight);
	});
}

void SupplyCrate::Update(double elapsedSeconds) {

}

void SupplyCrate::Draw() {

}

void SupplyCrate::Hit() {

}