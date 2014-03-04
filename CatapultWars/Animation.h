#pragma once
#include "SpriteBatch.h"

using namespace std;
using namespace DirectX;

namespace CatapultGame {
	class Animation sealed
	{
	public:
		Animation();
		int FrameIndex;
		int FrameCount;

		void PlayFromFrameIndex(int index);
		void Draw(shared_ptr<SpriteBatch> spriteBatch, XMFLOAT2 position, SpriteEffects spriteEffects);
	};
}

