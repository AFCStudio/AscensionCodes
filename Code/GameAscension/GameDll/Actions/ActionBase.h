/* -------------------------------------------------------------
-	ActionBase.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		That class stores basic definations for action.
-
-  -------------------------------------------------------------
-	Creation Date: 20.02.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __ACTION_BASE_H__
#define __ACTION_BASE_H__

#pragma once

#include "ICryMannequin.h"

typedef TAction<SAnimationContext> TPlayerAction;

enum EActionPriority
{
	PP_Lowest,
	PP_Movement,
	PP_PlayerAction,
	PP_Attack,
	PP_Defend,
	PP_HitReaction,
	PP_Death,
	PP_PostDeath
};

#endif