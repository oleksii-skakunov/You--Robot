#include "pch.h"
#include "NPCManager.h"

NPCManager::NPCManager() : m_NPCs{}
{
}

NPCManager::~NPCManager()
{
	DeleteAll();
}

void NPCManager::Draw() const
{
	for (int i = 0; i < m_NPCs.size(); i++)
	{
		m_NPCs[i]->Draw();
	}
}

void NPCManager::Update(float elapsedSec, Vector2f targetPos, std::vector<std::vector<Vector2f>> levelVerticies)
{
	for (int i = 0; i < m_NPCs.size(); i++)
	{
		if (m_NPCs[i]->GetHealth() <= 0)
		{
			delete m_NPCs[i];
			m_NPCs[i] = nullptr;
		}
		else
		{
			m_NPCs[i]->SetLevelVertices(levelVerticies);
			m_NPCs[i]->Update(targetPos, elapsedSec, levelVerticies);
		}
	}
	m_NPCs.erase(std::remove(m_NPCs.begin(), m_NPCs.end(), nullptr), m_NPCs.end());
}

bool NPCManager::Shoot(Vector2f targetPos, int i)
{
	return m_NPCs[i]->Shoot(targetPos);
}

void NPCManager::AddNPC(int team, const Vector2f& addPos, std::vector<std::vector<Vector2f>>& targetVector)
{
	m_NPCs.push_back(new NPC{ static_cast<Team>(team), addPos });
	AddHitboxToVector(targetVector, m_NPCs.size() - 1);
}

void NPCManager::AddBossNPC(int team, const Vector2f& addPos, std::vector<std::vector<Vector2f>>& targetVector)
{
	m_NPCs.push_back(new NPC{ static_cast<Team>(team), addPos, true });
	AddHitboxToVector(targetVector, m_NPCs.size() - 1);
}

void NPCManager::UpdateVector(std::vector<std::vector<Vector2f>>& targetVector, Team team)
{
	targetVector.clear();
	for (int i = 0; i < m_NPCs.size(); i++)
	{
		if (m_NPCs[i]->GetTeam() == team)
		{
			AddHitboxToVector(targetVector, i);
		}
		
	}
}

void NPCManager::AddHitboxToVector(std::vector<std::vector<Vector2f>>& targetVector, int i)
{
	Vector2f BOTTOMLEFT{ m_NPCs[i]->GetBounds().left, m_NPCs[i]->GetBounds().bottom};
	Vector2f BOTTOMRIGHT{ m_NPCs[i]->GetBounds().left + m_NPCs[i]->GetBounds().width, m_NPCs[i]->GetBounds().bottom };
	Vector2f UPLEFT{ m_NPCs[i]->GetBounds().left, m_NPCs[i]->GetBounds().bottom + m_NPCs[i]->GetBounds().height };
	Vector2f UPRIGHT{ m_NPCs[i]->GetBounds().left + m_NPCs[i]->GetBounds().width, m_NPCs[i]->GetBounds().bottom + m_NPCs[i]->GetBounds().height };

	targetVector.push_back(std::vector<Vector2f>{BOTTOMLEFT, BOTTOMRIGHT, UPRIGHT, UPLEFT});
}

void NPCManager::DeleteAll()
{
	for (int i = 0; i < m_NPCs.size(); i++)
	{
		delete m_NPCs[i];
		m_NPCs[i] = nullptr;
	}
	m_NPCs.clear();
}

const std::vector<NPC*>& NPCManager::GetNPCVector() const
{
	return m_NPCs;
}
