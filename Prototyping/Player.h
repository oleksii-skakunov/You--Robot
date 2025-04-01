#pragma once
#include "Texture.h"
#include "utils.h"
class Player
{
public:
	Player();
	void Draw() const;
	bool Shoot(Vector2f mousePos);
	void Move(Vector2f velocity, float elapsedSec);
	int GetBulletType();
	Vector2f GetPos();
private:
	int m_Health;
	Texture m_Texture;
	Vector2f m_Position;
	int m_CurrentBulletType;
};

