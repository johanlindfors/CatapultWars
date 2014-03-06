#pragma once

#include "Projectile.h"
#include "Animation.h"
#include <unordered_map>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

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
	ref class Player;

	ref class Catapult sealed
	{
	internal:
		Catapult(wchar_t* idleTexture, Vector2 position, SpriteEffects spriteEffect, bool isAi);
		void Initialize(ID3D11Device* device);
		void Update(float timeTotal, float timeDelta);
		void Draw();

		bool			AnimationRunning;
		wchar_t*		Name;
		bool			IsActive;
		CatapultState	CurrentState;
		float			ShotStrength;
		float			ShotVelocity;
		bool			GameOver;

		property Vector2 Position {
			Vector2 get() { return m_catapultPosition; }
		}
		
		property float Wind
		{
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
		wchar_t*										m_idleTextureName;
		bool											m_isAI;
		const float										m_gravity;
		CatapultState									m_lastUpdateState;
		int												m_stallUpdateCycles;
		CatapultState									m_currentState;
		float											m_wind;
		Player^											m_enemy;
		Player^											m_self;
		Vector2											m_catapultPosition;
		const int										m_winScore;

		std::unordered_map<wchar_t*, Animation*>		m_animations;
		std::unordered_map<wchar_t*, int>				m_splitFrames;

		bool AimReachedShotStrength();
		void UpdateAimAccordingToShotStrength();
		void ParseXmlAndCreateAnimations();
		void StartFiringFromLastAimPosition();
		void DrawIdleCatapult();
		void Hit();
		void Fire(float velocity);
		bool CheckHit();

		shared_ptr<SpriteBatch>							m_spriteBatch;
	};

}