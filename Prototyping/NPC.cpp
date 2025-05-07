#include "pch.h"
#include "NPC.h"

NPC::NPC() :
	m_Health{ 10 },
	m_Bounds{0.f, 0.f, 50.f, 50.f},
	m_CurrentBulletType{ 0 },
	m_Team{Team::Neutral}
{
}

NPC::NPC(Team team, Vector2f pos) :
	m_Health{ 10 },
	m_Bounds{ pos.x, pos.y, 35.f, 35.f },
	m_CurrentBulletType{ 0 },
	m_Team{ team },
	m_CurrentAction{ Action::Idle },
	m_MaxMoveTime{ 2.f },
	m_CurrentMoveTime{ 0.f },
	m_MaxReloadTime{ 3.f },
	m_CurrentReloadTime{0.f}
{
}

void NPC::Draw() const
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
	utils::FillRect(m_Bounds);
}

bool NPC::Shoot(Vector2f targetPos)
{
	if (m_CurrentReloadTime > m_MaxReloadTime)
	{
		m_CurrentReloadTime = float(rand() % 11) / 10;
		return true;
	}
	else {
		return false;
	}
}

void NPC::Update(Vector2f targetPos, float elapsedSec, std::vector<std::vector<Vector2f>> levelVerticies)
{
	m_CurrentReloadTime += elapsedSec;
	float randAngleVariation{ float(rand() % 31- 15)};
	m_CurrentMoveTime += elapsedSec;
	targetPos.x -= GetPos().x;
	targetPos.y -= GetPos().y;
	float angle{ float(atan2(double((targetPos.y)),double((targetPos.x)))) };
	
	int strafeDirection{};
	if (m_CurrentMoveTime> m_MaxMoveTime)
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
			targetPos.x = float(cos(double(angle))) * 150;
			targetPos.y = float(sin(double(angle))) * 150;

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
	m_Health += delta;
}

Team NPC::GetTeam()
{
	return m_Team;
}
