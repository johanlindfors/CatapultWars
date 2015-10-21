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
		SupplyCrate();
		void Initialize();
		
		property CrateState CurrentState {
			CrateState get() { return m_currentState; }
			void set(CrateState state) { m_currentState = state; }
		}

		property Vector2 Position;

	private:
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
		ComPtr<ID3D11ShaderResourceView>						m_idleTexture;
		shared_ptr<SpriteBatch>									m_spriteBatch;
	};
}