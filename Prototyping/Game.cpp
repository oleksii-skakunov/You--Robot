#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <random>
// all issues atm:
// bullets spawn from bottom left of the character and don't go directly to the cursor

// ideas
// you can earn defective points by breaking the laws yourself, but then you get a shit ton of debufs
Game::Game( const Window& window ) 
	:BaseGame{ window },
	m_Player{},
	m_HudManager(GetViewPort(), m_Player.GetHealth()),
	m_IsTeleportMode{true}, // Start in teleport mode
	m_CurrentWave{0},
	m_EnemiesPerWave{3},
	m_WaveTimer{0.f},
	m_WaveDelay{0.f}, // No delay between waves
	m_IsWaveActive{false},
	m_Enemies{},
	m_Obstacles{},
	m_TeleportHoldTime{0.f},
	m_RequiredHoldTime{1.5f},
	m_TeleportTarget{"Target.png"},
	m_TeleportText{"Hold to deploy", "Font.ttf", 20, Color4f{1.f, 1.f, 1.f, 1.f}},
	m_TeleportErrorText{"Cannot deploy here!", "Font.ttf", 20, Color4f{1.f, 0.f, 0.f, 1.f}},
	m_TeleportTargetPos{0.f, 0.f},
	m_IsHoldingTeleport{false},
	m_RestartText{"Press R to restart", "Font.ttf", 20, Color4f{1.f, 1.f, 1.f, 1.f}},
	m_RestartingText{"Restarting...", "Font.ttf", 20, Color4f{1.f, 1.f, 1.f, 1.f}},
	m_RestartHoldTime{0.f},
	m_IsHoldingRestart{false}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	m_VerticiesLevel.push_back(std::vector < Vector2f>{
		Vector2f{ 5.f, 5.f },
			Vector2f{ GetViewPort().width - 5.f,5.f },
			Vector2f(GetViewPort().width - 5.f, GetViewPort().height - 5.f),
			Vector2f(5, GetViewPort().height - 5.f)
	});
	std::vector<std::vector<Vector2f>> m_VerticiesTarget{};
	std::vector<std::vector<Vector2f>> m_VerticiesNonTarget{};
	GenerateObstacles();
	m_Player.SetVisible(false); // Set player to invisible at start
	SpawnWave(); // Start first wave immediately
}

void Game::GenerateObstacles()
{
	// Remove previously generated obstacles
	// Keep only the first element (the level boundary) in m_VerticiesLevel
	if (m_VerticiesLevel.size() > 1)
	{
		auto levelBoundary = m_VerticiesLevel[0];
		m_VerticiesLevel.clear();
		m_VerticiesLevel.push_back(levelBoundary);
	}
	m_GeneratedObstacles.clear();

	// Random number generation setup
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> numObstacles(3, 5);
	std::uniform_real_distribution<> posX(50.f, GetViewPort().width - 50.f);
	std::uniform_real_distribution<> posY(50.f, GetViewPort().height - 50.f);
	std::uniform_real_distribution<> size(50.f, 150.f);

	// Generate new obstacles
	int obstacleCount = numObstacles(gen);
	for (int i = 0; i < obstacleCount; ++i)
	{
		float x = posX(gen);
		float y = posY(gen);
		float width = size(gen);
		float height = size(gen);

		// Create rectangle vertices
		std::vector<Vector2f> obstacle{
			Vector2f{ x, y },
			Vector2f{ x + width, y },
			Vector2f{ x + width, y + height },
			Vector2f{ x, y + height }
		};

		m_VerticiesLevel.push_back(obstacle);
		m_GeneratedObstacles.push_back(obstacle);
	}
}

void Game::Cleanup( )
{
	m_BulletManager.DeleteAll();
}

