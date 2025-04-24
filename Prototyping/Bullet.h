#pragma once
#include "Texture.h"
#include "NPCManager.h"
class Bullet
{
public:
	Bullet(float traveltime, int damage, const Vector2f startPos, Vector2f velocityVector, const Texture* pTexture, float angle, int team);
	~Bullet();
	void Draw() const;
	void Update(float elapsedSec, NPCManager& npcManager);
	bool HasStopped() const;
private:
	float m_Lifetime;
	float m_Traveltime;
	float m_Angle;
	int m_Damage;
	Vector2f m_Pos;
	Vector2f m_VelocityVector;
	const Texture* m_pTexture;
	bool m_HasStopped;
	enum class Team {
		Player,
		Company,
		Rebel
	};
	Team m_Team;
};

