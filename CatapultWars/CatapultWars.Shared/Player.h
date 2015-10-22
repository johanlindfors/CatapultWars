#pragma once

namespace CatapultWars {

	enum WeaponType {
		Normal,
		Split
	};

	ref class Player abstract
	{
	internal:
		Player();
		virtual concurrency::task<void> Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager) = 0;
		virtual void Update(double elapsedSeconds);
		virtual void Draw();

		property CatapultWars::Catapult^ Catapult
		{
			CatapultWars::Catapult^ get() { return m_catapult; }
			void set(CatapultWars::Catapult^ catapult) { m_catapult = catapult; }
		}

		property Player^ Enemy {
			void set(Player^ enemy) {
				this->Catapult->Enemy = enemy;
				this->Catapult->Self = this;
			}
		}

		property Platform::String^ Name {
			Platform::String^ get() { return m_name; }
			void set(Platform::String^ name) { m_name = name; }
		}

		property int Score {
			int get() { return m_score; }
			void set(int score) { m_score = score; }
		}

		property bool IsActive {
			bool get() { return m_isActive; }
			void set(bool isActive) { m_isActive = isActive; }
		}

		property int Health {
			int get() { return m_health; }
			void set(int health) { m_health = health; }
		}

		WeaponType Weapon;
		const float MinShotVelocity = 200.0;
		const float MaxShotVelocity = 665.0;
		const float MinShotAngle = 0; // 0 degrees
		const float MaxShotAngle = 1.3962634f; // 80 degrees

	private:
		CatapultWars::Catapult^			m_catapult;
		CatapultWars::Player^			m_enemy;
		Platform::String^				m_name;
		int								m_score;
		bool							m_isActive;
		std::shared_ptr<SpriteBatch>	m_spriteBatch;
		int								m_health;
	};
}