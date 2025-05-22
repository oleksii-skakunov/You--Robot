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
	m_HudManager(GetViewPort(), m_Player.GetHealth(), m_Player.GetCurrentAmmo(), m_Player.GetMaxAmmo()),
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
	m_IsHoldingRestart{false},
	m_Test{"PlayerSpritesheet.png"},
	m_IsWinScreen{false},
	m_WaveHadBoss{false},
	m_WinText{"Victory!", "Font.ttf", 36, Color4f{1.f, 1.f, 0.f, 1.f}},
	m_ContinueText{"Hold C to Continue", "Font.ttf", 20, Color4f{1.f, 1.f, 1.f, 1.f}},
	m_FinishText{"Hold F to Finish Run", "Font.ttf", 20, Color4f{1.f, 1.f, 1.f, 1.f}},
	m_ContinueHoldTime{0.f},
	m_FinishHoldTime{0.f},
	m_IsHoldingContinue{false},
	m_IsHoldingFinish{false},
	m_ScoreText{"", "Font.ttf", 24, Color4f{1.f, 1.f, 1.f, 1.f}},
	m_MaxAmmo{200},
	m_CurrentAmmo{50},
	m_Pickups{}
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
		Vector2f{ -5.f, -5.f },
			Vector2f{ GetViewPort().width + 5.f,-5.f },
			Vector2f(GetViewPort().width + 5.f, 5.f),
			Vector2f(-5, 5.f)
	});
	m_VerticiesLevel.push_back(std::vector < Vector2f>{
		Vector2f{ GetViewPort().width - 5.f, -5.f },
			Vector2f{ GetViewPort().width + 5.f,-5.f },
			Vector2f(GetViewPort().width + 5.f, GetViewPort().height+5.f),
			Vector2f(GetViewPort().width - 5.f, GetViewPort().height + 5.f)
	});
	m_VerticiesLevel.push_back(std::vector < Vector2f>{
		Vector2f{ - 5.f, GetViewPort().height -5.f },
			Vector2f{ GetViewPort().width + 5.f,GetViewPort().height - 5.f },
			Vector2f(GetViewPort().width + 5.f, GetViewPort().height + 5.f),
			Vector2f(- 5.f, GetViewPort().height + 5.f)
	});
	m_VerticiesLevel.push_back(std::vector < Vector2f>{
		Vector2f{ -5.f, -5.f },
			Vector2f{ 5.f, -5.f },
			Vector2f{ 5.f, GetViewPort().height + 5.f },
			Vector2f(-5.f, GetViewPort().height + 5.f)
	});
	std::vector<std::vector<Vector2f>> m_VerticiesTarget{};
	std::vector<std::vector<Vector2f>> m_VerticiesNonTarget{};
	GenerateObstacles();
	m_Player.SetVisible(false); // Set player to invisible at start
	
	// Debug: Set wave to 9
	m_CurrentWave = 9; // Set to 9 because the wave number is incremented when the first wave starts
	
	SpawnWave(); // Start first wave immediately
}

