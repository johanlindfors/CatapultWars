#pragma once

#include "Direct3DBase.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "SimpleMath.h"
#include "Human.h"
#include "AI.h"

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

namespace CatapultWars {
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
		void Start();
		void UpdateClouds(float elapsed);
		std::shared_ptr<SpriteBatch> m_spriteBatch;
		std::unique_ptr<SpriteFont> m_hudFont;

		ComPtr<ID3D11ShaderResourceView> m_skyTexture;
		ComPtr<ID3D11ShaderResourceView> m_foregroundTexture;
		ComPtr<ID3D11ShaderResourceView> m_cloud1Texture;
		ComPtr<ID3D11ShaderResourceView> m_cloud2Texture;
		ComPtr<ID3D11ShaderResourceView> m_mountainTexture;
		ComPtr<ID3D11ShaderResourceView> m_hudBackgroundTexture;
		ComPtr<ID3D11ShaderResourceView> m_ammoTypeTexture;
		ComPtr<ID3D11ShaderResourceView> m_windArrowTexture;
		ComPtr<ID3D11ShaderResourceView> m_defeatTexture;
		ComPtr<ID3D11ShaderResourceView> m_victoryTexture;

		Vector2			m_cloud1Position;
		Vector2			m_cloud2Position;
		Vector2			m_playerHUDPosition;
		Vector2			m_computerHUDPosition;
		Vector2			m_windArrowPosition;

		Human^			m_player;
		AI^				m_computer;
		Vector2			m_wind;
		bool			m_changeTurn;
		bool			m_isHumanTurn;
		bool			m_gameOver;
		const int		m_minWind;
		const int		m_maxWind;
		bool			m_isDragging;
	};

}