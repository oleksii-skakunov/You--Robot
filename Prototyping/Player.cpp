#include "pch.h"
#include "Player.h"
#include <iostream>

Player::Player() :
	m_Health{ 10 },
	m_Spritesheet{ "PlayerSpritesheet.png" },
	m_CurrentBulletType{ 0 },
	m_Bounds{ 100.f,100.f, 17.f * 2.5, 16.f * 2.5 },
	m_Law_NoHarmNonTarget{ 1 },
	m_State{State::Idle},
	m_FrameTimer{1.f/24.f},
	m_CurrentFrameTime{0.f},
	m_CurrentFrame{ 0.f, 0.f, 17.f, 16.f },
	m_IsDead{0},
	m_IsVisible{true},
	m_IsLookingLeft{false}
{
}

void Player::Draw() const
{
	if (m_IsVisible)
	{
		if (m_IsLookingLeft)
		{
			
		}
		
		m_Spritesheet.Draw(m_Bounds, m_CurrentFrame);
	}
}

void Player::DebugDraw() const
{
	utils::SetColor(Color4f{ 0.f,1.f,0.f,1.f });
	utils::FillRect(m_Bounds);
}

bool Player::Shoot(Vector2f mousePos, std::vector<std::vector<Vector2f>> targetVector, std::vector<std::vector<Vector2f>> levelVector, std::vector<std::vector<Vector2f>> nonTargetVector)
{
	if (not m_IsDead)
	{
		if (m_Law_NoHarmNonTarget)
		{
			float angle{ float(atan2(double((mousePos.y - GetCenterPos().y)),double((mousePos.x - GetCenterPos().x)))) };
			for (float i = angle - 0.01f; i <= angle + 0.01f; i += 0.002f) // goofy aah fix to make sure that we won't shoot sth that we shouldn't
				// basically we check a cone instead of a line
			{
				mousePos.x = mousePos.x + float(cos(double(angle))) * 9001;
				mousePos.y = mousePos.y + float(sin(double(angle))) * 9001;
				bool tempBool{ 1 };
				while (tempBool)
				{
					for (int i = 0; i < nonTargetVector.size(); i++)
					{
						if (not utils::Raycast(nonTargetVector[i], GetCenterPos(), mousePos, m_HitInfo))
						{
							tempBool = false;
						}
						else
						{
							if (m_HitInfo.intersectPoint.x > GetCenterPos().x)
							{
								mousePos.x = m_HitInfo.intersectPoint.x - 1.f;
							}
							else
							{
								mousePos.x = m_HitInfo.intersectPoint.x + 1.f;
							}
							if (m_HitInfo.intersectPoint.y > GetCenterPos().y)
							{
								mousePos.y = m_HitInfo.intersectPoint.y - 1.f;
							}
							else
							{
								mousePos.y = m_HitInfo.intersectPoint.y + 1.f;
							}
						}
					}
					if (nonTargetVector.empty())
					{
						tempBool = false;
					}
				}
				for (int i = 0; i < targetVector.size(); i++)
				{
					if (utils::Raycast(targetVector[i], GetCenterPos(), mousePos, m_HitInfo))
					{
						std::cout << "DEBUG: shot allowed - enemy was detected\n";
						return true;
					}
				}
				for (int i = 0; i < levelVector.size(); i++)
				{
					if (utils::Raycast(levelVector[i], GetCenterPos(), mousePos, m_HitInfo))
					{
						std::cout << "DEBUG: shot allowed - wall was detected\n";
						return true;
					}
				}

			}
			std::cout << "DEBUG: shot denied - looking at nontarget\n";
			return false;
		}
		else
		{
			std::cout << "DEBUG: shot allowed - law disabled\n";
			return true;
		}
	}
	else
	{
		std::cout << "DEBUG: shot denied - player is dead\n";
		return false;
	}

	
	

	// check if has enough ammo and follows the laws
	// how to check what we are aiming at:
	// draw a line from player to the position of cursor times 2
	// keep a list of verticies of forbidden targets
	// check for colisions
	// no colisions - no problem
	// if colisions are present, save the interesction position and check the list of all other targets
	// if there are no other colisions closer than the saved, do not shoot
	// if there are other colisions - shoot
}



