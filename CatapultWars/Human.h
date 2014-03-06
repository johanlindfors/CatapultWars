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
		virtual void Update(float timeTotal, float timeDelta) override;
		virtual void Draw() override;
		void HandleInput();

	private:
		const float							m_maxDragDelta;
		ComPtr<ID3D11ShaderResourceView>	m_arrow;
		float								m_arrowScale;
		Vector2								m_catapultPosition;

		void DrawDragArrow(float arrowScale);
		void ResetDragState();
	};

}

