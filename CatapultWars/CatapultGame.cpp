#include "pch.h"
#include "CatapultGame.h"
#include "DDSTextureLoader.h"

CatapultGame::CatapultGame()
{

}

void CatapultGame::CreateDeviceResources()
{
	Direct3DBase::CreateDeviceResources();


}

void CatapultGame::CreateWindowSizeDependentResources()
{
	Direct3DBase::CreateWindowSizeDependentResources();

	auto device = m_d3dDevice.Get();
	auto context = m_d3dContext.Get();

	m_spriteBatch.reset(new SpriteBatch(context));
	m_spriteBatch->SetRotation(DXGI_MODE_ROTATION_ROTATE90);

	DX::ThrowIfFailed(
		CreateDDSTextureFromFile(device, L"Assets\\Textures\\Backgrounds\\sky.dds", nullptr, m_skyTexture.ReleaseAndGetAddressOf())
		);

}

void CatapultGame::Update(float timeTotal, float timeDelta)
{

}

void CatapultGame::Render()
{
	const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	const float white[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		white
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	m_spriteBatch->Begin();
	
	DrawBackground();

	m_spriteBatch->End();
}

void CatapultGame::DrawBackground() 
{
	m_spriteBatch->Draw(m_skyTexture.Get(),Vector2(0,0), Colors::White);
}