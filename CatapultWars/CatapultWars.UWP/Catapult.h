#pragma once

#include <Projectile.h>
#include <Animation.h>
#include <unordered_map>

namespace CatapultWars {

	enum CatapultState {
		Idle = 0x0,
		Aiming = 0x1,
		Firing = 0x2,
		ProjectileFlying = 0x4,
		ProjectileHit = 0x8,
		Hit = 0x10,
		Reset = 0x20,
		Stalling = 0x40
	};

	class Animation;
	class Player;

	class Catapult final
	{
	public:
		Catapult(const wchar_t* idleTexture, Vector2 position, SpriteEffects spriteEffect, bool isAi);
		void Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch);
		void Update(double elapsedSeconds);
		void Draw();
		void Fire(float velocity);

		bool				AnimationRunning;
		wchar_t*			Name;
		bool				IsActive;
		CatapultWars::CatapultState		CurrentState;
		float				ShotStrength;
		float				ShotVelocity;
		bool				GameOver;

		Vector2 GetPosition() { return m_catapultPosition; }
		
		void SetWind(float wind) { m_wind = wind; }	
		
		void SetSelf(CatapultWars::Player* self) { m_self.reset(self); }

		void SetEnemy(std::shared_ptr<CatapultWars::Player> enemy) { m_enemy = enemy; }	

	private:
		ComPtr<ID3D11ShaderResourceView>				m_idleTexture;		
		SpriteEffects									m_spriteEffects;
		std::shared_ptr<CatapultWars::Projectile>		m_projectile;
		const wchar_t*									m_idleTextureName;
		bool											m_isAI;
		const float										m_gravity;
		CatapultWars::CatapultState						m_lastUpdateState;
		int												m_stallUpdateCycles;
		//CatapultState									m_currentState;
		float											m_wind;
		std::shared_ptr<CatapultWars::Player>			m_enemy;
		std::shared_ptr<CatapultWars::Player>			m_self;
		Vector2											m_catapultPosition;
		const int										m_winScore;

		std::unordered_map<std::wstring, std::shared_ptr<CatapultWars::Animation>>		m_animations;
		std::unordered_map<std::wstring, int>				m_splitFrames;

		bool AimReachedShotStrength();
		void UpdateAimAccordingToShotStrength();
		void ParseXmlAndCreateAnimations(ID3D11Device* device);
		void StartFiringFromLastAimPosition();
		void DrawIdleCatapult();
		void Hit();
		bool CheckHit();
		void CreateAnimation(ID3D11Device* device, std::wstring key, std::wstring textureFilename, int frameWidth, int frameHeight, int sheetColumns, int sheetRows, int splitFrame, int offsetX, int offsetY);

		std::shared_ptr<DirectX::SpriteBatch>							m_spriteBatch;
	};

}