#include "pch.h"
#include "NPC.h"

NPC::NPC() :
	m_Health{ 10 },
	m_Bounds{0.f, 0.f, 50.f, 50.f},
	m_CurrentBulletType{ 0 },
	m_Team{Team::Neutral},
	m_IsBoss{false},
	m_SpawnDelay{0.f},
	m_BurstTimer{0.f},
	m_BulletsInBurst{0},
	m_TargetPosition{0.f, 0.f},
	m_HasReachedTarget{false},
	m_MoveSpeed{200.f},
	m_LastKnownPlayerPos{0.f, 0.f},
	m_IsFiringBurst{false},
	m_FlashTimer{0.f},
	m_PreviousHealth{10},
	m_BossState{BossState::Idle},
	m_ActionTimer{0.f},
	m_DashDistance{0.f},
	m_DashStartPos{0.f, 0.f},
	m_DashDirection{0.f, 0.f},
	m_BurstTargetPos{0.f, 0.f},
	m_DashCooldown{0.f}
{
}

NPC::NPC(Team team, Vector2f pos, bool isBoss) :
	m_Health{ isBoss ? 50 : 10 },
	m_Bounds{ pos.x, pos.y, isBoss ? 87.5f : 35.f, isBoss ? 87.5f : 35.f },
	m_CurrentBulletType{ 0 },
	m_Team{ team },
	m_CurrentAction{ Action::Idle },
	m_MaxMoveTime{ 2.f },
	m_CurrentMoveTime{ 0.f },
	m_InitialDelay{ 0.3f },
	m_MaxReloadTime{ 3.f },
	m_CurrentReloadTime{0.f},
	m_IsBoss{isBoss},
	m_SpawnDelay{0.f},
	m_BurstTimer{0.f},
	m_BulletsInBurst{0},
	m_TargetPosition{0.f, 0.f},
	m_HasReachedTarget{false},
	m_MoveSpeed{200.f},
	m_LastKnownPlayerPos{0.f, 0.f},
	m_IsFiringBurst{false},
	m_FlashTimer{0.f},
	m_PreviousHealth{isBoss ? 50 : 10},
	m_BossState{isBoss ? BossState::Entering : BossState::Idle},
	m_ActionTimer{0.f},
	m_DashDistance{0.f},
	m_DashStartPos{pos},
	m_DashDirection{0.f, 0.f},
	m_BurstTargetPos{0.f, 0.f},
	m_DashCooldown{0.f}
{
	if (isBoss)
	{
		// Calculate direction to target position
		Vector2f direction = m_TargetPosition - pos;
		float length = sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length > 0)
		{
			m_DashDirection = Vector2f{ direction.x / length, direction.y / length };
		}
		m_DashDistance = 400.f; // Set dash distance for entrance
		m_ActionTimer = 0.5f; // Set dash duration
	}
	std::cout << "NPC created - IsBoss: " << m_IsBoss << ", Position: (" << pos.x << ", " << pos.y << ")" << std::endl;
}

void NPC::Draw() const
{
	if (!m_IsBoss || ShouldSpawn())
	{
		if (m_FlashTimer > 0.f)
		{
			utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f }); // White flash
		}
		else
		{
			switch (m_Team)
			{
			case Team::Neutral:
				utils::SetColor(Color4f{ 1.f,1.f,1.f,1.f });
				break;
			case Team::NonTarget:
				utils::SetColor(Color4f{ 0.f,0.f,1.f,1.f });
				break;
			case Team::Target:
				utils::SetColor(Color4f{ 1.f,0.f,0.f,1.f });
				break;
			default:
				break;
			}
			if (m_IsBoss)
			{
				utils::SetColor(Color4f{ 1.f,1.f,0.f,1.f }); // Yellow for boss
			}
		}
		utils::FillRect(m_Bounds);
	}
}

bool NPC::HasLineOfSight(const Vector2f& targetPos, const std::vector<std::vector<Vector2f>>& obstacles) const
{
	Vector2f npcCenter = GetCenterPos();
	utils::HitInfo hitInfo;

	// Check if there are any obstacles between NPC and target
	for (const auto& obstacle : obstacles)
	{
		// Skip the level boundary (first obstacle)
		if (&obstacle == &obstacles[0]) continue;

		if (utils::Raycast(obstacle, npcCenter, targetPos, hitInfo))
		{
			return false; // Obstacle found in line of sight
		}
	}
	return true; // No obstacles found
}

