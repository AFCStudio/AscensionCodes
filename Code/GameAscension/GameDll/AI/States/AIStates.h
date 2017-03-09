// -------------------------------------------------------------
//	AIStates.h - Header file
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

#ifndef __PLAYER_STATES_H__
#define __PLAYER_STATES_H__

#pragma once

class CAIEnemy;

class IAIState
{
public:
	virtual void Enter() = 0;
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) = 0;
	virtual void Exit() = 0;
};

#define DEFINE_STATE(name)\
	class CAIState##name## : public IAIState\
	{\
	public:\
		CAIState##name##(CAIEnemy * pAIEnemy) : m_pAIEnemy(pAIEnemy) {}\
								\
		virtual void Enter(); \
		virtual void Update(SEntityUpdateContext& ctx, int updateSlot); \
		virtual void Exit(); \
							 \
	private:\
		CAIEnemy * m_pAIEnemy;\
	}

DEFINE_STATE(Idle);
DEFINE_STATE(Move);
DEFINE_STATE(Chase);
DEFINE_STATE(Taunt);
DEFINE_STATE(Attack);
DEFINE_STATE(Swoon);
DEFINE_STATE(Slave);

#endif