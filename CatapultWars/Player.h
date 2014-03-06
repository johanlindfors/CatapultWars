#pragma once

namespace CatapultWars {

	ref class Player abstract
	{

	internal:
		Player();
		virtual void Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch);
		virtual void Update(float timeTotal, float timeDelta);
		virtual void Draw();

		property CatapultWars::Catapult^ Catapult
		{
			CatapultWars::Catapult^ get() { return m_catapult; }
			void set(CatapultWars::Catapult^ catapult) { m_catapult = catapult; }
		}
		const float		MinShotStrength;
		const float		MaxShotStrength;

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


	private:
		CatapultWars::Catapult^			m_catapult;
		CatapultWars::Player^			m_enemy;
		Platform::String^				m_name;
		int								m_score;
		bool							m_isActive;
		std::shared_ptr<SpriteBatch>	m_spriteBatch;
	};
}