#pragma once
#include "Player.h"

namespace CatapultGame {
	class AI : Player
	{
	public:
		AI();
		~AI();

		virtual void Initialize(ID3D11Device* device) override;
		virtual void Update(float timeTotal, float timeDelta) override;
		virtual void Draw() override;
	};
}