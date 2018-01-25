#pragma once

namespace CatapultWars {

	class Player
	{

	public:
		Player();
		virtual void Initialize(ID3D11Device* device, std::shared_ptr<DirectX::SpriteBatch>& spriteBatch, std::shared_ptr<AudioManager>& audioManager);
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


	private:
		std::shared_ptr<CatapultWars::Catapult>			m_catapult;
		std::shared_ptr<CatapultWars::Player>			m_enemy;
		Platform::String^								m_name;
		int												m_score;
		bool											m_isActive;
		std::shared_ptr<DirectX::SpriteBatch>			m_spriteBatch;
	};
}