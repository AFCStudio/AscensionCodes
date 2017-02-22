// -------------------------------------------------------------
//	PlayerStateManager.h - Header file
// -------------------------------------------------------------
//	Description:
//
//		Manager of player states.
//
// -------------------------------------------------------------
//	Creation Date: 20.02.2017
//										Writed by Özkan Afacan
//		Copyright (C), AFC Studios
// -------------------------------------------------------------

#ifndef __PLAYER_STATE_MANAGER_H__
#define __PLAYER_STATE_MANAGER_H__

#pragma once

#include "PlayerStates.h"
#include "Entities/Helpers/ISimpleExtension.h"

class CPlayer;

enum EPlayerStates { epsIdle, epsMove, epsAlnitak, epsCapture, epsFly, epsSwim };

////////////////////////////////////////////////////////
// Player extension to manage player states
////////////////////////////////////////////////////////
class CPlayerStateManager : public CGameObjectExtensionHelper<CPlayerStateManager, ISimpleExtension>
{
public:

	CPlayerStateManager();
	virtual ~CPlayerStateManager() {};

	EPlayerStates GetCurrentState() { return m_epsCurrentState; }
	EPlayerStates GetPreviousState() { return m_epsPreviousState; }

	void ChangeState(EPlayerStates state);

	// ISimpleExtension
	virtual void PostInit(IGameObject* pGameObject) override;

	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override;
	// ~ISimpleExtension

	void InitPlayerStates();

private:
	EPlayerStates m_epsCurrentState;
	EPlayerStates m_epsPreviousState;

	IPlayerState * m_pCurrentPlayerState;
	IPlayerState * m_pPreviousPlayerState;

	CPlayer * m_pPlayer;


	CPlayerStateIdle	* m_pIdlePlayerState;
	CPlayerStateMove	* m_pMovePlayerState;
	CPlayerStateAlnitak	* m_pAlnitakPlayerState;
	CPlayerStateCapture	* m_pCapturePlayerState;
	CPlayerStateFly		* m_pFlyPlayerState;
	CPlayerStateSwim	* m_pSwimPlayerState;
};

#endif // !__PLAYER_STATE_MANAGER_H__