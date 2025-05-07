#pragma once
#include "Texture.h"
#include "NPCManager.h"
#include "Player.h"
#include "Team.h"
#include "HudManager.h"
class Bullet
{
public:
	Bullet(float traveltime, int damage, const Vector2f startPos, Vector2f velocityVector, const Texture* pTexture, float angle, Team team);
	~Bullet();
	void Draw() const;
	void Update(float elapsedSec, NPCManager& npcManager, Player& player, HudManager& hudManager, std::vector<std::vector<Vector2f>> levelVerticies);
	bool HasStopped() const;
	bool lineSegmentIntersect(Vector2f p1, Vector2f p2, Vector2f p3, Vector2f p4);
private:
	float m_Lifetime;
	float m_Traveltime;
	float m_Angle;
	int m_Damage;
	Vector2f m_Pos;
	Vector2f m_VelocityVector;
	const Texture* m_pTexture;
	bool m_HasStopped;
	Team m_Team;
};

