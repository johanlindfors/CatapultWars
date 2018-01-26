#pragma once
#include "Player.h"
#include "AudioManager.h"

namespace CatapultWars {

	enum PlayerSide {
		Left,
		Right
	};

	class Human final : public Player
	{
	public:
		Human(PlayerSide playerSide);
		
	public:
		bool IsDragging;
		virtual concurrency::task<void> Initialize(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager) override;

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
		DirectX::SpriteEffects								m_spriteEffects;
		PlayerSide											m_playerSide;

		void DrawDragArrow(float arrowScale);
		void ResetDragState();
	};

}