void Game::Update( float elapsedSec )
{
	m_BulletManager.Update(elapsedSec, m_NPCManager, m_Player, m_HudManager, m_VerticiesLevel);
	m_HudManager.UpdateHealth(elapsedSec, 0);

	// Update restart hold time
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_R])
	{
		if (m_Player.GetHealth() <= 0)
		{
			RestartGame();
		}
		else
		{
			m_RestartHoldTime += elapsedSec;
			if (m_RestartHoldTime >= m_RequiredHoldTime)
			{
				RestartGame();
				m_RestartHoldTime = 0.f;
			}
		}
	}
	else
	{
		m_RestartHoldTime = 0.f;
	}

	// Wave system update
	if (!m_IsWaveActive)
	{
		SpawnWave();
	}
	else if (IsWaveComplete())
	{
		m_IsWaveActive = false;
		m_CurrentWave++;
		m_EnemiesPerWave = 3 + m_CurrentWave; // Increase enemies per wave
		GenerateObstacles(); // Regenerate obstacles
		m_IsTeleportMode = true; // Enter teleport mode
		m_Player.SetVisible(false);
		std::cout << "Wave " << m_CurrentWave << " completed! Entering teleport mode for wave " << m_CurrentWave + 1 << std::endl;
	}

	// Only update enemies if player is visible
	if (!m_IsTeleportMode)
	{
		for (int i = 0; i < m_NPCManager.GetNPCVector().size(); i++)
		{
			if (m_NPCManager.Shoot(m_Player.GetCenterPos(), i)) {
				m_BulletManager.AddBullet(m_Player.GetBulletType(), m_NPCManager.GetNPCVector()[i]->GetPos(), m_Player.GetCenterPos(), Team::Target);
			}
		}
		m_NPCManager.Update(elapsedSec, m_Player.GetCenterPos(), m_VerticiesLevel);
	}

	// Update player if not in teleport mode
	if (!m_IsTeleportMode)
	{
		m_Player.Update(GetPlayerVelocity(), elapsedSec, m_VerticiesLevel);
	}
	else
	{
		// Update teleport hold time if holding
		if (m_IsHoldingTeleport)
		{
			m_TeleportHoldTime += elapsedSec;
			
			// Check if held long enough and try to teleport immediately
			if (m_TeleportHoldTime >= m_RequiredHoldTime)
			{
				if (TryTeleport(m_TeleportTargetPos))
				{
					m_IsTeleportMode = false;
					m_IsHoldingTeleport = false;
					m_TeleportHoldTime = 0.f;
				}
			}
		}
		else
		{
			m_TeleportHoldTime = 0.f;
		}
	}
}

void Game::SpawnWave()
{
	m_IsWaveActive = true;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> posX(50.f, GetViewPort().width - 50.f);
	std::uniform_real_distribution<> posY(50.f, GetViewPort().height - 50.f);

	int enemiesSpawned = 0;
	int maxAttempts = 100; // Prevent infinite loops

	while (enemiesSpawned < m_EnemiesPerWave && maxAttempts > 0)
	{
		float x = posX(gen);
		float y = posY(gen);
		
		// Create a rectangle representing the enemy's bounds
		Rectf enemyBounds{ x, y, 35.f, 35.f }; // Using NPC's default size
		
		// Check if the position is inside any obstacle
		bool isInsideObstacle = false;
		for (const auto& obstacle : m_VerticiesLevel)
		{
			// Skip the level boundary (first obstacle)
			if (&obstacle == &m_VerticiesLevel[0]) continue;

			// Check if any corner of the enemy bounds is inside the obstacle
			Vector2f bottomLeft{ enemyBounds.left, enemyBounds.bottom };
			Vector2f bottomRight{ enemyBounds.left + enemyBounds.width, enemyBounds.bottom };
			Vector2f topLeft{ enemyBounds.left, enemyBounds.bottom + enemyBounds.height };
			Vector2f topRight{ enemyBounds.left + enemyBounds.width, enemyBounds.bottom + enemyBounds.height };

			// Check if any corner is inside the obstacle
			if (utils::IsPointInPolygon(bottomLeft, obstacle) ||
				utils::IsPointInPolygon(bottomRight, obstacle) ||
				utils::IsPointInPolygon(topLeft, obstacle) ||
				utils::IsPointInPolygon(topRight, obstacle))
			{
				isInsideObstacle = true;
				break;
			}
		}

		if (!isInsideObstacle)
		{
			m_NPCManager.AddNPC(2, Vector2f{ x, y }, m_VerticiesTarget);
			enemiesSpawned++;
		}

		maxAttempts--;
	}

	if (enemiesSpawned < m_EnemiesPerWave)
	{
		std::cout << "Warning: Could only spawn " << enemiesSpawned << " out of " << m_EnemiesPerWave << " enemies due to obstacle constraints" << std::endl;
	}

	std::cout << "Wave " << m_CurrentWave + 1 << " started with " << enemiesSpawned << " enemies" << std::endl;
}

