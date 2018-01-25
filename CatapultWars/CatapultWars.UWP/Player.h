#pragma once

namespace CatapultWars {

	class Player
	{

	public:
		Player()
			: Score(0)
			, IsActive(true)
		{}

		~Player() {

		}

		virtual void Initialize(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch);
		virtual void Update(double elapsedSeconds);
		virtual void Draw();

		std::shared_ptr<CatapultWars::Catapult> Catapult;
		
		const float		MinShotStrength = 150;
		const float		MaxShotStrength = 400;

		void SetEnemy(std::shared_ptr<CatapultWars::Player> enemy)
		{
			Catapult->SetEnemy(enemy);
			Catapult->SetSelf(this);
		}

		std::wstring Name;
		int Score;
		int IsActive;	

	private:
		std::shared_ptr<CatapultWars::Player>		m_enemy;
		std::shared_ptr<DirectX::SpriteBatch>		m_spriteBatch;
	};
}