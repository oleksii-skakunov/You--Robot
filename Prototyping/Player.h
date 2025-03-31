#pragma once
#include "Texture.h"
#include "utils.h"
class Player
{
public:
	Player();
	void Draw() const;
	void Shoot(float angle);
	void Move(Vector2f velocity, float elapsedSec);
private:
	int m_Health;
	Texture m_Texture;
	Vector2f m_Position;
};

