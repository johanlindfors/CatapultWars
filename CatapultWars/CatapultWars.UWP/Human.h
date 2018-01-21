#pragma once
#include "Player.h"

namespace CatapultWars {

	ref class Human sealed : public Player
	{
	public:
		Human();
		
	internal:
		bool IsDragging;
		virtual void Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch) override;
		virtual void Update(double elapsedSeconds) override;
		virtual void Draw() override;
		void HandleInput(int x, int y);
		void HandleRelease();

	private:
		const float							m_maxDragDelta;
		ComPtr<ID3D11ShaderResourceView>	m_arrow;
		float								m_arrowScale;
		Vector2								m_catapultPosition;
		std::shared_ptr<SpriteBatch>		m_spriteBatch;
		Vector2								m_delta;

		void DrawDragArrow(float arrowScale);
		void ResetDragState();
	};

}

