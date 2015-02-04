#pragma once
#include "Player.h"

namespace CatapultWars {

	ref class AI sealed : public Player
	{
	public:
		AI();

	internal:
		virtual void Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch) override;
		virtual void Update() override;
		virtual void Draw() override;
	};
}