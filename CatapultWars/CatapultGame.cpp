#include "pch.h"
#include "CatapultGame.h"


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

}

void CatapultGame::Update(float timeTotal, float timeDelta)
{

}

void CatapultGame::Render()
{
	const float midnightBlue[] = { 0.098f, 0.098f, 0.439f, 1.000f };
	
	m_d3dContext->ClearRenderTargetView(
		m_renderTargetView.Get(),
		midnightBlue
		);

	m_d3dContext->ClearDepthStencilView(
		m_depthStencilView.Get(),
		D3D11_CLEAR_DEPTH,
		1.0f,
		0
		);
}