void Game::GenerateObstacles()
{
	// Remove previously generated obstacles
	// Keep only the first element (the level boundary) in m_VerticiesLevel
	if (m_VerticiesLevel.size() > 4)
	{
		std::vector < Vector2f> levelBoundaryBottom = m_VerticiesLevel[0];
		std::vector < Vector2f> levelBoundaryRight = m_VerticiesLevel[1];
		std::vector < Vector2f> levelBoundaryUp = m_VerticiesLevel[2];
		std::vector < Vector2f> levelBoundaryLeft = m_VerticiesLevel[3];
		m_VerticiesLevel.clear();
		m_VerticiesLevel.push_back(levelBoundaryBottom);
		m_VerticiesLevel.push_back(levelBoundaryRight);
		m_VerticiesLevel.push_back(levelBoundaryUp);
		m_VerticiesLevel.push_back(levelBoundaryLeft);
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
		float x = float(posX(gen));
		float y = float(posY(gen));
		float width = float(size(gen));
		float height = float(size(gen));

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
	m_HudManager.UpdateAmmo(m_Player.GetCurrentAmmo());

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
		// Check if this wave had bosses and should show win screen
		if (m_WaveHadBoss && !m_IsWinScreen)
		{
			m_IsWinScreen = true;
			std::string scoreString = "Final Score: " + std::to_string(m_HudManager.GetScore());
			m_ScoreText = Texture{scoreString, "Font.ttf", 24, Color4f{1.f, 1.f, 1.f, 1.f}};
			std::cout << "All bosses defeated! Showing win screen..." << std::endl;
			return;
		}
		
		if (!m_IsWinScreen && m_EscapePod.GetState() == EscapePod::State::Inactive)
		{
			// Start escape pod landing sequence only if it's not already active
			m_EscapePod.StartLanding(m_LastEnemyDeathPos, m_VerticiesLevel);
		}
	}

	// Update escape pod
	m_EscapePod.Update(elapsedSec, m_Player.GetCenterPos(), m_VerticiesLevel);

	// Handle escape pod interaction
	if (m_EscapePod.IsActive() && m_EscapePod.IsPlayerInRange(m_Player.GetCenterPos()))
	{
		if (pStates[SDL_SCANCODE_E])
		{
			m_Player.SetVisible(false);
			m_EscapePod.StartExit();
		}
	}

	// Check if escape pod has finished exit animation
	if (m_EscapePod.GetState() == EscapePod::State::Exfil)
	{
		m_IsWaveActive = false;
		m_CurrentWave++;
		m_EnemiesPerWave = 3 + m_CurrentWave;
		m_WaveHadBoss = false;
		GenerateObstacles();
		m_IsTeleportMode = true;
		m_EscapePod.SetState(EscapePod::State::Inactive);  // Reset to Inactive after spawning new wave
		std::cout << "Wave " << m_CurrentWave << " completed! Entering teleport mode for wave " << m_CurrentWave + 1 << std::endl;
	}

	// Handle win screen inputs
	if (m_IsWinScreen)
	{
		// Handle Continue (C key) with separate timer
		if (pStates[SDL_SCANCODE_C])
		{
			if (!m_IsHoldingContinue)
			{
				m_IsHoldingContinue = true;
				m_ContinueHoldTime = 0.f;
			}
			m_ContinueHoldTime += elapsedSec;
			if (m_ContinueHoldTime >= m_RequiredHoldTime)
			{
				// Continue to next wave
				m_IsWinScreen = false;
				m_IsWaveActive = false;
				m_CurrentWave++;
				m_EnemiesPerWave = 3 + m_CurrentWave;
				m_WaveHadBoss = false;
				GenerateObstacles();
				m_IsTeleportMode = true;
				m_Player.SetVisible(false);
				m_ContinueHoldTime = 0.f;
				m_FinishHoldTime = 0.f;
				m_IsHoldingContinue = false;
				m_IsHoldingFinish = false;
				std::cout << "Continuing to wave " << m_CurrentWave + 1 << std::endl;
			}
		}
		else
		{
			m_IsHoldingContinue = false;
			m_ContinueHoldTime = 0.f;
		}
		
		// Handle Finish (F key) with separate timer
		if (pStates[SDL_SCANCODE_F])
		{
			if (!m_IsHoldingFinish)
			{
				m_IsHoldingFinish = true;
				m_FinishHoldTime = 0.f;
			}
			m_FinishHoldTime += elapsedSec;
			if (m_FinishHoldTime >= m_RequiredHoldTime)
			{
				// Finish the run (restart)
				RestartGame();
				m_IsWinScreen = false;
				m_ContinueHoldTime = 0.f;
				m_FinishHoldTime = 0.f;
				m_IsHoldingContinue = false;
				m_IsHoldingFinish = false;
				std::cout << "Finishing run and restarting..." << std::endl;
			}
		}
		else
		{
			m_IsHoldingFinish = false;
			m_FinishHoldTime = 0.f;
		}
		
		return; // Don't update game logic while in win screen
	}

	// Only update enemies if player is visible
	if (!m_IsTeleportMode)
	{
		for (int i = 0; i < m_NPCManager.GetNPCVector().size(); i++)
		{
			if (m_NPCManager.Shoot(m_Player.GetCenterPos(), i)) {
				if (m_NPCManager.GetNPCVector()[i]->IsBoss())
				{
					m_BulletManager.AddBullet(m_Player.GetBulletType(), m_NPCManager.GetNPCVector()[i]->GetPos(), m_NPCManager.GetNPCVector()[i]->m_LastKnownPlayerPos, Team::Target);
				}
				else
				{
					m_BulletManager.AddBullet(m_Player.GetBulletType(), m_NPCManager.GetNPCVector()[i]->GetPos(), m_Player.GetCenterPos(), Team::Target);
				}
				
			}
		}
		
		// Store the current number of NPCs before updating
		size_t currentNPCCount = m_NPCManager.GetNPCVector().size();
		
		// Create a copy of NPC positions before update (in case they die)
		std::vector<Vector2f> npcPositions;
		for (const auto& npc : m_NPCManager.GetNPCVector()) {
			npcPositions.push_back(npc->GetCenterPos());
		}
		
		// Update NPCs
		m_NPCManager.Update(elapsedSec, m_Player.GetCenterPos(), m_VerticiesLevel);
		
		// Check if any NPCs died during the update
		if (m_NPCManager.GetNPCVector().size() < currentNPCCount) {
			// Store the position of the last enemy that died
			if (!npcPositions.empty()) {
				m_LastEnemyDeathPos = npcPositions.back();
			}

			// Some NPCs died, try to spawn pickups at their positions
			size_t newCount = m_NPCManager.GetNPCVector().size();
			size_t deadCount = currentNPCCount - newCount;
			
			for (size_t i = 0; i < npcPositions.size() && deadCount > 0; ++i) {
				bool found = false;
				for (const auto& npc : m_NPCManager.GetNPCVector()) {
					if (npc->GetCenterPos().x == npcPositions[i].x && 
						npc->GetCenterPos().y == npcPositions[i].y) {
						found = true;
						break;
					}
				}
				
				if (!found) {
					TrySpawnPickup(npcPositions[i]);
					--deadCount;
				}
			}
		}
	}

	// Update player if not in teleport mode
	if (!m_IsTeleportMode)
	{
		m_Player.Update(GetPlayerVelocity(), elapsedSec, m_VerticiesLevel);
		
		// Update pickups and check for collisions
		UpdatePickups();
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

	// Spawn regular enemies
	while (enemiesSpawned < m_EnemiesPerWave && maxAttempts > 0)
	{
		float x = float(posX(gen));
		float y = float(posY(gen));
		
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

	// Spawn bosses every 10th wave
	if ((m_CurrentWave + 1) % 10 == 0)
	{
		m_WaveHadBoss = true;
		
		int numBosses = (m_CurrentWave + 1) / 10;
		std::uniform_int_distribution<> wallChoice(0, 3);

		Vector2f levelCenter{ GetViewPort().width / 2.f, GetViewPort().height / 2.f };
		std::cout << "Spawning " << numBosses << " boss(es) for wave " << m_CurrentWave + 1 << std::endl;

		for (int i = 0; i < numBosses; i++)
		{
			int wall = wallChoice(gen);
			Vector2f spawnPos;
			
			switch (wall)
			{
			case 0: // Top wall
				spawnPos = Vector2f{ float(posX(gen)), GetViewPort().height + 100.f };
				break;
			case 1: // Right wall
				spawnPos = Vector2f{ GetViewPort().width + 100.f, float(posY(gen)) };
				break;
			case 2: // Bottom wall
				spawnPos = Vector2f{ float(posX(gen)), -100.f };
				break;
			case 3: // Left wall
				spawnPos = Vector2f{ -100.f, float(posY(gen)) };
				break;
			}

			m_NPCManager.AddBossNPC(2, spawnPos, m_VerticiesTarget);
			NPC* boss = m_NPCManager.GetNPCVector().back();
			boss->SetTargetPosition(levelCenter);
			
			// Set up summon callback
			boss->SetSummonCallback([this](Vector2f spawnPos, Vector2f targetPos) {
				m_NPCManager.AddNPC(2, spawnPos, m_VerticiesTarget);
				NPC* minion = m_NPCManager.GetNPCVector().back();
				minion->SetTargetPosition(targetPos);
				minion->m_BossState = BossState::Dashing;
				minion->m_ActionTimer = 0.5f;
				Vector2f direction = targetPos - spawnPos;
				float length = sqrt(direction.x * direction.x + direction.y * direction.y);
				if (length > 0)
				{
					minion->m_DashDirection = Vector2f{ direction.x / length, direction.y / length };
				}
				minion->m_DashDistance = 150.f;
			});
		}
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
	
	// Draw pickups
	DrawPickups();

	// Draw escape pod and its debug visualization
	m_EscapePod.Draw(m_Player.GetCenterPos());
	m_EscapePod.DebugDraw();

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
	m_HudManager.Draw();

	// Draw win screen
	if (m_IsWinScreen)
	{
		// Draw semi-transparent overlay
		utils::SetColor(Color4f{0.f, 0.f, 0.f, 0.7f});
		utils::FillRect(Rectf{0.f, 0.f, GetViewPort().width, GetViewPort().height});
		
		// Draw victory text
		Vector2f winTextPos{GetViewPort().width / 2.f - m_WinText.GetWidth() / 2.f,
							GetViewPort().height / 2.f + 50.f};
		m_WinText.Draw(winTextPos);
		
		// Draw score text
		Vector2f scoreTextPos{GetViewPort().width / 2.f - m_ScoreText.GetWidth() / 2.f,
							  GetViewPort().height / 2.f + 20.f};
		m_ScoreText.Draw(scoreTextPos);
		
		// Draw continue option
		Vector2f continueTextPos{GetViewPort().width / 2.f - m_ContinueText.GetWidth() / 2.f,
								 GetViewPort().height / 2.f};
		m_ContinueText.Draw(continueTextPos);
		
		// Draw continue progress bar
		if (m_IsHoldingContinue)
		{
			float progress = m_ContinueHoldTime / m_RequiredHoldTime;
			Rectf progressBar{GetViewPort().width / 2.f - 50.f, GetViewPort().height / 2.f - 30.f, 100.f, 5.f};
			Rectf progressFill{progressBar.left, progressBar.bottom, progressBar.width * progress, progressBar.height};
			
			utils::SetColor(Color4f{0.2f, 0.2f, 0.2f, 1.f});
			utils::FillRect(progressBar);
			utils::SetColor(Color4f{0.f, 1.f, 0.f, 1.f});
			utils::FillRect(progressFill);
		}
		
		// Draw finish option
		Vector2f finishTextPos{GetViewPort().width / 2.f - m_FinishText.GetWidth() / 2.f,
							   GetViewPort().height / 2.f - 50.f};
		m_FinishText.Draw(finishTextPos);
		
		// Draw finish progress bar
		if (m_IsHoldingFinish)
		{
			float progress = m_FinishHoldTime / m_RequiredHoldTime;
			Rectf progressBar{GetViewPort().width / 2.f - 50.f, GetViewPort().height / 2.f - 80.f, 100.f, 5.f};
			Rectf progressFill{progressBar.left, progressBar.bottom, progressBar.width * progress, progressBar.height};
			
			utils::SetColor(Color4f{0.2f, 0.2f, 0.2f, 1.f});
			utils::FillRect(progressBar);
			utils::SetColor(Color4f{1.f, 0.f, 0.f, 1.f});
			utils::FillRect(progressFill);
		}
		
		return; // Don't draw game when in win screen
	}
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	switch (e.keysym.sym)
	{
	case SDLK_1:
		m_Player.DebugSetLaw();
		break;
	case SDLK_i:
		DebugCheckEnemyBounds();
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
			// Check if player has ammo before shooting
			if (m_Player.GetCurrentAmmo() > 0)
			{
				m_NPCManager.UpdateVector(m_VerticiesTarget, Team::Target);
				m_NPCManager.UpdateVector(m_VerticiesNonTarget, Team::NonTarget);
				if (m_Player.Shoot(Vector2f{ float(e.x), float(e.y) }, m_VerticiesTarget, m_VerticiesLevel, m_VerticiesNonTarget))
				{
					m_BulletManager.AddBullet(m_Player.GetBulletType(), m_Player.GetCenterPos(), Vector2f{ float(e.x), float(e.y) }, Team::Neutral);
					m_Player.SetCurrentAmmo(-1);
				}
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
	// Calculate proposed player bounds
	Rectf playerBounds{ targetPos.x - (m_Player.GetBounds().width / 2), 
					   targetPos.y - (m_Player.GetBounds().height / 2),
					   m_Player.GetBounds().width,
					   m_Player.GetBounds().height };

	// Get player corners
	Vector2f bottomLeft{ playerBounds.left, playerBounds.bottom };
	Vector2f bottomRight{ playerBounds.left + playerBounds.width, playerBounds.bottom };
	Vector2f topLeft{ playerBounds.left, playerBounds.bottom + playerBounds.height };
	Vector2f topRight{ playerBounds.left + playerBounds.width, playerBounds.bottom + playerBounds.height };

	// Check if any corner is inside a wall
	bool isInsideWall = false;
	Vector2f repulsionForce{ 0.f, 0.f };

	for (const auto& wall : m_VerticiesLevel)
	{
		// Skip the level boundary
		if (&wall == &m_VerticiesLevel[0]) continue;

		if (utils::IsPointInPolygon(bottomLeft, wall) ||
			utils::IsPointInPolygon(bottomRight, wall) ||
			utils::IsPointInPolygon(topLeft, wall) ||
			utils::IsPointInPolygon(topRight, wall))
		{
			isInsideWall = true;
			// Calculate center of the wall
			Vector2f wallCenter{ 0.f, 0.f };
			for (const auto& point : wall)
			{
				wallCenter.x += point.x;
				wallCenter.y += point.y;
			}
			wallCenter.x /= wall.size();
			wallCenter.y /= wall.size();

			// Calculate direction away from wall center
			Vector2f direction = targetPos - wallCenter;
			float length = sqrt(direction.x * direction.x + direction.y * direction.y);
			if (length > 0.f)
			{
				direction.x /= length;
				direction.y /= length;
				// Add repulsion force
				repulsionForce.x += direction.x * 50.f; // Smaller force for teleporting
				repulsionForce.y += direction.y * 50.f;
			}
		}
	}

	// If we're inside a wall, try to find a valid position by applying repulsion
	if (isInsideWall)
	{
		// Normalize the repulsion force
		float length = sqrt(repulsionForce.x * repulsionForce.x + repulsionForce.y * repulsionForce.y);
		if (length > 0.f)
		{
			repulsionForce.x /= length;
			repulsionForce.y /= length;

			// Try multiple positions along the repulsion direction
			for (float distance = 50.f; distance <= 200.f; distance += 50.f)
			{
				Vector2f newPos = targetPos + Vector2f{ repulsionForce.x * distance, repulsionForce.y * distance };
				Rectf newBounds{ newPos.x - (m_Player.GetBounds().width / 2),
							   newPos.y - (m_Player.GetBounds().height / 2),
							   m_Player.GetBounds().width,
							   m_Player.GetBounds().height };

				// Check if this position is valid
				bool isValid = true;
				for (const auto& wall : m_VerticiesLevel)
				{
					// Skip the level boundary
					if (&wall == &m_VerticiesLevel[0]) continue;

					Vector2f newBottomLeft{ newBounds.left, newBounds.bottom };
					Vector2f newBottomRight{ newBounds.left + newBounds.width, newBounds.bottom };
					Vector2f newTopLeft{ newBounds.left, newBounds.bottom + newBounds.height };
					Vector2f newTopRight{ newBounds.left + newBounds.width, newBounds.bottom + newBounds.height };

					if (utils::IsPointInPolygon(newBottomLeft, wall) ||
						utils::IsPointInPolygon(newBottomRight, wall) ||
						utils::IsPointInPolygon(newTopLeft, wall) ||
						utils::IsPointInPolygon(newTopRight, wall))
					{
						isValid = false;
						break;
					}
				}

				if (isValid)
				{
					m_Player.SetPosition(newPos);
					m_Player.SetVisible(true);
					return true;
				}
			}
		}
		return false;
	}

	// If not inside a wall, check if the position is valid
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
	m_CurrentAmmo = 50; // Reset ammo to starting value
	
	// Reset player state
	m_Player.Reset();
	m_Player.SetCurrentAmmo(50 - m_Player.GetCurrentAmmo()); // Reset player's ammo to 50
	m_HudManager.Restart();
	// Clear all enemies and bullets
	m_NPCManager.DeleteAll();
	m_BulletManager.DeleteAll();
	
	// Clear pickups
	m_Pickups.clear();
	
	// Reset escape pod
	m_EscapePod.SetState(EscapePod::State::Inactive);
	
	// Regenerate obstacles
	GenerateObstacles();
	
	// Start first wave
	SpawnWave();

	// Reset win screen state
	m_IsWinScreen = false;
	m_WaveHadBoss = false;
	m_ContinueHoldTime = 0.f;
	m_FinishHoldTime = 0.f;
	m_IsHoldingContinue = false;
	m_IsHoldingFinish = false;
}

// New methods for pickup system
void Game::UpdatePickups()
{
	// Check for collisions with player
	auto it = m_Pickups.begin();
	while (it != m_Pickups.end())
	{
		if (it->IsColliding(m_Player.GetBounds()))
		{
			// Player collected the pickup
			if (it->GetType() == PickupType::AMMO)
			{
				// Add 25 ammo
				int newAmmo = m_Player.GetCurrentAmmo() + 25;
				// Cap at max ammo
				newAmmo = std::min(newAmmo, m_Player.GetMaxAmmo());
				m_Player.SetCurrentAmmo(newAmmo - m_Player.GetCurrentAmmo());
				std::cout << "Player picked up ammo. New ammo: " << m_Player.GetCurrentAmmo() << std::endl;
			}
			else if (it->GetType() == PickupType::HEALTH)
			{
				// Add 3 health
				int newHealth = m_Player.GetHealth() + 3;
				// Cap at max health (10)
				newHealth = std::min(newHealth, 10);
				int healthDelta = newHealth - m_Player.GetHealth();
				m_Player.SetHealth(healthDelta);
				m_HudManager.UpdateHealth(0.f, healthDelta); // Update health HUD
				std::cout << "Player picked up health. New health: " << m_Player.GetHealth() << std::endl;
			}
			
			// Remove the pickup
			it = m_Pickups.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Game::TrySpawnPickup(const Vector2f& position)
{
	// Calculate drop chances based on player's health and ammo
	float healthPercent = static_cast<float>(m_Player.GetHealth()) / 10.0f; // 10 is max health
	float ammoPercent = static_cast<float>(m_Player.GetCurrentAmmo()) / static_cast<float>(m_Player.GetMaxAmmo());
	
	// Calculate drop chances (higher chance when lower health/ammo)
	float healthDropChance = 1.0f - healthPercent - 0.2f;
	float ammoDropChance = 1.0f - ammoPercent - 0.4f;
	
	// No health drops if at max health
	if (m_Player.GetHealth() >= 10)
	{
		healthDropChance = 0.0f;
	}
	
	// Generate random value between 0 and 1
	float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	
	// Decide what to drop
	if (random < healthDropChance)
	{
		// Drop health
		m_Pickups.emplace_back(PickupType::HEALTH, position);
		std::cout << "Health pickup spawned at (" << position.x << ", " << position.y << ")" << std::endl;
	}
	else if (random < healthDropChance + ammoDropChance)
	{
		// Drop ammo
		m_Pickups.emplace_back(PickupType::AMMO, position);
		std::cout << "Ammo pickup spawned at (" << position.x << ", " << position.y << ")" << std::endl;
	}
	// Otherwise, drop nothing
}

void Game::DrawPickups() const
{
	for (const auto& pickup : m_Pickups)
	{
		pickup.Draw();
	}
}

void Game::DebugCheckEnemyBounds()
{
	std::cout << "\n=== Enemy Bounds Check ===" << std::endl;
	
	// Print cursor position
	std::cout << "Cursor Position: (" << m_TeleportTargetPos.x << ", " << m_TeleportTargetPos.y << ")" << std::endl;
	
	// Get level dimensions from viewport
	float minX = -5.f;
	float maxX = GetViewPort().width + 5.f;
	float minY = -5.f;
	float maxY = GetViewPort().height + 5.f;
	
	// Add a small buffer around the level boundaries
	const float buffer = 100.f;
	
	int totalEnemies = 0;
	int outOfBoundsEnemies = 0;
	
	for (const auto& npc : m_NPCManager.GetNPCVector())
	{
		if (npc->GetHealth() > 0) // Only check alive enemies
		{
			totalEnemies++;
			Vector2f centerPos = npc->GetCenterPos();
			
			bool isOutOfBounds = centerPos.x < minX - buffer || 
								centerPos.x > maxX + buffer || 
								centerPos.y < minY - buffer || 
								centerPos.y > maxY + buffer;
			
			if (isOutOfBounds)
			{
				outOfBoundsEnemies++;
				std::cout << "Enemy at (" << centerPos.x << ", " << centerPos.y << ") is OUT OF BOUNDS" << std::endl;
			}
			else
			{
				std::cout << "Enemy at (" << centerPos.x << ", " << centerPos.y << ") is in bounds" << std::endl;
			}
		}
	}
	
	std::cout << "\nSummary:" << std::endl;
	std::cout << "Total alive enemies: " << totalEnemies << std::endl;
	std::cout << "Out of bounds enemies: " << outOfBoundsEnemies << std::endl;
	std::cout << "Level boundaries: X[" << minX - buffer << " to " << maxX + buffer << "], Y[" << minY - buffer << " to " << maxY + buffer << "]" << std::endl;
	std::cout << "Viewport size: " << GetViewPort().width << "x" << GetViewPort().height << std::endl;
	std::cout << "====================\n" << std::endl;
}
