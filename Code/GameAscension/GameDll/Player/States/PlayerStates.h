// -------------------------------------------------------------
//	PlayerStates.h - Header file
// -------------------------------------------------------------
//	Description:
//
//		Defines player states for game.
//
// -------------------------------------------------------------
//	Creation Date: 20.02.2017
//										Writed by Özkan Afacan
//		Copyright (C), AFC Studios
// -------------------------------------------------------------

#ifndef __PLAYER_STATES_H__
#define __PLAYER_STATES_H__

#pragma once

class CPlayer;

class IPlayerState
{
public:
	virtual void Enter() = 0;
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) = 0;
	virtual void Exit() = 0;
};

#define DEFINE_STATE(name)\
	class CPlayerState##name## : public IPlayerState\
	{\
	public:\
		CPlayerState##name##(CPlayer * pPlayer) : m_pPlayer(pPlayer) {}\
								\
		virtual void Enter(); \
		virtual void Update(SEntityUpdateContext& ctx, int updateSlot); \
		virtual void Exit(); \
							 \
	private:\
		CPlayer * m_pPlayer;\
	}

DEFINE_STATE(Idle);
DEFINE_STATE(Move);
DEFINE_STATE(Alnitak);
DEFINE_STATE(Capture);
DEFINE_STATE(Fly);
DEFINE_STATE(Swim);

#endif