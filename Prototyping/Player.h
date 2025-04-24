#pragma once
#include "Texture.h"
#include "utils.h"
class Player
{
public:
	Player();
	void Draw() const;
	void DebugDraw() const;
	bool Shoot(Vector2f mousePos, std::vector<std::vector<Vector2f>> targetVector, std::vector<std::vector<Vector2f>> levelVector, std::vector<std::vector<Vector2f>> nonTargetVector);
	void Update(Vector2f velocity, float elapsedSec);
	int GetBulletType();
	Vector2f GetPos();
	Rectf GetBounds();
	Vector2f GetCenterPos();
	void DebugSetLaw(); // should make a class that holds law id, law desc and law bool and manipulate a vector of them
private:
	int m_Health;
	Texture m_Texture;
	Rectf m_Bounds;
	int m_CurrentBulletType;
	utils::HitInfo m_HitInfo;
	bool m_Law_NoHarmNonTarget;
};

