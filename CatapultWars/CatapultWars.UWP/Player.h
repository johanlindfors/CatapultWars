#pragma once

namespace CatapultWars {

	class Player
	{

	public:
		//virtual void Initialize(ID3D11Device* device, std::shared_ptr<SpriteBatch>& spriteBatch) = 0;
		virtual void Update(double elapsedSeconds) = 0;
		virtual void Draw() = 0;

		std::shared_ptr<CatapultWars::Catapult> GetCatapult() { return m_catapult; }
		void SetCatapult(std::shared_ptr < CatapultWars::Catapult> catapult) { m_catapult = catapult; }
		
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
		//property Platform::String^ Name {
		//	Platform::String^ get() { return m_name; }
		//	void set(Platform::String^ name) { m_name = name; }
		//}

		//property int Score {
		//	int get() { return m_score; }
		//	void set(int score) { m_score = score; }
		//}

		//property bool IsActive {
		//	bool get() { return m_isActive; }
		//	void set(bool isActive) { m_isActive = isActive; }
		//}


	private:
		Player() 
			: m_score(0)
			, m_isActive(true)
		{}

		std::shared_ptr<CatapultWars::Catapult>		m_catapult;
		std::shared_ptr<CatapultWars::Player>		m_enemy;
		Platform::String^				m_name;
		int								m_score;
		bool							m_isActive;
		std::shared_ptr<SpriteBatch>	m_spriteBatch;
	};
}