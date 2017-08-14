/* -------------------------------------------------------------
-	DefendAction.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for player defence animations.
-
-	Class use pe_action_move to move player according to
-	velocity. And rotate player according relative movemement.
-
-  -------------------------------------------------------------
-	Creation Date: 31.07.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#include "DefendAction.h"

#include "Actor/Actor.h"
#include "AI/AIEnemy.h"
#include "AI/States/AIStateManager.h"

CDefendAction::CDefendAction(
	CActor * pActor, CAIEnemy * pEnemy, bool isRotateForce,
	int priority, FragmentID fragmentID, const TagState &fragTags,
	uint32 flags, ActionScopes scopeMask, uint32 userToken)
	:
	BaseClass(pActor, m_bIsRotateForce, priority, fragmentID, fragTags, flags, scopeMask, userToken)
	, m_pEnemy(pEnemy)
{
}

IAction::EStatus CDefendAction::Update(float timePassed)
{
	assert(m_pActor);
	assert(m_pEnemy);

	return BaseClass::Update(timePassed);
}

void CDefendAction::Enter()
{
	BaseClass::Enter();

	assert(m_pEnemy);	

	m_pEnemy->GetStateManager()->ChangeState(EAIStates::Slave);

	CryLog("Defend action enter!!!!");
}

void CDefendAction::Exit()
{
	BaseClass::Exit();

	m_pEnemy->GetStateManager()->ChangeState(EAIStates::Chase);

	m_pActor->BlockMove(false);

	m_pActor->ReleaseSlave();

	CryLog("End of defend action!!!");
}
