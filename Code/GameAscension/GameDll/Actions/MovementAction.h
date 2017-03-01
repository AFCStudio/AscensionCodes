/* -------------------------------------------------------------
-	MovementAction.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for player movement mechanic.
-
-	Class use pe_action_move to move player according to
-	velocity.
-
-  -------------------------------------------------------------
-	Creation Date: 23.02.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __MOVEMENT_ACTION_H__
#define __MOVEMENT_ACTION_H__

#pragma once

#include "ActionBase.h"

class CActor;

class CMovementAction : public TPlayerAction
{
	typedef TPlayerAction BaseClass;

public:
	CMovementAction(
		CActor * pActor,
		int priority = PP_PlayerAction, FragmentID fragmentID = FRAGMENT_ID_INVALID,
		const TagState &fragTags = TAG_STATE_EMPTY,
		uint32 flags = 0, ActionScopes scopeMask = 0, uint32 userToken = 0);

	virtual EStatus Update(float timePassed);

protected:

	CActor		* m_pActor;
};

#endif