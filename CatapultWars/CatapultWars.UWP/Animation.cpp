#include "pch.h"
#include "Animation.h"

using namespace CatapultWars;

Animation::Animation(ComPtr<ID3D11ShaderResourceView> frameSheet, POINT size, POINT frameSheetSize)
{
	m_animatedCharacter = frameSheet;
	FrameSize = size;
	m_sheetSize = frameSheetSize;
	Offset = Vector2(0, 0);

	FrameCount = m_sheetSize.x * m_sheetSize.y;
}

Animation::~Animation()
{
	if (m_animatedCharacter != nullptr)
	{
		auto toDelete = m_animatedCharacter.ReleaseAndGetAddressOf();
		delete(toDelete);
	}
}

void Animation::PlayFromFrameIndex(int frameIndex)
{
	FrameIndex = frameIndex;
	IsActive = true;
}

void Animation::Update()
{
	if (IsActive)
	{
		if (FrameIndex >= FrameCount - 1)
		{
			IsActive = false;
			FrameIndex = FrameCount - 1; // Stop at last frame;
		}
		else
		{
			// Remember that updating currentFrame will also
			// update the FrameIndex property

			m_currentFrame.x++;
			if (m_currentFrame.x >= m_sheetSize.x)
			{
				m_currentFrame.x = 0;
				m_currentFrame.y++;
			}
			if (m_currentFrame.y >= m_sheetSize.y)
				m_currentFrame.y = 0;
		}
	}
}

void Animation::Draw(shared_ptr<SpriteBatch> spriteBatch, Vector2 position, SpriteEffects spriteEffects)
{
	Draw(spriteBatch, position, Vector2(1.0f,1.0f), spriteEffects);
}

void Animation::Draw(shared_ptr<SpriteBatch> spriteBatch, Vector2 position, Vector2 scale, SpriteEffects spriteEffects)
{
	int left = FrameSize.x * m_currentFrame.x;
	int top = FrameSize.y * m_currentFrame.y;
	int right = left + FrameSize.x;
	int bottom = top + FrameSize.y;

	RECT sourceRectangle = { left, top, right, bottom };
	spriteBatch->Draw(
		m_animatedCharacter.Get(),
		position + Offset,
		&sourceRectangle,
		Colors::White, 0.0f, Vector2(0, 0), scale, spriteEffects, 0.0f);
}
