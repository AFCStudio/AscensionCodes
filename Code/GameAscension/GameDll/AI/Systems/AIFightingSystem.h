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

#ifndef __AI_FIGHTING_SYSTEM_H__
#define __AI_FIGHTING_SYSTEM_H__

#pragma once

#include "AI/States/AIStates.h"
#include "Entities/Helpers/ISimpleExtension.h"

class CAIBase;
class CAIEnemy;

using EnemyVector = std::vector<CAIEnemy*>;
using EnemyQueue = std::queue<CAIEnemy*>;

////////////////////////////////////////////////////////
// Player extension to manage player states
////////////////////////////////////////////////////////
class CAIFightingSystem
{
public:

	CAIFightingSystem();
	virtual ~CAIFightingSystem() {};

	// ISimpleExtension
	virtual void PostInit(IGameObject* pGameObject);

	virtual void Update(SEntityUpdateContext& ctx, int updateSlot);
	// ~ISimpleExtension

	void AddFighter(CAIEnemy * pEnemy);
	void RemoveFighter(CAIEnemy * pEnemy);

	void AddAttackGroup(CAIEnemy * pEnemy);
	void ChangeToAttackGroup(CAIEnemy * pEnemy);

	void AddTauntGroup(CAIEnemy * pEnemy);
	void ChangeToTauntGroup(CAIEnemy * pEnemy);

	CAIEnemy * PopAttacking();
	void PushAttacking(CAIEnemy * pEnemy);

private:

	void FindBestAttackGroupCandidate();

private:

	EnemyVector m_attackGroup;
	EnemyVector m_tauntGroup;

	EnemyQueue	m_attackers;
};

#endif // !__AI_FIGHTING_SYSTEM_H__