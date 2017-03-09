// -------------------------------------------------------------
//	AIStateManager.h - Header file
// -------------------------------------------------------------
//	Description:
//
//		Manager of AI states.
//
// -------------------------------------------------------------
//	Creation Date: 03.03.2017
//										Writed by Özkan Afacan
//		Copyright (C), AFC Studios
// -------------------------------------------------------------

#ifndef __AI_STATE_MANAGER_H__
#define __AI_STATE_MANAGER_H__

#pragma once

#include "AIStates.h"
#include "Entities/Helpers/ISimpleExtension.h"

class CAIEnemy;

enum class EAIStates { Idle, Move, Chase, Taunt, Attack, Swoon, Slave };

////////////////////////////////////////////////////////
// Player extension to manage player states
////////////////////////////////////////////////////////
class CAIStateManager : public CGameObjectExtensionHelper<CAIStateManager, ISimpleExtension>
{
public:

	CAIStateManager();
	virtual ~CAIStateManager() {};

	EAIStates GetCurrentState() { return m_eaisCurrentState; }
	EAIStates GetPreviousState() { return m_eaisPreviousState; }

	void ChangeState(EAIStates state);

	// ISimpleExtension
	virtual void PostInit(IGameObject* pGameObject) override;

	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override;
	// ~ISimpleExtension

	void InitAIStates();

private:
	EAIStates m_eaisCurrentState;
	EAIStates m_eaisPreviousState;

	IAIState * m_pCurrentAIState;
	IAIState * m_pPreviousAIState;

	CAIEnemy * m_pAIEnemy;


	CAIStateIdle		* m_pIdleAIState;
	CAIStateMove		* m_pMoveAIState;
	CAIStateChase		* m_pChaseAIState;
	CAIStateTaunt		* m_pTauntAIState;
	CAIStateAttack		* m_pAttackAIState;
	CAIStateSwoon		* m_pSwoonAIState;
	CAIStateSlave		* m_pSlaveAIState;
};

#endif // !__AI_STATE_MANAGER_H__