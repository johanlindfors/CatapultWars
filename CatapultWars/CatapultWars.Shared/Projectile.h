#pragma once

#include "Animation.h"

namespace CatapultWars {

	enum ProjectileState {
		InFlight,
		HitGround,
		Destroyed
	};

	class Projectile
	{
	public:
		Projectile(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, vector<Projectile*>& activeProjectiles, String^ textureName, DirectX::SimpleMath::Vector2 startPosition, float groundHitOffset, bool isRightPlayer, float gravity);
		concurrency::task<void> Initialize(ID3D11Device* device);
		void Draw();
		void Fire(float velocityX, float velocityY);
		void Update(double elapsedSeconds);
		void UpdateProjectileFlightData(double elapsedSeconds, float wind, float gravity);
		void UpdateProjectileHit(double elapsedSeconds);
		void UpdateProjectileFlight(double elapsedSeconds);

		DirectX::SimpleMath::Vector2						ProjectilePosition;
		DirectX::SimpleMath::Vector2						ProjectileStartPosition;
		DirectX::SimpleMath::Vector2						ProjectileHitPosition;
		DirectX::SimpleMath::Vector2						CurrentVelocity;
		ProjectileState										State;
		float												ProjectileTextureWidth;
		float												ProjectileTextureHeight;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	ProjectileTexture;
		bool												HitHandled;
		float												Wind;
		Animation^											HitAnimation;

	private:
		Platform::String^						m_textureName;
		DirectX::SimpleMath::Vector2			m_projectileVelocity;
		DirectX::SimpleMath::Vector2			m_projectileInitialVelocity;
		DirectX::SimpleMath::Vector2			m_projectileRotationPosition;
		float									m_projectileRotation;
		float									m_flightTime;
		bool									m_isRightPlayer;
		float									m_hitOffset;
		float									m_gravity;
		std::shared_ptr<DirectX::SpriteBatch>	m_spriteBatch;
		UINT									m_textureWidth;
		UINT									m_textureHeight;
		std::vector<Projectile*>				m_activeProjectiles;
	};
}

