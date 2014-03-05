#pragma once

#include "Direct3DBase.h"
#include "SpriteBatch.h"
#include "SimpleMath.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

ref class CatapultGame sealed : public Direct3DBase
{
public:
	CatapultGame();

	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

	// Method for updating time-dependent objects.
	void Update(float timeTotal, float timeDelta);

private:
	void DrawBackground();
	std::shared_ptr<SpriteBatch> m_spriteBatch;

	ComPtr<ID3D11ShaderResourceView> m_skyTexture;
};

