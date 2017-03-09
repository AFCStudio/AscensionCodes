// -------------------------------------------------------------
//	AIStateManager.cpp - Source file
// -------------------------------------------------------------
//	Description:
//
//		Manager of player states.
//
// -------------------------------------------------------------
//	Creation Date: 21.02.2017
//										Writed by Özkan Afacan
//		Copyright (C), AFC Studios
// -------------------------------------------------------------


#include "StdAfx.h"
#include "AIStateManager.h"
#include "AI/AIEnemy.h"

CAIStateManager::CAIStateManager()
	: m_eaisCurrentState(EAIStates::Idle)
	, m_eaisPreviousState(EAIStates::Idle)
	, m_pCurrentAIState(nullptr)
	, m_pPreviousAIState(nullptr)
	, m_pIdleAIState(nullptr)
	, m_pMoveAIState(nullptr)
	, m_pChaseAIState(nullptr)
	, m_pTauntAIState(nullptr)
	, m_pAttackAIState(nullptr)
	, m_pSwoonAIState(nullptr)
	, m_pSlaveAIState(nullptr)
{
	
}

void CAIStateManager::InitAIStates()
{
	m_pIdleAIState = new CAIStateIdle(m_pAIEnemy);
	m_pMoveAIState = new CAIStateMove(m_pAIEnemy);
	m_pChaseAIState = new CAIStateChase(m_pAIEnemy);
	m_pTauntAIState = new CAIStateTaunt(m_pAIEnemy);
	m_pAttackAIState = new CAIStateAttack(m_pAIEnemy);
	m_pSwoonAIState = new CAIStateSwoon(m_pAIEnemy);
	m_pSlaveAIState = new CAIStateSlave(m_pAIEnemy);

	m_pPreviousAIState = m_pIdleAIState;
	m_pCurrentAIState = m_pIdleAIState;
}

void CAIStateManager::PostInit(IGameObject * pGameObject)
{
	m_pAIEnemy = static_cast<CAIEnemy *>(pGameObject->QueryExtension("AIEnemy"));

	// Make sure that this extension is updated regularly via the Update function below
	pGameObject->EnableUpdateSlot(this, 0);

	InitAIStates();

	ChangeState(EAIStates::Idle);
}

void CAIStateManager::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	if (m_pCurrentAIState)
		m_pCurrentAIState->Update(ctx, updateSlot);
}

void CAIStateManager::ChangeState(EAIStates eps)
{
	if (m_eaisCurrentState == eps)
		return;

	m_eaisPreviousState = m_eaisCurrentState;
	m_eaisCurrentState = eps;

	if (m_pCurrentAIState != nullptr)
	{
		m_pCurrentAIState->Exit();
		m_pPreviousAIState = m_pCurrentAIState;
	}
	
	switch (eps)
	{
	case EAIStates::Idle:		m_pCurrentAIState = m_pIdleAIState;		break;
	case EAIStates::Move:		m_pCurrentAIState = m_pMoveAIState;		break;
	case EAIStates::Chase:		m_pCurrentAIState = m_pChaseAIState;	break;
	case EAIStates::Taunt:		m_pCurrentAIState = m_pTauntAIState;	break;
	case EAIStates::Attack:		m_pCurrentAIState = m_pAttackAIState;	break;
	case EAIStates::Swoon:		m_pCurrentAIState = m_pSwoonAIState;	break;
	case EAIStates::Slave:		m_pCurrentAIState = m_pSlaveAIState;	break;
	}

	m_pCurrentAIState->Enter();
}
