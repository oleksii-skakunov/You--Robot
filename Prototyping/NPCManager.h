#pragma once
#include "vector"
#include "NPC.h"
#include "Team.h"
class NPCManager
{
public:
	NPCManager();
	~NPCManager();
	void Draw() const;
	void Update(float elapsedSec, Vector2f targetPos, std::vector<std::vector<Vector2f>> levelVerticies);
	bool Shoot(Vector2f targetPos, int i);
	void AddNPC(int team, const Vector2f& addPos, std::vector<std::vector<Vector2f>>& targetVector);
	void UpdateVector(std::vector<std::vector<Vector2f>>& targetVector, Team team);
	void DeleteAll();
	const std::vector<NPC*>& GetNPCVector() const;
private:
	void AddHitboxToVector(std::vector<std::vector<Vector2f>>& targetVector, int i);
	std::vector<NPC*> m_NPCs;
};