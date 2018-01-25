#pragma once
#include "Player.h"

namespace CatapultWars {

	class Human final : public Player
	{
	public:
		Human();
		
		bool IsDragging;

		virtual void Initialize(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch) override;
		virtual void Update(double elapsedSeconds) override;
		virtual void Draw() override;
		void HandleInput(int x, int y);
		void HandleRelease();

	private:
		const float											m_maxDragDelta;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_arrow;
		float												m_arrowScale;
		DirectX::SimpleMath::Vector2						m_catapultPosition;
		std::shared_ptr<DirectX::SpriteBatch>				m_spriteBatch;
		DirectX::SimpleMath::Vector2						m_delta;

		void DrawDragArrow(float arrowScale);
		void ResetDragState();
	};

}

