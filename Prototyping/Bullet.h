#pragma once
#include "Texture.h"
class Bullet
{
public:
	Bullet(float traveltime, int damage, const Vector2f startPos, float velocity, const Texture* pTexture, float angle, int team);
	void Draw() const;
	void Update(float elapsedSec);
	bool HasStopped() const;
private:
	float m_Lifetime;
	float m_Traveltime;
	float m_Angle;
	int m_Damage;
	Vector2f m_Pos;
	float m_Velocity;
	const Texture* m_pTexture;
	bool m_HasStopped;
	enum class Team {
		Company
	};
	Team m_Team;
};

