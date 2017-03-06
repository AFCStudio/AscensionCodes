/* -------------------------------------------------------------
-	MoveForceAction.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for player move animations with
-		forced rotation according to animation.
-
-	Class use pe_action_move to move player according to
-	velocity.
-
-	That class behave as MoveAction except player rotation.
-
-  -------------------------------------------------------------
-	Creation Date: 05.05.2016
-	Edit     Date: 20.02.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __MOVE_FORCE_ACTION_H__
#define __MOVE_FORCE_ACTION_H__

#pragma once

#include "MoveAction.h"

class CMoveForceAction : public CMoveAction
{
	typedef CMoveAction BaseClass;

public:
	CMoveForceAction(
		IEntity * pEntity,
		int priority = PP_PlayerAction, FragmentID fragmentID = FRAGMENT_ID_INVALID,
		const TagState &fragTags = TAG_STATE_EMPTY,
		uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0);

	virtual EStatus Update(float timePassed);

	virtual void Enter();
	virtual void Exit();
};

#endif