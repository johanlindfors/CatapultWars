#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\SampleFpsTextRenderer.h"
#include "Human.h"
#include "AI.h"
#include "AudioManager.h"

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
		std::shared_ptr<DirectX::SpriteBatch>	m_spriteBatch;
		std::shared_ptr<DirectX::SpriteFont>	m_hudFont;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_skyTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_foregroundTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_cloud1Texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_cloud2Texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_mountainTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_hudBackgroundTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_ammoTypeTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_windArrowTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_defeatTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		m_victoryTexture;

		DirectX::SimpleMath::Vector2			m_cloud1Position;
		DirectX::SimpleMath::Vector2			m_cloud2Position;
		DirectX::SimpleMath::Vector2			m_playerHUDPosition;
		DirectX::SimpleMath::Vector2			m_computerHUDPosition;
		DirectX::SimpleMath::Vector2			m_windArrowPosition;

		std::shared_ptr<AudioManager>			m_audioManager;

		std::shared_ptr<Human>			m_player;
		std::shared_ptr<AI>				m_computer;
		DirectX::SimpleMath::Vector2	m_wind;
		bool							m_changeTurn;
		bool							m_isHumanTurn;
		bool							m_gameOver;
		const int						m_minWind;
		const int						m_maxWind;
		bool							m_isDragging;
		int								m_viewportWidth;
		int								m_viewportHeight;
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
		void DrawString(std::shared_ptr<DirectX::SpriteFont> font, std::wstring text, DirectX::SimpleMath::Vector2 position, DirectX::FXMVECTOR color);
		void DrawString(std::shared_ptr<DirectX::SpriteFont> font, std::wstring text, DirectX::SimpleMath::Vector2 position, DirectX::FXMVECTOR color, float fontScale);
		void UpdateClouds(double elapsedTime);
	};
}