#pragma once

namespace CatapultWars {
	class Projectile
	{
	public:
		Projectile(std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, Platform::String^ textureName, DirectX::SimpleMath::Vector2 startPosition, float groundHitOffset, bool isAI, float gravity);
		void Initialize(ID3D11Device* device);
		void Draw();
		void Fire(float velocityX, float velocityY);
		bool UpdateProjectileFlightData(float wind, float gravity);

		DirectX::SimpleMath::Vector2						ProjectilePosition;
		DirectX::SimpleMath::Vector2						ProjectileStartPosition;
		DirectX::SimpleMath::Vector2						ProjectileHitPosition;
		float												ProjectileTextureWidth;
		float												ProjectileTextureHeight;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	ProjectileTexture;

	private:
		Platform::String^						m_textureName;
		DirectX::SimpleMath::Vector2			m_projectileVelocity;
		float									m_projectileInitialVelocityY;
		DirectX::SimpleMath::Vector2			m_projectileRotationPosition;
		float									m_projectileRotation;
		float									m_flightTime;
		bool									m_isAI;
		float									m_hitOffset;
		float									m_gravity;
		std::shared_ptr<DirectX::SpriteBatch>	m_spriteBatch;
		UINT									m_textureWidth;
		UINT									m_textureHeight;
	};
}