void Player::Update(Vector2f velocity, float elapsedSec, std::vector<std::vector<Vector2f>> levelVerticies)
{
	if (!m_IsDead)
	{
		if (velocity.x == 0.f && velocity.y == 0.f)
		{
			m_State = State::Idle;
		}
		else
		{
			m_State = State::Walk;
		}

		// сalc proposed movement
		Vector2f proposedMovement = velocity * elapsedSec;
		
		// if we have a horizontal component...
		if (proposedMovement.x != 0)
		{
			Rectf newBounds = m_Bounds;
			newBounds.left += proposedMovement.x;
			
			// create a bool to store information if we would collide
			bool wouldCollide = false;
			
			// calc newBounds corners
			Vector2f newBottomLeft{ newBounds.left, newBounds.bottom };
			Vector2f newBottomRight{ newBounds.left + newBounds.width, newBounds.bottom };
			Vector2f newTopLeft{ newBounds.left, newBounds.bottom + newBounds.height };
			Vector2f newTopRight{ newBounds.left + newBounds.width, newBounds.bottom + newBounds.height };
			
			// check each corner against each obstacle
			for (const auto& obstacle : levelVerticies)
			{
				
				// if a corner is inside any boundary...
				if (utils::IsPointInPolygon(newBottomLeft, obstacle) ||
					utils::IsPointInPolygon(newBottomRight, obstacle) ||
					utils::IsPointInPolygon(newTopLeft, obstacle) ||
					utils::IsPointInPolygon(newTopRight, obstacle))
				{
					wouldCollide = true; // we must've collided
					break;
				}
			}
			
			if (!wouldCollide) // since we would've collided, we shouldn't move horizontaly
			{
				m_Bounds.left = newBounds.left;
			}
		}

		// Then try vertical movement
		if (proposedMovement.y != 0)
		{
			Rectf newBounds = m_Bounds;
			newBounds.bottom += proposedMovement.y;
			
			// Check if new position would cause collision
			bool wouldCollide = false;
			
			// Get corners of the new position
			Vector2f newBottomLeft{ newBounds.left, newBounds.bottom };
			Vector2f newBottomRight{ newBounds.left + newBounds.width, newBounds.bottom };
			Vector2f newTopLeft{ newBounds.left, newBounds.bottom + newBounds.height };
			Vector2f newTopRight{ newBounds.left + newBounds.width, newBounds.bottom + newBounds.height };
			
			// Check each corner against all obstacles
			for (const auto& obstacle : levelVerticies)
			{
				
				
				if (utils::IsPointInPolygon(newBottomLeft, obstacle) ||
					utils::IsPointInPolygon(newBottomRight, obstacle) ||
					utils::IsPointInPolygon(newTopLeft, obstacle) ||
					utils::IsPointInPolygon(newTopRight, obstacle))
				{
					wouldCollide = true;
					break;
				}
			}
			
			// If no collision, apply vertical movement
			if (!wouldCollide)
			{
				m_Bounds.bottom = newBounds.bottom;
			}
		}
	}

	m_CurrentFrameTime += elapsedSec;
	if (m_CurrentFrameTime >= m_FrameTimer)
	{
		NextFrame();
		m_CurrentFrameTime = 0;
	}
	
	if (m_Health <= 0)
	{
		m_IsDead = 1;
		m_State = State::Dead;
	}
}

