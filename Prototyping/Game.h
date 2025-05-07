#pragma once
#include "BaseGame.h"
#include "Player.h"
#include "BulletManager.h"
#include "NPCManager.h"
#include "Team.h"
#include "HudManager.h"
#include "NPC.h"
#include "Texture.h"
#include <vector>

class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

	void RestartGame();

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void GenerateObstacles();
	void SpawnWave();
	bool IsWaveComplete() const;
	bool TryTeleport(const Vector2f& targetPos);
	bool IsValidTeleportPosition(const Vector2f& targetPos) const;
	Vector2f GetPlayerVelocity() const;

	// MEMBERS
	Player m_Player;
	BulletManager m_BulletManager;
	NPCManager m_NPCManager;
	HudManager m_HudManager;
	std::vector<std::vector<Vector2f>> m_VerticiesLevel;
	std::vector<Rectf> m_BoundsLevel;
	std::vector<std::vector<Vector2f>> m_VerticiesTarget;
	std::vector<std::vector<Vector2f>> m_VerticiesNonTarget;
	bool m_IsTeleportMode;
	std::vector<std::vector<Vector2f>> m_GeneratedObstacles;
	std::vector<NPC> m_Enemies;
	std::vector<std::vector<Vector2f>> m_Obstacles;
	
	// Teleport system
	float m_TeleportHoldTime;
	float m_RequiredHoldTime;
	Texture m_TeleportTarget;
	Texture m_TeleportText;
	Texture m_TeleportErrorText;
	Vector2f m_TeleportTargetPos;
	bool m_IsHoldingTeleport;

	// Restart system
	Texture m_RestartText;
	Texture m_RestartingText;
	float m_RestartHoldTime;
	bool m_IsHoldingRestart;

	// Wave system
	int m_CurrentWave;
	int m_EnemiesPerWave;
	float m_WaveTimer;
	float m_WaveDelay;
	bool m_IsWaveActive;
};