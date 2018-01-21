#pragma once
#include "property.hpp"

using namespace std;
using namespace Platform;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

namespace CatapultWars {
	ref class Animation sealed
	{
	internal:
		Animation(ComPtr<ID3D11ShaderResourceView> frameSheet, POINT size, POINT frameSheetSize);
		
		void PlayFromFrameIndex(int index);
		void Update();
		void Draw(shared_ptr<SpriteBatch> spriteBatch, Vector2 position, SpriteEffects spriteEffects);
		void Draw(shared_ptr<SpriteBatch> spriteBatch, Vector2 position, Vector2 scale, SpriteEffects spriteEffects);
		
		property int FrameIndex {
			void set(int frameIndex) {
				if (frameIndex >= m_sheetSize.x*m_sheetSize.y + 1) {
					 throw ref new OutOfBoundsException(L"FrameIndex is out of bounds");
				}
				m_currentFrame.y = frameIndex / m_sheetSize.x;
				m_currentFrame.x = frameIndex % (int)m_sheetSize.x;
			}
			int get() {
				return m_sheetSize.x * m_currentFrame.y + m_currentFrame.x;
			}
		}
	internal:
		POINT FrameSize;
		Vector2 Offset;
		int FrameCount;
		bool IsActive;

	private:
		~Animation();
		POINT m_sheetSize;
		POINT m_currentFrame;
		ComPtr<ID3D11ShaderResourceView> m_animatedCharacter;
	};
}

