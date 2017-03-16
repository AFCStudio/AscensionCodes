/* -------------------------------------------------------------
-	AttackAction.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		That action class deals with attack animation move
-	and position and rotation alignment according to enemy.
-
-  -------------------------------------------------------------
-	Creation Date: 13.03.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __ATTACK_ACTION_H__
#define __ATTACK_ACTION_H__

#pragma once

#include "ActionBase.h"

class CPlayer;

class CAttackAction : public TPlayerAction
{
	typedef TPlayerAction BaseClass;

public:
	CAttackAction(
		CPlayer * pPlayer,
		int priority = PP_PlayerAction, FragmentID fragmentID = FRAGMENT_ID_INVALID,
		const TagState &fragTags = TAG_STATE_EMPTY,
		uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0);

	virtual EStatus Update(float timePassed);
	virtual void Enter();

protected:
	CPlayer * m_pPlayer;

	Quat  m_initialRot;
	float m_targetAngle;
	float m_totalRot;
};

#endif