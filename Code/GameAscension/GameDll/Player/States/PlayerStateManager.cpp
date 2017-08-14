// -------------------------------------------------------------
//	PlayerStateManager.cpp - Source file
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
#include "PlayerStateManager.h"
#include "Player/Player.h"

CPlayerStateManager::CPlayerStateManager()
	: m_epsCurrentState(EPlayerStates::Idle)
	, m_epsPreviousState(EPlayerStates::Idle)
	, m_pCurrentPlayerState(nullptr)
	, m_pPreviousPlayerState(nullptr)
	, m_pIdlePlayerState(nullptr)
	, m_pMovePlayerState(nullptr)
	, m_pAlnitakPlayerState(nullptr)
	, m_pCapturePlayerState(nullptr)
	, m_pFlyPlayerState(nullptr)
	, m_pSwimPlayerState(nullptr)
{
	
}

void CPlayerStateManager::InitPlayerStates()
{
	m_pIdlePlayerState = new CPlayerStateIdle(m_pPlayer);
	m_pMovePlayerState = new CPlayerStateMove(m_pPlayer);
	m_pCapturePlayerState = new CPlayerStateCapture(m_pPlayer);
	m_pFlyPlayerState = new CPlayerStateFly(m_pPlayer);
	m_pSwimPlayerState = new CPlayerStateSwim(m_pPlayer);

	m_pPreviousPlayerState = m_pIdlePlayerState;
	m_pCurrentPlayerState = m_pIdlePlayerState;
}

void CPlayerStateManager::PostInit(IGameObject * pGameObject)
{
	m_pPlayer = static_cast<CPlayer *>(pGameObject->QueryExtension("Player"));

	// Make sure that this extension is updated regularly via the Update function below
	pGameObject->EnableUpdateSlot(this, 0);

	InitPlayerStates();

	ChangeState(EPlayerStates::Idle);
}

void CPlayerStateManager::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	if (m_pCurrentPlayerState)
		m_pCurrentPlayerState->Update(ctx, updateSlot);
}

const char * CPlayerStateManager::GetStateName() const
{
	switch (m_epsCurrentState)
	{
	case EPlayerStates::Idle:		return "Idle";
	case EPlayerStates::Move:		return "Move";
	case EPlayerStates::Alnitak:	return "Alnitak";
	case EPlayerStates::Capture:	return "Capture";
	case EPlayerStates::Fly:		return "Fly";
	case EPlayerStates::Swim:		return "Swim";
	default:						return "Invalid State";
	}
}

void CPlayerStateManager::ChangeState(EPlayerStates eps)
{
	if (m_epsCurrentState == eps)
		return;

	m_epsPreviousState = m_epsCurrentState;
	m_epsCurrentState = eps;

	if (m_pCurrentPlayerState != nullptr)
	{
		m_pCurrentPlayerState->Exit();
		m_pPreviousPlayerState = m_pCurrentPlayerState;
	}
	
	switch (eps)
	{
	case EPlayerStates::Idle:	m_pCurrentPlayerState = m_pIdlePlayerState;	break;
	case EPlayerStates::Move:	m_pCurrentPlayerState = m_pMovePlayerState;	break;
	case EPlayerStates::Alnitak:	m_pCurrentPlayerState = m_pAlnitakPlayerState;	break;
	case EPlayerStates::Capture:	m_pCurrentPlayerState = m_pCapturePlayerState;	break;
	case EPlayerStates::Fly:	m_pCurrentPlayerState = m_pFlyPlayerState;	break;
	case EPlayerStates::Swim:	m_pCurrentPlayerState = m_pSwimPlayerState;	break;
	}

	m_pCurrentPlayerState->Enter();
}
