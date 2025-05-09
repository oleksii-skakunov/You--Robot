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
	void Update(Vector2f velocity, float elapsedSec, std::vector<std::vector<Vector2f>> levelVerticies);
	void NextFrame();
	int GetBulletType();
	Vector2f GetPos();
	Rectf GetBounds() const;
	Vector2f GetCenterPos();
	void DebugSetLaw(); // should make a class that holds law id, law desc and law bool and manipulate a vector of them
	void SetHealth(int delta);
	int GetHealth() const;
	void SetPosition(Vector2f newPos);
	void SetVisible(bool isVisible);
	void Reset();
private:
	int m_Health;
	Texture m_Spritesheet;
	Rectf m_Bounds;
	int m_CurrentBulletType;
	utils::HitInfo m_HitInfo;
	bool m_Law_NoHarmNonTarget;
	bool m_IsVisible;
	enum class State {
		Idle,
		Walk,
		Hurt,
		Dead
	};
	State m_State;
	Rectf m_CurrentFrame;
	float m_FrameTimer;
	float m_CurrentFrameTime;
	bool m_IsDead;
	bool m_IsLookingLeft;
};

