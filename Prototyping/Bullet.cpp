#include "pch.h"
#include "Bullet.h"



Bullet::Bullet(float traveltime, int damage, const Vector2f startPos, Vector2f velocityVector, const Texture* pTexture, float angle, Team team):
	m_Lifetime{ 0 },
	m_Traveltime{ traveltime },
	m_Damage{ damage },
	m_Pos{ startPos },
	m_VelocityVector{ velocityVector },
	m_pTexture{ pTexture },
	m_Angle{ angle * 57.29f },
	// also rewrite the update function so that we don't recalculate the velocityVector every frame
	m_HasStopped{ 0 },
	m_Team{static_cast<Team>(team)}
{ 
}

Bullet::~Bullet()
{
}

void Bullet::Draw() const
{
	glPushMatrix();
	glTranslatef(m_Pos.x, m_Pos.y, 0);
	glRotatef(m_Angle + 90, 0, 0, 1); // COUNTERCLOCKWISE!!!
	m_pTexture->Draw();
	glPopMatrix();
}

void Bullet::Update(float elapsedSec, NPCManager& npcManager, Player& player, HudManager& hudManager, std::vector<std::vector<Vector2f>> levelVerticies)
{
	Rectf l_BulletHitbox{ m_Pos.x, m_Pos.y, 1.f, 1.f };
	//std::vector<std::vector<Vector2f>> l_NPCVertices{};
	//for (int i = 0; i < npcManager.GetNPCVector().size(); i++)
	//{
	//	l_NPCVertices.push_back(std::vector<Vector2f>{Vector2f{ npcManager.GetNPCVector()[i]->GetBounds().left, npcManager.GetNPCVector()[i]->GetBounds().bottom }});
	//	l_NPCVertices[i].push_back(Vector2f{ npcManager.GetNPCVector()[i]->GetBounds().left + npcManager.GetNPCVector()[i]->GetBounds().width, npcManager.GetNPCVector()[i]->GetBounds().bottom });
	//	l_NPCVertices[i].push_back(Vector2f{ npcManager.GetNPCVector()[i]->GetBounds().left + npcManager.GetNPCVector()[i]->GetBounds().width, npcManager.GetNPCVector()[i]->GetBounds().bottom + npcManager.GetNPCVector()[i]->GetBounds().height });
	//	l_NPCVertices[i].push_back(Vector2f{ npcManager.GetNPCVector()[i]->GetBounds().left , npcManager.GetNPCVector()[i]->GetBounds().bottom + npcManager.GetNPCVector()[i]->GetBounds().height });


	//}
	utils::HitInfo l_HitInfo{};

	
	switch (m_Team)
	{
	case Team::Neutral:
		
		
		for (int i = 0; i < npcManager.GetNPCVector().size(); i++)
		{
			if (l_BulletHitbox.left < npcManager.GetNPCVector()[i]->GetBounds().left + npcManager.GetNPCVector()[i]->GetBounds().width &&
				l_BulletHitbox.left + l_BulletHitbox.width > npcManager.GetNPCVector()[i]->GetBounds().left &&
				l_BulletHitbox.bottom < npcManager.GetNPCVector()[i]->GetBounds().bottom + npcManager.GetNPCVector()[i]->GetBounds().height &&
				l_BulletHitbox.bottom + l_BulletHitbox.height > npcManager.GetNPCVector()[i]->GetBounds().bottom)
			{
				npcManager.GetNPCVector()[i]->AddHealth(m_Damage * -1);
				m_HasStopped = 1;
			}

		}
		break;
	case Team::NonTarget:
		break;
	case Team::Target:
		
		if (l_BulletHitbox.left < player.GetBounds().left + player.GetBounds().width &&
			l_BulletHitbox.left + l_BulletHitbox.width > player.GetBounds().left &&
			l_BulletHitbox.bottom < player.GetBounds().bottom + player.GetBounds().height &&
			l_BulletHitbox.bottom + l_BulletHitbox.height > player.GetBounds().bottom)
		{
			player.SetHealth(m_Damage * - 1);
			hudManager.UpdateHealth(0.f, m_Damage * -1);
			m_HasStopped = 1;
		}
		break;
	default:
		break;
	}
	for (int i = 0; i < levelVerticies.size(); i++)
	{
		if (utils::Raycast(levelVerticies[i], Vector2f{ l_BulletHitbox.left, l_BulletHitbox.bottom }, Vector2f{ l_BulletHitbox.left + m_VelocityVector.x * elapsedSec, l_BulletHitbox.bottom + m_VelocityVector.y * elapsedSec }, l_HitInfo))
		{
			m_HasStopped = 1;
		}
	}
	if (m_Lifetime >= m_Traveltime)
	{
		m_HasStopped = 1;
	}
	else {
		m_Lifetime += elapsedSec;
		m_Pos.x += m_VelocityVector.x * elapsedSec;
		m_Pos.y += m_VelocityVector.y * elapsedSec;
	}
	
}

bool Bullet::HasStopped() const
{
	return m_HasStopped;
}

bool Bullet::lineSegmentIntersect(Vector2f p1, Vector2f p2, Vector2f p3, Vector2f p4)
{
	float epsilon{ 1e-8 };
	Vector2f directionVector1{ p1, p2 };
	Vector2f directionVector2{ p3, p4 };
	float denominator{ directionVector1.x * directionVector2.y - directionVector2.x * directionVector1.y };
	if (denominator < epsilon)
	{
		return false;
	}
	return false;
}