void Player::NextFrame()
{
	switch (m_State)
	{
	case Player::State::Idle:
		if (m_CurrentFrame.bottom == 0.f)
		{
			m_CurrentFrame.left = m_CurrentFrame.left + 18.f;

		}
		else
		{
			m_CurrentFrame = Rectf{ 0.f, 0.f, 17.f, 16.f };
			m_Bounds.width = m_CurrentFrame.width * 2.5 ;
			m_Bounds.height = m_CurrentFrame.height * 2.5;
		}
		break;
	case Player::State::Walk:
		if (m_CurrentFrame.bottom == 17.f)
		{
			m_CurrentFrame.left = (int(m_CurrentFrame.left) + 17) % (102);
		}
		else
		{
			m_CurrentFrame = Rectf{ 0.f, 17.f, 16.f, 17.f };
			m_Bounds.width = m_CurrentFrame.width * 2.5;
			m_Bounds.height = m_CurrentFrame.height * 2.5;
		}
		break;
	case Player::State::Hurt:
		// fix this, it doesn't work rn
		if (m_CurrentFrame.bottom == 35.f)
		{
			if (m_CurrentFrame.left == 19.f)
			{
				m_CurrentFrame = Rectf{ 0.f, 0.f, 17.f, 16.f };
				m_State = State::Idle;
			}
			else
			{
				m_CurrentFrame.left = 19.f;
			}
			
		}
		else
		{
			m_CurrentFrame = Rectf{ 0.f, 35.f, 18.f, 17.f };
		}
		break;
	case Player::State::Dead:
		if (m_CurrentFrame.bottom == 53.f)
		{
			if (m_CurrentFrame.left == 18.f)
			{
				m_CurrentFrame = Rectf{ 36.f, 53.f, 17.f, 14.f };
			}
			else if (m_CurrentFrame.left == 36.f)
			{
				m_CurrentFrame = Rectf{ 36.f, 53.f, 17.f, 14.f };
			}
			else {
				m_CurrentFrame = Rectf{ 18.f, 53.f, 17.f, 14.f };
			}
		}
		else
		{
			m_CurrentFrame = Rectf{ 0.f, 53.f, 17.f, 14.f };
		}
		break;
	default:
		break;
	}
}

int Player::GetBulletType()
{
	return m_CurrentBulletType;
}

Vector2f Player::GetPos()
{
	return Vector2f{ m_Bounds.left, m_Bounds.bottom };
}

Rectf Player::GetBounds() const
{
	return m_Bounds;
}

Vector2f Player::GetCenterPos()
{
	return Vector2f{ m_Bounds.left + (m_Bounds.width / 2), m_Bounds.bottom + (m_Bounds.height / 2) };
}

void Player::DebugSetLaw()
{
	m_Law_NoHarmNonTarget = not m_Law_NoHarmNonTarget;
	if (m_Law_NoHarmNonTarget)
	{
		std::cout << "DEBUG: NoHarmNonTarget Law enabled" << std::endl;
	}
	else
	{
		std::cout << "DEBUG: NoHarmNonTarget Law disabled" << std::endl;
	}
}

void Player::SetHealth(int delta)
{
	m_Health += delta;
}

int Player::GetHealth() const
{
	return m_Health;
}

void Player::SetPosition(Vector2f newPos)
{
	m_Bounds.left = newPos.x - (m_Bounds.width / 2);
	m_Bounds.bottom = newPos.y - (m_Bounds.height / 2);
}

void Player::SetVisible(bool isVisible)
{
	m_IsVisible = isVisible;
	if (!isVisible)
	{
		// Move player off-screen when hiding
		m_Bounds.left = -1000.f;
		m_Bounds.bottom = -1000.f;
	}
}

void Player::Reset()
{
	m_Health = 10;
	m_IsDead = false;
	m_State = State::Idle;
	m_CurrentFrame = Rectf{ 0.f, 0.f, 17.f, 16.f };
	m_CurrentFrameTime = 0.f;
	m_IsVisible = false;
	m_Bounds = Rectf{ 100.f, 100.f, 17.f * 2.5, 16.f * 2.5 };
}
