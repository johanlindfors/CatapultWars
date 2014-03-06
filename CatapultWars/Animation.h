#pragma once
#include "property.hpp"

using namespace std;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

namespace CatapultWars {
	class Animation sealed
	{
	public:
		Animation(ComPtr<ID3D11ShaderResourceView> frameSheet, POINT size, POINT frameSheetSize);
		~Animation();
		int FrameCount;
		POINT FrameSize;
		bool IsActive;
		Vector2 Offset;

		void PlayFromFrameIndex(int index);
		void Update();
		void Draw(shared_ptr<SpriteBatch> spriteBatch, Vector2 position, SpriteEffects spriteEffects);
		void Draw(shared_ptr<SpriteBatch> spriteBatch, Vector2 position, Vector2 scale, SpriteEffects spriteEffects);
		
		property<Animation, int, READ_WRITE> FrameIndex;
		void setFrameIndex(int frameIndex)
		{
			if (frameIndex >= m_sheetSize.x*m_sheetSize.y + 1)
			{
				// throw exception;
			}
			m_currentFrame.y = frameIndex / m_sheetSize.x;
			m_currentFrame.x = frameIndex % (int)m_sheetSize.x;
		}
		int getFrameIndex()
		{
			return m_sheetSize.x * m_currentFrame.y + m_currentFrame.x;
		}
	private:
		POINT m_sheetSize;
		POINT m_currentFrame;
		ComPtr<ID3D11ShaderResourceView> m_animatedCharacter;
	};
}

