#pragma once

#include <Projectile.h>
#include <Animation.h>
#include <AudioManager.h>
#include <unordered_map>
#include <ppltasks.h>

namespace CatapultWars {

	enum CatapultState {
		Idle = 0x0,
		Aiming = 0x1,
		Firing = 0x2,
		ProjectileFlying = 0x4,
		ProjectileHit = 0x8,
		HitKill = 0x10,
		HitDamage = 0x20,
		Reset = 0x40,
		Stalling = 0x80,
		ProjectilesFalling = 0x100
	};

	enum HitCheckResult {
		Nothing = 0x0,
		SelfCatapult = 0x1,
		EnemyCatapult = 0x2,
		SelfCrate = 0x4,
		EnemyCrate = 0x8
	};

	ref class Animation;
	ref class Player;

	ref class Catapult sealed
	{
	internal:
		Catapult(Platform::String^ idleTexture, Vector2 position, SpriteEffects spriteEffect, bool isHuman, bool isLeftSide);
		concurrency::task<void> Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager);
		void Update(double elapsedSeconds);
		void Draw();
		void Fire(float velocity);

		bool				AnimationRunning;
		Platform::String^	Name;
		bool				IsActive;
		CatapultState		CurrentState;
		bool				GameOver;
		Vector2				ProjectileStartPosition;

		property Vector2 Position {
			Vector2 get() { return m_catapultPosition; }
		}
		
		property float Wind {
			void set(float wind) { m_wind = wind; }
		}
		
		property Player^ Self{
			void set(Player^ self) { m_self = self; }
		}
		
		property Player^ Enemy {
			void set(Player^ enemy) { m_enemy = enemy; }
		}

		property float ShotStrength {
			void set(float value) { m_shotStrength = value; }
			float get() { return m_shotStrength; }
		}

		property float ShotVelocity {
			void set(float value) { m_shotVelocity = value; }
			float get() { return m_shotVelocity; }
		}

	private:
		ComPtr<ID3D11ShaderResourceView>				m_idleTexture;		
		SpriteEffects									m_spriteEffects;
		Projectile*										m_normalProjectile;
		Projectile*										m_splitProjectile;
		Platform::String^								m_idleTextureName;
		bool											m_isHuman;
		const float										m_gravity;
		CatapultState									m_lastUpdateState;
		int												m_stallUpdateCycles;
		//CatapultState									m_currentState;
		float											m_wind;
		Player^											m_enemy;
		Player^											m_self;
		Vector2											m_catapultPosition;
		const int										m_winScore;
		const int										m_MaxActiveProjectiles = 3;
		float											m_shotStrength;
		float											m_shotVelocity;
		bool											m_isLeftSide;

		std::unordered_map<Platform::String^, Animation^>		m_animations;
		std::unordered_map<Platform::String^, int>				m_splitFrames;
		std::vector<Platform::String^>							m_activeProjectiles;
		std::vector<Platform::String^>							m_activeProjectilesCopy;
		std::vector<Platform::String^>							m_destroyedProjectiles;

		bool AimReachedShotStrength();
		void UpdateAimAccordingToShotStrength();
		concurrency::task<void> ParseXmlAndCreateAnimations(ID3D11Device* device);
		void StartFiringFromLastAimPosition();
		void DrawIdleCatapult();
		void Hit();
		bool CheckHit();
		void CreateAnimation(ID3D11Device* device, Platform::String^ key, Platform::String^ textureFilename, int frameWidth, int frameHeight, int sheetColumns, int sheetRows, int splitFrame, int offsetX, int offsetY);

		shared_ptr<SpriteBatch>							m_spriteBatch;
		shared_ptr<AudioManager>						m_audioManager;

		bool											m_isInitialized;
	};
}