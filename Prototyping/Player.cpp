#include "pch.h"
#include "Player.h"
#include <iostream>

Player::Player() :
	m_Health{ 10 },
	m_Texture{ "robot_3Dblue.png" },
	m_CurrentBulletType{ 0 },
	m_Bounds{ 0.f,0.f, 50.f, 50.f },
	m_Law_NoHarmNonTarget{ 1 }
{
}

void Player::Draw() const
{
	m_Texture.Draw(Rectf{m_Bounds.left, m_Bounds.bottom, 50.f, 50.f});
}

void Player::DebugDraw() const
{
	utils::SetColor(Color4f{ 0.f,1.f,0.f,1.f });
	utils::FillRect(m_Bounds);
}

bool Player::Shoot(Vector2f mousePos, std::vector<std::vector<Vector2f>> targetVector, std::vector<std::vector<Vector2f>> levelVector, std::vector<std::vector<Vector2f>> nonTargetVector)
{
	if (m_Law_NoHarmNonTarget)
	{
		float angle{ float(atan2(double((mousePos.y - GetCenterPos().y)),double((mousePos.x - GetCenterPos().x)))) };
		for (float i = angle - 0.01; i <= angle + 0.01; i+=0.002) // goofy aah fix to make sure that we won't shoot sth that we shouldn't
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
			std::cout << "DEBUG: shot denied\n";
			return false;
		}
		
	}
	else
	{
		std::cout << "DEBUG: shot allowed - law disabled\n";
		return true;
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



void Player::Update(Vector2f velocity, float elapsedSec)
{

	m_Bounds.left += velocity.x * elapsedSec; // move player
	m_Bounds.bottom += velocity.y * elapsedSec; // move player
	//Vector2f BOTTOMLEFT{ m_Bounds.left, m_Bounds.bottom }; // create points to be used later
	//Vector2f BOTTOMRIGHT{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom };
	//Vector2f UPLEFT{ m_Bounds.left, m_Bounds.bottom + m_Bounds.height };
	//Vector2f UPRIGHT{ m_Bounds.left + m_Bounds.width, m_Bounds.bottom + m_Bounds.height };
	//for (size_t i = 0; i < vertices.size(); i++)
	//{
	//	if (utils::Raycast(vertices[i], Vector2f{ BOTTOMLEFT.x, BOTTOMLEFT.y + 1 }, Vector2f{ BOTTOMRIGHT.x, BOTTOMRIGHT.y + 1 }, m_HitInfo) or // if horizonal lines between points defined above collide with any other lines...
	//		utils::Raycast(vertices[i], Vector2f{ UPLEFT.x, UPLEFT.y - 1 }, Vector2f{ UPRIGHT.x, UPRIGHT.y - 1 }, m_HitInfo))
	//	{
	//		if (m_Velocity.x > 0) // check direction by looking at horizontal velocity
	//		{
	//			m_Bounds.left = m_HitInfo.intersectPoint.x - m_Bounds.width; // if player moves to the right, stick his right side to the point of intersection
	//		}
	//		else if (m_Velocity.x < 0) {
	//			m_Bounds.left = m_HitInfo.intersectPoint.x; // otherwise stick his left side
	//		}
	//	}
	//	if (utils::Raycast(vertices[i], Vector2f{ BOTTOMLEFT.x, BOTTOMLEFT.y + 1 }, Vector2f{ BOTTOMLEFT.x, BOTTOMLEFT.y - 1 * m_Velocity.y }, m_HitInfo) or
	//		utils::Raycast(vertices[i], Vector2f{ BOTTOMRIGHT.x, BOTTOMRIGHT.y - 1 }, Vector2f{ BOTTOMRIGHT.x, BOTTOMRIGHT.y - 1 * m_Velocity.y }, m_HitInfo)) // check for floor
	//	{
	//		if (m_Velocity.y < 0) { // if there is floor below and we are falling,
	//			m_Velocity.y = 0; // stop
	//			m_Bounds.bottom = m_HitInfo.intersectPoint.y; // and land
	//		}

	//	}

	//}
	//if (m_Velocity.x != 0)
	//{
	//	m_Velocity.x = 0; // reset horizontal velocity to avoid sliding and other issues caused by this
	//}
}

int Player::GetBulletType()
{
	return m_CurrentBulletType;
}

Vector2f Player::GetPos()
{
	return Vector2f{ m_Bounds.left, m_Bounds.bottom };
}

Rectf Player::GetBounds()
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
