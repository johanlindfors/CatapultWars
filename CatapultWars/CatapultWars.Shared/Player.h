#pragma once

namespace CatapultWars {

	enum WeaponType {
		Normal,
		Split
	};

	class Player
	{

	public:
		Player();
		virtual concurrency::task<void> Initialize(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager) = 0;
		virtual void Update(double elapsedSeconds);
		virtual void Draw();

		std::shared_ptr<CatapultWars::Catapult> Catapult;

		const float		MinShotStrength;
		const float		MaxShotStrength;

		void SetEnemy(Player* enemy) {
			this->Catapult->SetEnemy(enemy);
			this->Catapult->SetSelf(this);
		}

		std::wstring Name;
		int Score;
		bool IsActive;
		int Health;
		WeaponType Weapon;

		const float MinShotVelocity = 200.0;
		const float MaxShotVelocity = 665.0;
		const float MinShotAngle = 0; // 0 degrees
		const float MaxShotAngle = 1.3962634f; // 80 degrees

	private:
		std::shared_ptr<CatapultWars::Catapult>			m_catapult;
		std::shared_ptr<CatapultWars::Player>			m_enemy;
		Platform::String^								m_name;
		int												m_score;
		bool											m_isActive;
		std::shared_ptr<DirectX::SpriteBatch>			m_spriteBatch;
	};
}