bool Game::IsWaveComplete() const
{
	return m_NPCManager.GetNPCVector().empty();
}

void Game::Draw( ) const
{
	ClearBackground( );
	m_Player.Draw();
	m_BulletManager.Draw();
	m_NPCManager.Draw();
	m_HudManager.DrawHealth();
	utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
	for (size_t i = 0; i < m_VerticiesLevel.size(); i++)
	{
		for (size_t o = 0; o < m_VerticiesLevel[i].size(); o++)
		{
			utils::DrawLine(m_VerticiesLevel[i][o], m_VerticiesLevel[i][(o + 1) % m_VerticiesLevel[i].size()]);
		}
	}
	
	// Draw obstacles
	for (const auto& obstacle : m_GeneratedObstacles)
	{
		utils::SetColor(Color4f{ 0.5f, 0.5f, 0.5f, 1.f });
		utils::FillPolygon(obstacle);
	}

	// Draw enemies
	for (const auto& enemy : m_Enemies)
	{
		enemy.Draw();
	}

	// Draw player if not in teleport mode
	if (!m_IsTeleportMode)
	{
		m_Player.Draw();
	}
	else
	{
		// Draw teleport target centered on cursor
		Rectf targetBounds{
			m_TeleportTargetPos.x - m_TeleportTarget.GetWidth() / 2,
			m_TeleportTargetPos.y - m_TeleportTarget.GetHeight() / 2,
			m_TeleportTarget.GetWidth(),
			m_TeleportTarget.GetHeight()
		};
		m_TeleportTarget.Draw(targetBounds);
		
		// Draw hold progress
		if (m_IsHoldingTeleport)
		{
			float progress = m_TeleportHoldTime / m_RequiredHoldTime;
			Rectf progressBar{ m_TeleportTargetPos.x - 25.f, m_TeleportTargetPos.y + 30.f, 50.f, 5.f };
			Rectf progressFill{ progressBar.left, progressBar.bottom, progressBar.width * progress, progressBar.height };
			
			utils::SetColor(Color4f{ 0.2f, 0.2f, 0.2f, 1.f });
			utils::FillRect(progressBar);
			utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
			utils::FillRect(progressFill);
		}
		
		// Draw appropriate text based on position validity
		Vector2f textPos{ m_TeleportTargetPos.x - (IsValidTeleportPosition(m_TeleportTargetPos) ? m_TeleportText.GetWidth() : m_TeleportErrorText.GetWidth()) / 2, 
						 m_TeleportTargetPos.y + 40.f };
		if (IsValidTeleportPosition(m_TeleportTargetPos))
		{
			m_TeleportText.Draw(textPos);
		}
		else
		{
			m_TeleportErrorText.Draw(textPos);
		}
	}

	// Draw restart text if player is dead
	if (m_Player.GetHealth() <= 0)
	{
		Vector2f restartTextPos{ GetViewPort().width / 2.f - m_RestartText.GetWidth() / 2.f,
								GetViewPort().height / 2.f };
		m_RestartText.Draw(restartTextPos);
	}
	// Draw restarting text if player is holding R while alive
	else if (m_RestartHoldTime > 0.f)
	{
		Vector2f restartingTextPos{ GetViewPort().width / 2.f - m_RestartingText.GetWidth() / 2.f,
								   GetViewPort().height / 2.f };
		m_RestartingText.Draw(restartingTextPos);

		// Draw progress bar
		float progress = m_RestartHoldTime / m_RequiredHoldTime;
		Rectf progressBar{ GetViewPort().width / 2.f - 25.f, GetViewPort().height / 2.f + 30.f, 50.f, 5.f };
		Rectf progressFill{ progressBar.left, progressBar.bottom, progressBar.width * progress, progressBar.height };
		
		utils::SetColor(Color4f{ 0.2f, 0.2f, 0.2f, 1.f });
		utils::FillRect(progressBar);
		utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
		utils::FillRect(progressFill);
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch (e.keysym.sym)
	{
	case SDLK_1:
		m_Player.DebugSetLaw();
		break;
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	switch (e.keysym.sym)
	{
	case SDLK_1:
		m_Player.DebugSetLaw();
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	// Update teleport target position
	m_TeleportTargetPos = Vector2f{ float(e.x), float(e.y) };
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		if (m_IsTeleportMode)
		{
			m_IsHoldingTeleport = true;
			m_TeleportHoldTime = 0.f;
		}
		else
		{
			m_NPCManager.UpdateVector(m_VerticiesTarget, Team::Target);
			m_NPCManager.UpdateVector(m_VerticiesNonTarget, Team::NonTarget);
			if (m_Player.Shoot(Vector2f{ float(e.x), float(e.y) }, m_VerticiesTarget, m_VerticiesLevel, m_VerticiesNonTarget))
			{
				m_BulletManager.AddBullet(m_Player.GetBulletType(), m_Player.GetCenterPos(), Vector2f{ float(e.x), float(e.y) }, Team::Neutral);
			}
		}
		break;
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	if (m_IsTeleportMode && e.button == SDL_BUTTON_LEFT)
	{
		m_IsHoldingTeleport = false;
		m_TeleportHoldTime = 0.f;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

bool Game::IsValidTeleportPosition(const Vector2f& targetPos) const
{
	Rectf playerBounds{ targetPos.x - (m_Player.GetBounds().width / 2), 
					   targetPos.y - (m_Player.GetBounds().height / 2),
					   m_Player.GetBounds().width,
					   m_Player.GetBounds().height };

	for (const auto& obstacle : m_VerticiesLevel)
	{
		// Skip the level boundary
		if (&obstacle == &m_VerticiesLevel[0]) continue;

		// Check if any corner of the player bounds would be inside an obstacle
		Vector2f bottomLeft{ playerBounds.left, playerBounds.bottom };
		Vector2f bottomRight{ playerBounds.left + playerBounds.width, playerBounds.bottom };
		Vector2f topLeft{ playerBounds.left, playerBounds.bottom + playerBounds.height };
		Vector2f topRight{ playerBounds.left + playerBounds.width, playerBounds.bottom + playerBounds.height };

		if (utils::IsPointInPolygon(bottomLeft, obstacle) ||
			utils::IsPointInPolygon(bottomRight, obstacle) ||
			utils::IsPointInPolygon(topLeft, obstacle) ||
			utils::IsPointInPolygon(topRight, obstacle))
		{
			return false;
		}
	}
	return true;
}

bool Game::TryTeleport(const Vector2f& targetPos)
{
	if (IsValidTeleportPosition(targetPos))
	{
		m_Player.SetPosition(targetPos);
		m_Player.SetVisible(true);
		return true;
	}
	return false;
}

Vector2f Game::GetPlayerVelocity() const
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	Vector2f velocity{ 0.f, 0.f };
	
	if (pStates[SDL_SCANCODE_W])
	{
		velocity.y += 250.f;
	}
	if (pStates[SDL_SCANCODE_A])
	{
		velocity.x -= 250.f;
	}
	if (pStates[SDL_SCANCODE_S])
	{
		velocity.y -= 250.f;
	}
	if (pStates[SDL_SCANCODE_D])
	{
		velocity.x += 250.f;
	}
	
	return velocity;
}

void Game::RestartGame()
{
	// Reset game state
	m_CurrentWave = 0;
	m_EnemiesPerWave = 3;
	m_WaveTimer = 0.f;
	m_IsWaveActive = false;
	m_IsTeleportMode = true;
	
	// Reset player state
	m_Player.Reset();
	
	// Clear all enemies and bullets
	m_NPCManager.DeleteAll();
	m_BulletManager.DeleteAll();
	
	// Regenerate obstacles
	GenerateObstacles();
	
	// Start first wave
	SpawnWave();
}
