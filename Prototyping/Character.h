//#pragma once
//#include "Team.h"
//class Character
//{
//public:
//
//	Character(Team team, int health = 1, Vector2f pos);
//	void Update(Vector2f velocity, float elapsedSec);
//
//};
//
//
//class NPC
//{
//public:
//	NPC();
//	NPC(Team team);
//	void Draw() const;
//	bool Shoot(Vector2f mousePos);
//	void Update(Vector2f velocity, float elapsedSec);
//	int GetBulletType();
//	Vector2f GetPos();
//	Rectf GetBounds();
//	int GetHealth();
//private:
//	int m_Health;
//	Rectf m_Bounds;
//	int m_CurrentBulletType;
//	enum class Team {
//		Player,
//		Company,
//		Rebel
//	};
//	Team m_Team;
//};
//
//
//class Player
//{
//public:
//	Player();
//	void Draw() const;
//	void DebugDraw() const;
//	bool Shoot(Vector2f mousePos);
//	void Update(Vector2f velocity, float elapsedSec);
//	int GetBulletType();
//	Vector2f GetPos();
//	Vector2f GetCenterPos();
//private:
//	int m_Health;
//	Texture m_Texture;
//	Rectf m_Bounds;
//	int m_CurrentBulletType;
//};
//
// forgot why i was doing this