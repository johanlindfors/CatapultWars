#pragma once
//#include "property.hpp"

using namespace std;
using namespace Platform;
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace DirectX::SimpleMath;

namespace CatapultWars {
	class Animation final
	{
	public:
		Animation(ComPtr<ID3D11ShaderResourceView> frameSheet, POINT size, POINT frameSheetSize);
		~Animation();

		void PlayFromFrameIndex(int index);
		void Update();
		void Draw(shared_ptr<SpriteBatch> spriteBatch, Vector2 position, SpriteEffects spriteEffects);
		void Draw(shared_ptr<SpriteBatch> spriteBatch, Vector2 position, Vector2 scale, SpriteEffects spriteEffects);
		
		void SetFrameIndex(int frameIndex) {
			if (frameIndex >= m_sheetSize.x*m_sheetSize.y + 1) {
					//throw new OutOfBoundsException(L"FrameIndex is out of bounds");
			}
			m_currentFrame.y = frameIndex / m_sheetSize.x;
			m_currentFrame.x = frameIndex % (int)m_sheetSize.x;
		}
		
		int GetFrameIndex() {
			return m_sheetSize.x * m_currentFrame.y + m_currentFrame.x;
		}
		
		POINT FrameSize;
		Vector2 Offset;
		int FrameCount;
		bool IsActive;

	private:
		POINT m_sheetSize;
		POINT m_currentFrame;
		ComPtr<ID3D11ShaderResourceView> m_animatedCharacter;
	};
}

