#pragma once

#include "DirectXHelper.h"
#include "property.hpp"
#include "SpriteBatch.h"
#include "Projectile.h"
#include "Player.h"
#include "Animation.h"
#include "SimpleMath.h"
#include <unordered_map>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std;

namespace CatapultGame {

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

	class Catapult
	{
	public:
		Catapult(wchar_t* idleTexture, Vector2 position, SpriteEffects spriteEffect, bool isAi);
		~Catapult();

		void Initialize();
		void Update(float timeTotal, float timeDelta);
		void Draw();

		property<Catapult, CatapultState, READ_WRITE>	CurrentState;
		property<Catapult, bool, READ_WRITE>			AnimationRunning;
		property<Catapult, float, READ_WRITE>			ShotStrength;
		property<Catapult, float, READ_WRITE>			ShotVelocity;
		property<Catapult, wchar_t*, READ_WRITE>		Name;
		property<Catapult, bool, READ_WRITE>			IsActive;
		property<Catapult, float, WRITE_ONLY>			Wind;
		property<Catapult, Player*, WRITE_ONLY>			Enemy;
		property<Catapult, Player*, WRITE_ONLY>			Self;
		XMFLOAT2										Position;
		property<Catapult, bool, READ_WRITE>			GameOver;

		void setWind(float wind) { m_wind = wind; }
		void setEnemy(Player* enemy) { m_enemy = enemy; }
		void setSelf(Player* self) { m_self = self; }
		
		//XMFLOAT2 getPosition() { return m_catapultPosition; }

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
		Player*											m_enemy;
		Player*											m_self;
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