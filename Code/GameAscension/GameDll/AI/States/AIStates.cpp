// -------------------------------------------------------------
//	AIStates.cpp - Source file
// -------------------------------------------------------------
//	Description:
//
//		Defines AI states for game.
//
// -------------------------------------------------------------
//	Creation Date: 03.03.2017
//										Writed by Özkan Afacan
//		Copyright (C), AFC Studios
// -------------------------------------------------------------

#include "StdAfx.h"
#include "AIStates.h"

#include "Game/GameCVars.h"

#include "AI/AIEnemy.h"

#include "Actor/Actor.h"

#include "Actions/ActionBase.h"

#include "AIStateManager.h"

// Idle State
void CAIStateIdle::Enter()
{
	m_pAIEnemy->ForceFinishLastAction();
	m_pAIEnemy->PlayFragment("MotionIdle", PP_Lowest, TAG_STATE_EMPTY);
}
void CAIStateIdle::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	// Temporarly fighting enterence code to check AI behaviors.
	// This section will be changed fihgting enterence behavior in the future.

	float fLenght = m_pAIEnemy->GetTargetDistance();

	if (fLenght < 8.0f && fLenght > 2.5f)
	{
		m_pAIEnemy->GetStateManager()->ChangeState(EAIStates::Chase);

		m_pAIEnemy->AddToAIFightingSystem();
	}
}
void CAIStateIdle::Exit()
{}
// -----------------------------------------------------

// Move State
void CAIStateMove::Enter()
{
	m_pAIEnemy->PlayMovementAction("MotionMovement", PP_Movement, TAG_STATE_EMPTY);

	m_pAIEnemy->ResetAttackTime();
}
void CAIStateMove::Update(SEntityUpdateContext& ctx, int updateSlot)
{
}
void CAIStateMove::Exit()
{
}
// -----------------------------------------------------

// Chase State
void CAIStateChase::Enter()
{
	m_pAIEnemy->PlayMovementAction("MotionMovement", PP_Movement, TAG_STATE_EMPTY);

	m_pAIEnemy->ResetAttackTime();
}
void CAIStateChase::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	float distance = m_pAIEnemy->GetStopDistance();

	if (distance <= g_pGameCVars->ai_distanceThreshold)
	{
		if (m_pAIEnemy->IsAttackGroup())
			m_pAIEnemy->GetStateManager()->ChangeState(EAIStates::Attack);
		else
			m_pAIEnemy->GetStateManager()->ChangeState(EAIStates::Taunt);
	}
}
void CAIStateChase::Exit()
{}
// -----------------------------------------------------

// Taunt Group State
void CAIStateTaunt::Enter()
{
	m_pAIEnemy->ForceFinishLastAction();
	m_pAIEnemy->PlayFragment("AI_Taunt", PP_PlayerAction, TAG_STATE_EMPTY);
}
void CAIStateTaunt::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	if (m_pAIEnemy->GetTargetDistance() >
		(g_pGameCVars->ai_tauntDistance + g_pGameCVars->ai_distanceThreshold))
	{
		m_pAIEnemy->GetStateManager()->ChangeState(EAIStates::Chase);
	}
}
void CAIStateTaunt::Exit()
{}
// -----------------------------------------------------

// Attack Group State
void CAIStateAttack::Enter()
{
	m_pAIEnemy->ForceFinishLastAction();
}
void CAIStateAttack::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	if (m_pAIEnemy->IsAttackTimeCompleted())
	{
		m_pAIEnemy->Attack();
		m_pAIEnemy->ResetAttackTime();
	}
	else
	{
		m_pAIEnemy->RunOutAttackTime(ctx.fFrameTime);

		if (m_pAIEnemy->GetTargetDistance() > g_pGameCVars->ai_startChaseDistance)
		{
			m_pAIEnemy->GetStateManager()->ChangeState(EAIStates::Chase);
		}
	}
}
void CAIStateAttack::Exit()
{}
// -----------------------------------------------------

// Swoon State
void CAIStateSwoon::Enter()
{
	m_pAIEnemy->ResetAttackTime();
}
void CAIStateSwoon::Update(SEntityUpdateContext& ctx, int updateSlot)
{}
void CAIStateSwoon::Exit()
{}
// -----------------------------------------------------

// Slave State
void CAIStateSlave::Enter()
{
	m_pAIEnemy->ResetAttackTime();
}
void CAIStateSlave::Update(SEntityUpdateContext& ctx, int updateSlot)
{}
void CAIStateSlave::Exit()
{}