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
}
void CAIStateIdle::Exit()
{}
// -----------------------------------------------------

// Move State
void CAIStateMove::Enter()
{
	m_pAIEnemy->PlayMovementAction("MotionMovement", PP_Movement, TAG_STATE_EMPTY);
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
	m_pAIEnemy->PlayFragment("Taunt", PP_PlayerAction, TAG_STATE_EMPTY);
}
void CAIStateTaunt::Update(SEntityUpdateContext& ctx, int updateSlot)
{

}
void CAIStateTaunt::Exit()
{}
// -----------------------------------------------------

// Attack Group State
void CAIStateAttack::Enter()
{
}
void CAIStateAttack::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	if (m_pAIEnemy->IsAttackTimeCompleted())
	{
		m_pAIEnemy->PlayFragment("Attack", PP_Sword, TAG_STATE_EMPTY);
		m_pAIEnemy->ResetAttackTime();
	}
	else
	{
		m_pAIEnemy->RunOutAttackTime(ctx.fFrameTime);
	}
}
void CAIStateAttack::Exit()
{}
// -----------------------------------------------------

// Swoon State
void CAIStateSwoon::Enter()
{}
void CAIStateSwoon::Update(SEntityUpdateContext& ctx, int updateSlot)
{}
void CAIStateSwoon::Exit()
{}
// -----------------------------------------------------

// Slave State
void CAIStateSlave::Enter()
{}
void CAIStateSlave::Update(SEntityUpdateContext& ctx, int updateSlot)
{}
void CAIStateSlave::Exit()
{}
