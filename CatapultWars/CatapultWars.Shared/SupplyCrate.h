#pragma once

namespace CatapultWars {

	enum class CrateState {
		Idle = 0x0,
		Hit = 0x1
	};

	ref class SupplyCrate {

	public: 
		property bool AnimationRunning;
		property bool IsDestroyed;

	internal:
		SupplyCrate(std::shared_ptr<SpriteBatch>& spriteBatch, String^ idleTextureName, Vector2 catapultCenterPosition, bool isAI);
		concurrency::task<void> Initialize(ID3D11Device* device);
		void Update(double elapsedSeconds);
		void Draw();
		void Hit();

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	Texture;

		property CrateState CurrentState {
			CrateState get() { return m_currentState; }
			void set(CrateState state) { m_currentState = state; }
		}

		property Vector2 Position;

		property int Width {
			int get() { return (int)m_textureWidth; }
		}

		property int Height {
			int get() { return (int)m_textureHeight; }
		}

	private:
		concurrency::task<void>	ParseXmlAndCreateAnimations(ID3D11Device* device);

		bool													m_animationRunning;
		bool													m_isDestroyed;
		CrateState												m_lastUpdateState;
		CrateState												m_currentState;
		const int												m_positionXOffset = -100;
		const int												m_positionYOffset = 30;
		String^													m_textureName;
		Vector2													m_catapultCenter;
		bool													m_isAI;
		std::unordered_map<Platform::String^, Animation^>		m_animations;
		String^													m_idleTextureName;
		ComPtr<ID3D11ShaderResourceView>						m_idleTexture;
		shared_ptr<SpriteBatch>									m_spriteBatch;
		SpriteEffects											m_spriteEffects;
		UINT													m_textureWidth;
		UINT													m_textureHeight;
	};
}