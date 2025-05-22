#pragma once
#include "Texture.h"
#include "utils.h"
#include "Team.h"
#include "Action.h"
#include <functional>

enum class BossState
{
	Entering,    // Initial dash into arena
	Idle,        // Waiting to choose next action
	Dashing,     // Performing dash attack
	Summoning,   // Summoning minions
	Bursting,    // Performing burst attack
	PostDash     // Delay after dash attack
};

class NPC
{
public:
	NPC();
	NPC(Team team, Vector2f pos, bool isBoss = false);
	void Draw() const;
	bool Shoot(Vector2f targetPos);
	void Update(Vector2f velocity, float elapsedSec, std::vector<std::vector<Vector2f>> levelVerticies);
	int GetBulletType();
	Vector2f GetPos();
	Rectf GetBounds();
	int GetHealth();
	void AddHealth(int delta);
	Vector2f GetCenterPos() const;
	Team GetTeam();
	bool IsBoss() const { return m_IsBoss; }
	void SetSpawnDelay(float delay) { m_SpawnDelay = delay; }
	bool ShouldSpawn() const { return m_SpawnDelay <= 0.f; }
	void UpdateSpawnDelay(float elapsedSec) { m_SpawnDelay -= elapsedSec; }
	void SetTargetPosition(const Vector2f& target) { m_TargetPosition = target; }
	bool HasReachedTarget() const { return m_HasReachedTarget; }
	Vector2f GetShootingTarget() const;
	void SetPosition(const Vector2f& newPos);
	void SetLevelVertices(const std::vector<std::vector<Vector2f>>& vertices) { m_LevelVertices = vertices; }
	void SetSummonCallback(std::function<void(Vector2f, Vector2f)> callback) { m_SummonCallback = callback; }

	// Boss-related public variables
	BossState m_BossState;
	float m_ActionTimer;
	float m_DashDistance;
	Vector2f m_DashStartPos;
	Vector2f m_DashDirection;
	Vector2f m_LastKnownPlayerPos;
	Vector2f m_BurstTargetPos;

private:
	int m_Health;
	Rectf m_Bounds;
	int m_CurrentBulletType;
	Team m_Team;
	Action m_CurrentAction;
	float m_MaxMoveTime;
	float m_CurrentMoveTime;
	float m_InitialDelay;
	float m_MaxReloadTime;
	float m_CurrentReloadTime;
	bool m_IsBoss;
	float m_SpawnDelay;
	float m_BurstTimer;
	int m_BulletsInBurst;
	Vector2f m_TargetPosition;
	bool m_HasReachedTarget;
	float m_MoveSpeed;
	bool m_IsFiringBurst;
	std::vector<std::vector<Vector2f>> m_LevelVertices;
	float m_FlashTimer;
	int m_PreviousHealth;
	std::function<void(Vector2f, Vector2f)> m_SummonCallback;
	float m_DashCooldown;

	// Helper function to check line of sight
	bool HasLineOfSight(const Vector2f& targetPos, const std::vector<std::vector<Vector2f>>& obstacles) const;
};


