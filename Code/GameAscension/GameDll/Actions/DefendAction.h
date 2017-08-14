/* -------------------------------------------------------------
-	DefendAction.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for player defence animations.
-
-	Class use pe_action_move to move player according to
-	velocity.
-
-  -------------------------------------------------------------
-	Creation Date: 31.07.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __DEFEND_ACTION_H__
#define __DEFEND_ACTION_H__

#pragma once

#include "MoveAction.h"

class CActor;
class CAIEnemy;

class CDefendAction : public CMoveAction
{
	typedef CMoveAction BaseClass;

public:
	CDefendAction(
		CActor * pActor, CAIEnemy * pEnemy, bool isRotateForce = false,
		int priority = PP_PlayerAction, FragmentID fragmentID = FRAGMENT_ID_INVALID,
		const TagState &fragTags = TAG_STATE_EMPTY,
		uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0);

	virtual EStatus Update(float timePassed) override;
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	CAIEnemy * m_pEnemy;
};

#endif