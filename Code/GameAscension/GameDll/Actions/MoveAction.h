/* -------------------------------------------------------------
-	MoveAction.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for player moving animations.
-
-	Class use pe_action_move to move player according to
-	velocity.
-
-  -------------------------------------------------------------
-	Creation Date: 05.05.2016
-	Edit     Date: 20.02.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __MOVE_ACTION_H__
#define __MOVE_ACTION_H__

#pragma once

#include "ActionBase.h"

class CActor;

class CMoveAction : public TPlayerAction
{
	typedef TPlayerAction BaseClass;

public:
	CMoveAction(
		CActor * pActor, bool isRotateForce = false,
		int priority = PP_PlayerAction, FragmentID fragmentID = FRAGMENT_ID_INVALID,
		const TagState &fragTags = TAG_STATE_EMPTY,
		uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0);

	virtual EStatus Update(float timePassed);
	virtual void Enter();

protected:
	CActor * m_pActor;

	bool	m_bIsRotateForce;

	float m_fTotalRot;

	Quat m_initialRot;

	float m_initialZ;
};

#endif