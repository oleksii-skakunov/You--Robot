#pragma once
#include "Texture.h"
#include "utils.h"
#include "Team.h"
#include "Action.h"
class NPC
{
public:
	NPC();
	NPC(Team team, Vector2f pos);
	void Draw() const;
	bool Shoot(Vector2f targetPos);
	void Update(Vector2f velocity, float elapsedSec, std::vector<std::vector<Vector2f>> levelVerticies);
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
	Action m_CurrentAction;
	float m_MaxMoveTime;
	float m_CurrentMoveTime;
	float m_MaxReloadTime;
	float m_CurrentReloadTime;
};


