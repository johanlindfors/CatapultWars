#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\SampleFpsTextRenderer.h"
#include "Human.h"
#include "AI.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

// Renders Direct2D and 3D content on the screen.
namespace CatapultWars
{
	class CatapultWarsMain : public DX::IDeviceNotify
	{
	public:
		CatapultWarsMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~CatapultWarsMain();
		void CreateWindowSizeDependentResources();
		void Update();
		void HandleInput(int x, int y);
		void IsTouchDown(bool isTouchDown);
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<SampleFpsTextRenderer>	m_fpsTextRenderer;
		std::shared_ptr<SpriteBatch>			m_spriteBatch;
		std::shared_ptr<SpriteFont>				m_hudFont;

		ComPtr<ID3D11ShaderResourceView>		m_skyTexture;
		ComPtr<ID3D11ShaderResourceView>		m_foregroundTexture;
		ComPtr<ID3D11ShaderResourceView>		m_cloud1Texture;
		ComPtr<ID3D11ShaderResourceView>		m_cloud2Texture;
		ComPtr<ID3D11ShaderResourceView>		m_mountainTexture;
		ComPtr<ID3D11ShaderResourceView>		m_hudBackgroundTexture;
		ComPtr<ID3D11ShaderResourceView>		m_ammoTypeTexture;
		ComPtr<ID3D11ShaderResourceView>		m_windArrowTexture;
		ComPtr<ID3D11ShaderResourceView>		m_defeatTexture;
		ComPtr<ID3D11ShaderResourceView>		m_victoryTexture;

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
		int				m_viewportWidth;
		int				m_viewportHeight;
		UINT m_cloud1TextureWidth, m_cloud1TextureHeight;
		UINT m_cloud2TextureWidth, m_cloud2TextureHeight;
		UINT m_defeatTextureWidth, m_defeatTextureHeight;
		UINT m_victoryTextureWidth, m_victoryTextureHeight;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		void DrawBackground();
		void DrawComputer();
		void DrawPlayer();
		void Start();
		void DrawHud();
		void DrawString(std::shared_ptr<SpriteFont> font, Platform::String^ text, Vector2 position, FXMVECTOR color);
		void DrawString(std::shared_ptr<SpriteFont> font, Platform::String^ text, Vector2 position, FXMVECTOR color, float fontScale);
		void UpdateClouds();
	};
}