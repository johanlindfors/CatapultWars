#pragma once

namespace CatapultWars {

	enum class CrateState {
		Idle = 0x0,
		Hit = 0x1
	};

	class SupplyCrate {

	public: 
		bool AnimationRunning;
		bool IsDestroyed;
		CrateState CurrentState;
		DirectX::SimpleMath::Vector2 Position;
		int Width;
		int Height;

		SupplyCrate(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, std::wstring idleTextureName, DirectX::SimpleMath::Vector2 catapultCenterPosition, bool isAI);
		concurrency::task<void> Initialize(ID3D11Device* device);
		void Update(double elapsedSeconds);
		void Draw();
		void Hit();

		//Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>		Texture;
		
	private:
		concurrency::task<void>	ParseXmlAndCreateAnimations(ID3D11Device* device);

		bool													m_animationRunning;
		bool													m_isDestroyed;
		CrateState												m_lastUpdateState;
		CrateState												m_currentState;
		const int												m_positionXOffset = -100;
		const int												m_positionYOffset = 30;
		DirectX::SimpleMath::Vector2							m_catapultCenter;
		bool													m_isAI;
		std::unordered_map<std::wstring, std::shared_ptr<Animation>>		m_animations;
		std::wstring							m_idleTextureName;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>						m_idleTexture;
		std::shared_ptr<DirectX::SpriteBatch>									m_spriteBatch;
		DirectX::SpriteEffects											m_spriteEffects;
		UINT													m_textureWidth;
		UINT													m_textureHeight;
	};
}