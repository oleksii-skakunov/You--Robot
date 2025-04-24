#pragma once
#include "Texture.h"
#include "utils.h"
#include "Team.h"
class NPC
{
public:
	NPC();
	NPC(Team team, Vector2f pos);
	void Draw() const;
	bool Shoot(Vector2f mousePos);
	void Update(Vector2f velocity, float elapsedSec);
	int GetBulletType();
	Vector2f GetPos();
	Rectf GetBounds();
	int GetHealth();
	void AddHealth(int delta);
	Team GetTeam();
private:
	int m_Health;
	Rectf m_Bounds;
	int m_CurrentBulletType;
	Team m_Team;
};


