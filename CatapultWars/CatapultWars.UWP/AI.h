#pragma once
#include "Player.h"

namespace CatapultWars {

	class AI final: public CatapultWars::Player
	{
	public:
		AI();

		virtual void Initialize(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) override;
		virtual void Update(double elapsedSeconds) override;
		virtual void Draw() override;
	};
}