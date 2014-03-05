#pragma once
#include "Player.h"

namespace CatapultGame {
	class AI : Player
	{
	public:
		AI();
		~AI();

		virtual void Initialize() override;
		virtual void Update(float timeTotal, float timeDelta) override;
		virtual void Draw() override;
	};
}