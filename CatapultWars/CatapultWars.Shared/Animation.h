#pragma once
#include <SimpleMath.h>

namespace CatapultWars {
	class Animation final
	{
	public:
		Animation(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> frameSheet, POINT size, POINT frameSheetSize);
		~Animation();

		void PlayFromFrameIndex(int index);
		void Update();
		void Draw(std::shared_ptr<DirectX::SpriteBatch> spriteBatch, DirectX::SimpleMath::Vector2 position, DirectX::SpriteEffects spriteEffects);
		void Draw(std::shared_ptr<DirectX::SpriteBatch> spriteBatch, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale, DirectX::SpriteEffects spriteEffects);

		int SetFrameIndex(int frameIndex) {
			if (frameIndex >= m_sheetSize.x*m_sheetSize.y + 1) {
				throw ref new Platform::OutOfBoundsException(L"FrameIndex is out of bounds");
			}
			m_currentFrame.y = frameIndex / m_sheetSize.x;
			m_currentFrame.x = frameIndex % (int)m_sheetSize.x;
		}

		int GetFrameIndex() {
			return m_sheetSize.x * m_currentFrame.y + m_currentFrame.x;
		}

		POINT FrameSize;
		DirectX::SimpleMath::Vector2 Offset;
		int FrameCount;
		bool IsActive;

	private:
		POINT m_sheetSize;
		POINT m_currentFrame;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_animatedCharacter;
	};
}

