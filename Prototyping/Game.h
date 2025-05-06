#pragma once
#include "BaseGame.h"
#include "Player.h"
#include "BulletManager.h"
#include "NPCManager.h"
#include "Team.h"
#include "HudManager.h"
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

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	// MEMBERS
	Player m_Player;
	BulletManager m_BulletManager;
	NPCManager m_NPCManager;
	HudManager m_HudManager;
	std::vector<std::vector<Vector2f>> m_VerticiesLevel;
	std::vector<std::vector<Vector2f>> m_VerticiesTarget;
	std::vector<std::vector<Vector2f>> m_VerticiesNonTarget;
};