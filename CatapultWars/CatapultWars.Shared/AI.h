#pragma once
#include "Player.h"
#include "AudioManager.h"

namespace CatapultWars {

	ref class AI sealed : public Player
	{
	public:
		AI();

	internal:
		virtual concurrency::task<void> Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager) override;
		virtual void Update(double elapsedSeconds) override;
		virtual void Draw() override;
	};
}