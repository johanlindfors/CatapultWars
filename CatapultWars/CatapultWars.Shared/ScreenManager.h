#pragma once
#include "GameScreen.h"
#include "Common\DeviceResources.h"

namespace CatapultWars {

	ref class ScreenManager {
	internal:
		ScreenManager(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void Initialize();
		void LoadContent(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch);
		void UnloadContent();
		void Update(double elapsedSeconds);
		void Draw(double elapsedSeconds);

		void TraceScreens();
		void AddScreen(GameScreen^ screen);
		void RemoveScreen(GameScreen^ screen);
		std::vector<GameScreen^> GetScreens();

		void FadeBackBufferToBlack(float alpha);
		void SerializeState();
		void DeserializeState();
		void DeleteState();

		property SpriteFont* Font {
			SpriteFont* get() { return m_font.get(); }
		}

		property DX::DeviceResources* DeviceResources {
			DX::DeviceResources* get() { return m_deviceResources.get(); }
		}

	private:
		std::vector<GameScreen^> m_screens;
		std::vector<GameScreen^> m_screensToUpdate;
		std::shared_ptr<DirectX::SpriteBatch> m_spriteBatch;
		std::shared_ptr<DirectX::SpriteFont> m_font;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_blankTexture;
		InputState^ m_input;

		bool m_isInitialized;
		bool m_traceEnabled;

		std::shared_ptr<DX::DeviceResources> m_deviceResources;
	};
}