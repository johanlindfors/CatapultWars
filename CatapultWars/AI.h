#pragma once
#include "Player.h"

namespace CatapultWars {

	ref class AI sealed : public Player
	{
	public:
		AI();

	internal:
		virtual void Initialize(ID3D11Device* device) override;
		virtual void Update(float timeTotal, float timeDelta) override;
		virtual void Draw() override;
	};
}