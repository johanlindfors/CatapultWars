#pragma once

#include <Projectile.h>
#include <Animation.h>
#include <AudioManager.h>
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

	ref class Animation;
	ref class Player;

	ref class Catapult sealed
	{
	internal:
		Catapult(Platform::String^ idleTexture, Vector2 position, SpriteEffects spriteEffect, bool isAi);
		void Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager);
		void Update(double elapsedSeconds);
		void Draw();
		void Fire(float velocity);

		bool				AnimationRunning;
		Platform::String^	Name;
		bool				IsActive;
		CatapultState		CurrentState;
		float				ShotStrength;
		float				ShotVelocity;
		bool				GameOver;

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

	private:
		ComPtr<ID3D11ShaderResourceView>				m_idleTexture;		
		SpriteEffects									m_spriteEffects;
		Projectile*										m_projectile;
		Platform::String^								m_idleTextureName;
		bool											m_isAI;
		const float										m_gravity;
		CatapultState									m_lastUpdateState;
		int												m_stallUpdateCycles;
		//CatapultState									m_currentState;
		float											m_wind;
		Player^											m_enemy;
		Player^											m_self;
		Vector2											m_catapultPosition;
		const int										m_winScore;

		std::unordered_map<Platform::String^, Animation^>		m_animations;
		std::unordered_map<Platform::String^, int>				m_splitFrames;

		bool AimReachedShotStrength();
		void UpdateAimAccordingToShotStrength();
		void ParseXmlAndCreateAnimations(ID3D11Device* device);
		void StartFiringFromLastAimPosition();
		void DrawIdleCatapult();
		void Hit();
		bool CheckHit();
		void CreateAnimation(ID3D11Device* device, Platform::String^ key, Platform::String^ textureFilename, int frameWidth, int frameHeight, int sheetColumns, int sheetRows, int splitFrame, int offsetX, int offsetY);

		shared_ptr<SpriteBatch>							m_spriteBatch;
		shared_ptr<AudioManager>						m_audioManager;
	};

}