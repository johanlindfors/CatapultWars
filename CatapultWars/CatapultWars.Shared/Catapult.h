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
		void Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager);
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
		void ParseXmlAndCreateAnimations(ID3D11Device* device);
		void StartFiringFromLastAimPosition();
		void DrawIdleCatapult();
		void Hit();
		bool CheckHit();
		void CreateAnimation(ID3D11Device* device, Platform::String^ key, Platform::String^ textureFilename, int frameWidth, int frameHeight, int sheetColumns, int sheetRows, int splitFrame, int offsetX, int offsetY);

		shared_ptr<SpriteBatch>							m_spriteBatch;
		shared_ptr<AudioManager>						m_audioManager;
	};

	//static String^ xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?><a1></a1>";
	static String^ xml = "<?xml version=\"1.0\" encoding=\"utf-8\" ?><Animations><!--Player animation definitions--><Definition FrameWidth=\"75\" FrameHeight=\"60\" SheetRows=\"2\" SheetColumns=\"15\" Speed=\"30\" SplitFrame=\"20\" SheetName=\"Textures/Catapults/Blue/blueFire/blueCatapult_fire\" Alias=\"Fire\" IsAI=\"false\"/><Definition FrameWidth=\"75\" FrameHeight=\"60\" SheetRows=\"1\" SheetColumns=\"18\" Speed=\"30\" SheetName=\"Textures/Catapults/Blue/bluePullback/blueCatapult_Pullback\" Alias=\"Aim\" IsAI=\"false\"/><Definition FrameWidth=\"122\" FrameHeight=\"62\" SheetRows=\"2\" SheetColumns=\"15\" Speed=\"30\" OffsetX=\"-40\" OffsetY=\"0\" SheetName=\"Textures/Catapults/Blue/blueDestroyed/blueCatapult_destroyed\" Alias=\"Destroyed\" IsAI=\"false\"/><Definition FrameWidth=\"90\" FrameHeight=\"80\" SheetRows=\"2\" SheetColumns=\"15\" Speed=\"30\" OffsetX=\"-50\" OffsetY=\"0\" SheetName=\"Textures/Catapults/Fire_Miss/fire_miss\" Alias=\"fireMiss\" IsAI=\"false\"/><Definition FrameWidth=\"128\" FrameHeight=\"128\" SheetRows=\"2\" SheetColumns=\"15\" Speed=\"30\" OffsetX=\"-64\" OffsetY=\"-64\" SheetName=\"Textures/Catapults/Hit_Smoke/smoke\" Alias=\"hitSmoke\" IsAI=\"false\"/><!--AI animation definitions--><Definition FrameWidth=\"75\" FrameHeight=\"60\" SheetRows=\"2\" SheetColumns=\"15\" Speed=\"30\" SplitFrame=\"20\" SheetName=\"Textures/Catapults/Red/redFire/redCatapult_fire\" Alias=\"Fire\" IsAI=\"true\"/><Definition FrameWidth=\"122\" FrameHeight=\"62\" SheetRows=\"2\" SheetColumns=\"15\" Speed=\"30\" OffsetX=\"-11\" OffsetY=\"0\" SheetName=\"Textures/Catapults/Red/redDestroyed/redCatapult_destroyed\" Alias=\"Destroyed\" IsAI=\"true\"/><Definition FrameWidth=\"75\" FrameHeight=\"60\" SheetRows=\"1\" SheetColumns=\"18\" Speed=\"30\" SheetName=\"Textures/Catapults/Red/redPullback/redCatapult_Pullback\" Alias=\"Aim\" IsAI=\"true\"/><Definition FrameWidth=\"90\" FrameHeight=\"80\" SheetRows=\"2\" SheetColumns=\"15\" Speed=\"30\" OffsetX=\"-50\" OffsetY=\"0\" SheetName=\"Textures/Catapults/Fire_Miss/fire_miss\" Alias=\"fireMiss\" IsAI=\"true\"/><Definition FrameWidth=\"128\" FrameHeight=\"128\" SheetRows=\"2\" SheetColumns=\"15\" Speed=\"30\" OffsetX=\"-30\" OffsetY=\"-64\" SheetName=\"Textures/Catapults/Hit_Smoke/smoke\" Alias=\"hitSmoke\" IsAI=\"true\"/></Animations>";
}