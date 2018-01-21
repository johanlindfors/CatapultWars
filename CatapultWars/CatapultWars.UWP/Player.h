#pragma once

namespace CatapultWars {

	class Player
	{

	public:
		Player()
			: m_score(0)
			, m_isActive(true)
		{}

		virtual void Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch);
		virtual void Update(double elapsedSeconds);
		virtual void Draw();

		std::shared_ptr<CatapultWars::Catapult> GetCatapult() { return m_catapult; }
		void SetCatapult(std::shared_ptr<CatapultWars::Catapult> catapult) { m_catapult = catapult; }
		
		const float		MinShotStrength = 150;
		const float		MaxShotStrength = 400;

		void SetEnemy(std::shared_ptr<Player> enemy) 
		{
			m_catapult->SetEnemy(enemy);
			m_catapult->SetSelf(this);
		}

		wchar_t* GetName() {
			return m_name;
		}

		void SetName(wchar_t* name) {
			m_name = name;
		}

		int GetScore() {
			return m_score;
		}

		void SetScore(int score) {
			m_score = score;
		}

		int GetIsActive() {
			return m_isActive;
		}

		void SetIsActive(int isActive) {
			m_isActive = isActive;
		}

	private:
		std::shared_ptr<CatapultWars::Catapult>		m_catapult;
		std::shared_ptr<CatapultWars::Player>		m_enemy;
		wchar_t*						m_name;
		int								m_score;
		bool							m_isActive;
		std::shared_ptr<SpriteBatch>	m_spriteBatch;
	};
}