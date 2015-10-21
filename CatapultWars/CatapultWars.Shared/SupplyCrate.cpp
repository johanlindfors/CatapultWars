#include "pch.h"
#include <SupplyCrate.h>

using namespace CatapultWars;

SupplyCrate::SupplyCrate() 
	: m_lastUpdateState(CrateState::Idle)
	//, m_positionXOffset(-100)
	//, m_positionYOffset(30)
{

}

void SupplyCrate::Initialize() {
	//ComPtr<ID3D11Resource> res;
	//DX::ThrowIfFailed(
	//	CreateWICTextureFromFile(device, m_textureName->Data(), res.ReleaseAndGetAddressOf(), ProjectileTexture.ReleaseAndGetAddressOf())
	//	);

	//DX::GetTextureSize(res.Get(), &m_textureWidth, &m_textureHeight);

	//ProjectileTextureWidth = m_textureWidth;
	//ProjectileTextureHeight = m_textureHeight;
}