#pragma once
#include "GameScreen.h"

namespace CatapultWars {

	ref class BackgroundScreen : GameScreen {

	internal:
		BackgroundScreen(ScreenManager^ manager);
		void LoadContent(ID3D11Device* device) override;
		void Draw(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, double elapsedSeconds) override;

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_background;
	};
}
