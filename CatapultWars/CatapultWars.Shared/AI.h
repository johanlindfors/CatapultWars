#pragma once
#include "Player.h"
#include "AudioManager.h"

namespace CatapultWars {

	class AI final: public Player
	{
	public:
		AI();

		virtual concurrency::task<void> Initialize(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager) override;
		virtual void Update(double elapsedSeconds) override;
		virtual void Draw() override;
	};
}