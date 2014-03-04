#pragma once
#include "property.hpp"
#include "Catapult.h"

class Player
{
public:
	Player();
	~Player();

	void Initialize();

	Catapult*		Catapult;
	property<Player, Player*, WRITE_ONLY> Enemy;
	property<Player, wchar_t*, READ_WRITE> Name;
	property<Player, int, READ_WRITE> Score;
	property<Player, bool, READ_WRITE> IsActive;

	const float MinShotStrength;
	const float MaxShotStrength;

	void Update(float timeTotal, float timeDelta);
	void Draw();

private:
	Player*			m_enemy;
	void			setEnemy(Player* enemy) { m_enemy = enemy; }
	
	wchar_t*		m_name;
	wchar_t*		getName() { return m_name; }
	void			setName(wchar_t* name) { m_name = name; }

	int				m_score;
	int				getScore() { return m_score; }
	void			setScore(int score) { m_score = score; }

	bool			m_isActive;
	bool			getIsActive() { return m_isActive; }
	void			setIsActive(bool isActive) { m_isActive = IsActive; }
};

