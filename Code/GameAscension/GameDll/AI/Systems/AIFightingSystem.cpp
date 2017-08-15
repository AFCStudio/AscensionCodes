// -------------------------------------------------------------
//	AIFighingtSystem.h - Header file
// -------------------------------------------------------------
//	Description:
//
//		Manage AI fightings against client players.
//
// -------------------------------------------------------------
//	Creation Date: 05.03.2017
//										Writed by Özkan Afacan
//		Copyright (C), AFC Studios
// -------------------------------------------------------------


#include "StdAfx.h"
#include "AIFightingSystem.h"
#include "AI/AIEnemy.h"

#include "Game/GameCVars.h"

CAIFightingSystem::CAIFightingSystem()
	: m_pTargetEnemy(nullptr)
	, m_targetAngle(0.0f)
{
}

void CAIFightingSystem::PostInit(IGameObject * pGameObject)
{
}

void CAIFightingSystem::Update(SEntityUpdateContext & ctx, int updateSlot)
{
}

CAIEnemy * CAIFightingSystem::IdentifyTargetEnemy(const CActor * pActor)
{
	Vec3 intendedDir = pActor->GetMoveDirection();

	if (intendedDir == ZERO)
	{
		intendedDir = pActor->GetLookDirection();
	}

	Vec3 targetDir;

	m_pTargetEnemy = nullptr;
	float minAngle = g_pGameCVars->pl_maxAttackAngle;

	for (auto pEnemy : m_attackGroup)
	{
		targetDir = pEnemy->GetEntity()->GetPos() - pActor->GetEntity()->GetPos();
		targetDir.z = 0.0f;
		targetDir.Normalize();

		float targetAngle = acos(intendedDir.dot(targetDir));

		if (targetAngle < minAngle)
		{
			m_pTargetEnemy = pEnemy;
			minAngle = targetAngle;
		}
	}

	for (auto pEnemy : m_tauntGroup)
	{
		targetDir = pEnemy->GetEntity()->GetPos() - pActor->GetEntity()->GetPos();
		targetDir.z = 0.0f;
		targetDir.Normalize();

		float targetAngle = acos(intendedDir.dot(targetDir));

		if (targetAngle < minAngle)
		{
			m_pTargetEnemy = pEnemy;
			minAngle = targetAngle;
		}
	}

	if (m_pTargetEnemy)
	{
		Vec3 actorDir = pActor->GetEntity()->GetForwardDir();
		m_targetAngle = acos(actorDir.dot(targetDir));
		m_targetAngle *= actorDir.cross(targetDir).z >= 0 ? 1.0f : -1.0f;
	}
	else
	{
		m_targetAngle = 0.0f;
	}

	return m_pTargetEnemy;
}

void CAIFightingSystem::AddFighter(CAIEnemy * pFighter)
{
	if (m_attackGroup.size() < g_pGameCVars->ai_maxAttackerCount)
	{
		AddAttackGroup(pFighter);
		pFighter->SetAsAttackGroup();
	}
	else
	{
		AddTauntGroup(pFighter);
		pFighter->SetAsTauntGroup();
	}
}

void CAIFightingSystem::RemoveFighter(CAIEnemy * pEnemy)
{
	if (pEnemy->IsAttackGroup())
	{
		m_attackGroup.erase(std::remove(m_attackGroup.begin(), m_attackGroup.end(), pEnemy), m_attackGroup.end());

		if (m_tauntGroup.size() > 0)
		{
			FindBestAttackGroupCandidate();
		}
	}
	else
	{
		m_tauntGroup.erase(std::remove(m_tauntGroup.begin(), m_tauntGroup.end(), pEnemy), m_tauntGroup.end());
	}
}

void CAIFightingSystem::AddAttackGroup(CAIEnemy * pEnemy)
{
	m_attackGroup.push_back(pEnemy);

	pEnemy->SetAsAttackGroup();
}

void CAIFightingSystem::ChangeToAttackGroup(CAIEnemy * pEnemy)
{
	m_tauntGroup.erase(std::remove(m_tauntGroup.begin(), m_tauntGroup.end(), pEnemy), m_tauntGroup.end());
	AddAttackGroup(pEnemy);
}

void CAIFightingSystem::AddTauntGroup(CAIEnemy * pEnemy)
{
	m_tauntGroup.push_back(pEnemy);

	pEnemy->SetAsTauntGroup();
}

void CAIFightingSystem::ChangeToTauntGroup(CAIEnemy * pEnemy)
{
	m_attackGroup.erase(std::remove(m_attackGroup.begin(), m_attackGroup.end(), pEnemy), m_attackGroup.end());
	AddTauntGroup(pEnemy);
}

CAIEnemy * CAIFightingSystem::PopAttacking()
{
	CAIEnemy * pEnemy = m_attacks.empty() ? nullptr : m_attacks.front();

	if (pEnemy)
	{
		m_attacks.pop_front();
		pEnemy->CancelAttack();
	}

	return pEnemy;
}

void CAIFightingSystem::PushAttacking(CAIEnemy * pEnemy)
{
	m_attacks.push_back(pEnemy);
}

void CAIFightingSystem::RemoveAttacking(CAIEnemy * pEnemy)
{
	m_attacks.erase(std::remove(m_attacks.begin(), m_attacks.end(), pEnemy), m_attacks.end());
}

void CAIFightingSystem::FindBestAttackGroupCandidate()
{
	int index = cry_random(0, (int)m_tauntGroup.size());
	
	CAIEnemy * pFighter = m_tauntGroup[index];
	m_tauntGroup.erase(m_tauntGroup.begin() + index);

	pFighter->SetAsAttackGroup();
	m_attackGroup.push_back(pFighter);
}


