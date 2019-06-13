#pragma once

#include <Projectile.h>
#include <Animation.h>
#include <SupplyCrate.h>
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

	class Animation;
	class Player;

	class Catapult final
	{
	public:
		Catapult(std::wstring idleTexture, DirectX::SimpleMath::Vector2 position, DirectX::SpriteEffects spriteEffect, bool isLeftSide, bool isHuman);
		concurrency::task<void> Initialize(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager);
		void Update(double elapsedSeconds);
		void Draw();
		void Fire(float velocity, float angle);

		bool							AnimationRunning;
		Platform::String^				Name;
		bool							IsActive;
		CatapultState					CurrentState;
		bool							GameOver;
		DirectX::SimpleMath::Vector2	ProjectileStartPosition;

		DirectX::SimpleMath::Vector2 GetPosition() { return m_catapultPosition; }	
		
		void SetWind(float wind) { m_wind = wind; }
		
		void SetSelf(Player* self) { m_self = self; }		
		
		void SetEnemy(Player* enemy) { m_enemy = enemy; }

		float ShotStrength;
		float ShotAngle;
		float ShotVelocity;

		std::shared_ptr<SupplyCrate> GetCrate() { return m_crate; }	

	private:

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	m_idleTexture;		
		DirectX::SpriteEffects								m_spriteEffects;
		std::shared_ptr<Projectile>							m_normalProjectile;
		std::shared_ptr<Projectile>							m_splitProjectile;
		std::shared_ptr<SupplyCrate>						m_crate;
		std::wstring										m_idleTextureName;
		bool												m_isHuman;
		const float											m_gravity;
		CatapultState										m_lastUpdateState;
		int													m_stallUpdateCycles;
		float												m_wind;
		Player*												m_enemy;
		Player*												m_self;
		DirectX::SimpleMath::Vector2						m_catapultPosition;
		const int											m_winScore;
		const int											m_MaxActiveProjectiles = 3;
		float												m_shotStrength;
		float												m_shotVelocity;
		float												m_shotAngle;
		bool												m_isLeftSide;

		std::unordered_map<std::wstring, std::shared_ptr<Animation>>	m_animations;
		std::unordered_map<std::wstring, int>							m_splitFrames;
		std::vector<std::shared_ptr<Projectile>>						m_activeProjectiles;
		std::vector<std::shared_ptr<Projectile>>						m_activeProjectilesCopy;
		std::vector<std::shared_ptr<Projectile>>						m_destroyedProjectiles;

		bool AimReachedShotStrength();
		void UpdateAimAccordingToShotStrength();
		//concurrency::task<void> ParseXmlAndCreateAnimations(ID3D11Device* device);
		void StartFiringFromLastAimPosition();
		void DrawIdleCatapult();

		void Hit(bool isKilled);
		HitCheckResult Catapult::CheckHit(std::shared_ptr<Projectile> projectile);
		void CreateAnimation(ID3D11Device* device, std::wstring key, std::wstring , int frameWidth, int frameHeight, int sheetColumns, int sheetRows, int splitFrame, int offsetX, int offsetY);

		void HandleProjectileHit(std::shared_ptr<Projectile> projectile);
		void PerformNothingHit(std::shared_ptr<Projectile> projectile);
		void UpdateHealth(Player* player, DirectX::BoundingSphere projectile, DirectX::BoundingBox catapult);

		std::shared_ptr<DirectX::SpriteBatch>				m_spriteBatch;
		std::shared_ptr<AudioManager>						m_audioManager;

		bool												m_isInitialized;
	};
}