bool NPC::Shoot(Vector2f targetPos)
{
	// Check line of sight before allowing shooting
	if (!HasLineOfSight(targetPos, m_LevelVertices))
	{
		return false;
	}

	if (m_IsBoss)
	{
		if (m_BossState == BossState::Bursting)
		{
			if (m_BulletsInBurst < 10 && m_BurstTimer <= 0.f)
			{
				m_BurstTimer = 0.15f;
				m_BulletsInBurst++;
				return true;
			}
		}
		return false;
	}
	else
	{
		if (m_CurrentReloadTime > m_MaxReloadTime)
		{
			m_CurrentReloadTime = float(rand() % 11) / 10;
			return true;
		}
		return false;
	}
}

void NPC::Update(Vector2f targetPos, float elapsedSec, std::vector<std::vector<Vector2f>> levelVerticies)
{
	if (m_FlashTimer > 0.f)
	{
		m_FlashTimer -= elapsedSec;
	}

	// Check if NPC is out of bounds (only if not dashing)
	if ((m_IsBoss && m_BossState != BossState::Dashing && m_BossState != BossState::Entering) || !m_IsBoss)
	{
		Vector2f centerPos = GetCenterPos();
		// Get level dimensions from viewport
		float minX = -5.f;
		float maxX = 1080.f + 5.f; // Viewport width
		float minY = -5.f;
		float maxY = 720.f + 5.f; // Viewport height
		
		// Add buffer around boundaries
		const float buffer = 100.f;
		
		if (centerPos.x < minX - buffer || centerPos.x > maxX + buffer ||
			centerPos.y < minY - buffer || centerPos.y > maxY + buffer)
		{
			std::cout << "NPC killed - Out of bounds at position: (" << centerPos.x << ", " << centerPos.y << ")" 
				<< " State: " << (m_IsBoss ? "Boss" : "Normal") 
				<< " BossState: " << static_cast<int>(m_BossState) << std::endl;
			m_Health = 0; // Kill the NPC
			return;
		}
	}

	if (m_IsBoss)
	{
		switch (m_BossState)
		{
		case BossState::Entering:
		case BossState::Dashing:
			m_ActionTimer -= elapsedSec;
			if (m_ActionTimer <= 0.f)
			{
				if (m_BossState == BossState::Entering)
				{
					m_BossState = BossState::Idle;
				}
				else
				{
					m_BossState = BossState::PostDash; // Transition to post-dash delay
					m_ActionTimer = 2.f; // Set 2 second delay
				}
			}
			else
			{
				// Move in dash direction
				float dashSpeed = m_DashDistance / 0.5f; // 400 units in 0.5 seconds
				m_Bounds.left += m_DashDirection.x * dashSpeed * elapsedSec;
				m_Bounds.bottom += m_DashDirection.y * dashSpeed * elapsedSec;
			}
			break;

		case BossState::PostDash:
			m_ActionTimer -= elapsedSec;
			if (m_ActionTimer <= 0.f)
			{
				m_BossState = BossState::Idle;
				m_ActionTimer = 2.f; // Wait 2 seconds before next action
			}
			break;

		case BossState::Idle:
			m_ActionTimer -= elapsedSec;
			if (m_IsBoss)
			{
				m_DashCooldown -= elapsedSec;
			}
			if (m_ActionTimer <= 0.f)
			{
				// Choose next action
				int action = rand() % 3; // 0: Dash, 1: Summon, 2: Burst
				switch (action)
				{
				case 0: // Dash
					if (m_DashCooldown <= 0.f) // Only dash if cooldown is ready
					{
						m_BossState = BossState::Dashing;
						m_ActionTimer = 0.5f;
						// Calculate dash direction towards player
						{
							Vector2f direction = targetPos - GetCenterPos();
							float length = sqrt(direction.x * direction.x + direction.y * direction.y);
							if (length > 0)
							{
								m_DashDirection = Vector2f{ direction.x / length, direction.y / length };
							}
						}
						m_DashDistance = 300.f; // Reduced from 400 to 300
						m_DashCooldown = 10.f; // Set 10 second cooldown
					}
					else
					{
						// If dash is on cooldown, choose a different action
						m_ActionTimer = 0.1f; // Short delay before trying again
					}
					break;

				case 1: // Summon
					m_BossState = BossState::Summoning;
					m_ActionTimer = 0.5f;
					// Summon 3 minions
					if (m_SummonCallback)
					{
						for (int i = 0; i < 3; ++i)
						{
							// Calculate spawn position outside the arena
							float angle = float(i) * 120.f * 3.14159f / 180.f; // 120 degrees apart
							Vector2f spawnPos = GetCenterPos() + Vector2f{ cos(angle) * 200.f, sin(angle) * 200.f };
							Vector2f targetPos = GetCenterPos() + Vector2f{ cos(angle) * 50.f, sin(angle) * 50.f };
							m_SummonCallback(spawnPos, targetPos);
						}
					}
					break;

				case 2: // Burst
					m_BossState = BossState::Bursting;
					m_BurstTargetPos = targetPos; // Store initial target position
					m_LastKnownPlayerPos = targetPos; // Update last known position when starting burst
					m_IsFiringBurst = true;
					m_BulletsInBurst = 0;
					m_BurstTimer = 0.15f;
					break;
				}
			}
			break;

		case BossState::Summoning:
			m_ActionTimer -= elapsedSec;
			if (m_ActionTimer <= 0.f)
			{
				m_BossState = BossState::Idle;
				m_ActionTimer = 2.f; // Wait 2 seconds before next action
			}
			break;

		case BossState::Bursting:
			m_BurstTimer -= elapsedSec;
			if (m_BulletsInBurst >= 10)
			{
				m_BossState = BossState::Idle;
				m_ActionTimer = 2.f;
				m_IsFiringBurst = false;
			}
			break;
		}
		return;
	}

	m_CurrentReloadTime += elapsedSec;
	if (m_IsBoss)
	{
		m_BurstTimer -= elapsedSec;
		if (m_BulletsInBurst > 0)
		{
			std::cout << "Boss firing burst: " << m_BulletsInBurst << "/10 bullets" << std::endl;
		}
	}

	float randAngleVariation{ float(rand() % 31- 15)};
	m_CurrentMoveTime += elapsedSec;
	targetPos.x -= GetPos().x;
	targetPos.y -= GetPos().y;
	float angle{ float(atan2(double((targetPos.y)),double((targetPos.x)))) };
	
	int strafeDirection{};
	if (m_CurrentMoveTime > (m_CurrentAction == Action::Idle ? m_InitialDelay : m_MaxMoveTime))
	{
		m_CurrentAction = Action::Idle;
		m_CurrentMoveTime = 0.f;
		m_CurrentAction = static_cast<Action>(rand() % 4);
		if (m_CurrentAction == Action::Strafe)
		{
			strafeDirection = rand() % 2 ;
		}
	}
	else
	{
		switch (m_Team)
		{
		case Team::Neutral:
			break;
		case Team::NonTarget:
			targetPos.x = float(cos(double(angle))) * 50;
			targetPos.y = float(sin(double(angle))) * 50;
			switch (m_CurrentAction)
			{
			case Action::Idle:
				targetPos = Vector2f{ 0.f,0.f };
				break;
			case Action::Push:
				targetPos = Vector2f{ 0.f,0.f };
				break;
			case Action::Run:
				targetPos = utils::RotateVector(targetPos, randAngleVariation + 180);
				break;
			case Action::Strafe:
				targetPos = Vector2f{ 0.f,0.f };
				break;
			default:
				break;
			}
			break;
		case Team::Target:
			targetPos.x = float(cos(double(angle))) * (m_IsBoss ? 200.f : 150.f);
			targetPos.y = float(sin(double(angle))) * (m_IsBoss ? 200.f : 150.f);

			switch (m_CurrentAction)
			{
			case Action::Idle:
				targetPos = Vector2f{ 0.f,0.f };
				break;
			case Action::Push:
				targetPos = utils::RotateVector(targetPos, randAngleVariation);
				break;
			case Action::Run:
				targetPos = utils::RotateVector(targetPos, randAngleVariation + 180);
				break;
			case Action::Strafe:
				if (strafeDirection == 1)
				{
					targetPos = utils::RotateVector(targetPos, randAngleVariation + 90);
				}
				else
				{
					targetPos = utils::RotateVector(targetPos, randAngleVariation - 90);
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	// Only check collisions if not a boss or if boss has reached its target
	if (!m_IsBoss || m_HasReachedTarget)
	{
		// Calculate proposed movement
		Vector2f proposedMovement = targetPos * elapsedSec;
		
		// Get NPC corners
		Vector2f bottomLeft{ m_Bounds.left, m_Bounds.bottom };
		Vector2f bottomRight{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom };
		Vector2f topLeft{ m_Bounds.left, m_Bounds.bottom + m_Bounds.height };
		Vector2f topRight{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + m_Bounds.height };

		// Cast rays from each corner in the direction of movement
		utils::HitInfo hitInfo;
		bool hasCollision = false;

		// Check horizontal movement
		if (proposedMovement.x != 0)
		{
			// Cast rays from both left and right sides
			Vector2f rayStart1, rayStart2, rayEnd1, rayEnd2;
			
			if (proposedMovement.x > 0)
			{
				// Moving right - cast from right side
				rayStart1 = bottomRight;
				rayStart2 = topRight;
				rayEnd1 = rayStart1 + Vector2f{ proposedMovement.x, 0 };
				rayEnd2 = rayStart2 + Vector2f{ proposedMovement.x, 0 };
			}
			else
			{
				// Moving left - cast from left side
				rayStart1 = bottomLeft;
				rayStart2 = topLeft;
				rayEnd1 = rayStart1 + Vector2f{ proposedMovement.x, 0 };
				rayEnd2 = rayStart2 + Vector2f{ proposedMovement.x, 0 };
			}
			
			for (const auto& obstacle : levelVerticies)
			{
				if (utils::Raycast(obstacle, rayStart1, rayEnd1, hitInfo) || 
					utils::Raycast(obstacle, rayStart2, rayEnd2, hitInfo))
				{
					hasCollision = true;
					break;
				}
			}

			// Only move if no collision
			if (!hasCollision)
			{
				m_Bounds.left += proposedMovement.x;
			}
		}

		// Update corner positions after horizontal movement
		bottomLeft = Vector2f{ m_Bounds.left, m_Bounds.bottom };
		bottomRight = Vector2f{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom };
		topLeft = Vector2f{ m_Bounds.left, m_Bounds.bottom + m_Bounds.height };
		topRight = Vector2f{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + m_Bounds.height };

		// Reset collision flag for vertical check
		hasCollision = false;

		// Check vertical movement
		if (proposedMovement.y != 0)
		{
			// Cast rays from both bottom and top sides
			Vector2f rayStart1, rayStart2, rayEnd1, rayEnd2;
			
			if (proposedMovement.y > 0)
			{
				// Moving up - cast from top side
				rayStart1 = topLeft;
				rayStart2 = topRight;
				rayEnd1 = rayStart1 + Vector2f{ 0, proposedMovement.y };
				rayEnd2 = rayStart2 + Vector2f{ 0, proposedMovement.y };
			}
			else
			{
				// Moving down - cast from bottom side
				rayStart1 = bottomLeft;
				rayStart2 = bottomRight;
				rayEnd1 = rayStart1 + Vector2f{ 0, proposedMovement.y };
				rayEnd2 = rayStart2 + Vector2f{ 0, proposedMovement.y };
			}
			
			for (const auto& obstacle : levelVerticies)
			{
				if (utils::Raycast(obstacle, rayStart1, rayEnd1, hitInfo) || 
					utils::Raycast(obstacle, rayStart2, rayEnd2, hitInfo))
				{
					hasCollision = true;
					break;
				}
			}

			// Only move if no collision
			if (!hasCollision)
			{
				m_Bounds.bottom += proposedMovement.y;
			}
		}
	}
}

Vector2f NPC::GetShootingTarget() const
{
	if (m_IsBoss && m_BossState == BossState::Bursting)
	{
		return m_BurstTargetPos;
	}
	return Vector2f{0.f, 0.f};
}

void NPC::SetPosition(const Vector2f& newPos)
{
	// Simply set the position, collision checks are handled by TryTeleport
	m_Bounds.left = newPos.x - (m_Bounds.width / 2);
	m_Bounds.bottom = newPos.y - (m_Bounds.height / 2);
}

int NPC::GetBulletType()
{
	return m_CurrentBulletType;
}

Vector2f NPC::GetPos()
{
	return Vector2f{ m_Bounds.left , m_Bounds.bottom };
}

Rectf NPC::GetBounds()
{
	return m_Bounds;
}

int NPC::GetHealth()
{
	return m_Health;
}

void NPC::AddHealth(int delta)
{
	m_PreviousHealth = m_Health; // Store current health before change
	m_Health += delta;
	if (delta < 0) // If damage was taken
	{
		m_FlashTimer = 0.1f; // Set flash duration to 0.1 seconds
	}
}

Team NPC::GetTeam()
{
	return m_Team;
}

Vector2f NPC::GetCenterPos() const
{
	return Vector2f{ m_Bounds.left + m_Bounds.width / 2, m_Bounds.bottom + m_Bounds.height / 2 };
}
