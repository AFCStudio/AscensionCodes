// -------------------------------------------------------------
//	GameCVars.h - Header file
// -------------------------------------------------------------
//	Description:
//
//		That class storage console variables for game.
//
// -------------------------------------------------------------
//	Creation Date: 18.02.2017
//										Writed by Özkan Afacan
//		Copyright (C), AFC Studios
// -------------------------------------------------------------

#ifndef __GAME_CVARS_H__
#define __GAME_CVARS_H__

#pragma once

#include "IGameObject.h"

struct SCVars
{
	// Player Attack Settings
	float pl_attackDuration;
	float pl_maxAttackAngle;
	float pl_alignDistance;
	float pl_alignSpeed;
	float pl_alignThreshold;
	float pl_hitReactionDelay;

	// Player Camera Settings
	float cam_viewOffsetX;
	float cam_viewOffsetY;
	float cam_viewOffsetZ;

	float cam_nearPlane;
	float cam_colliderRadius;
	float cam_colliderExtraLength;
	float cam_colliderTransitionSpeed;

	// Fighting System
	float ai_successAttackDistance;
	float ai_startChaseDistance;
	float ai_tauntDistance;
	float ai_distanceThreshold;
	int   ai_maxAttackerCount;
	float ai_maxAttackTime;
	float ai_minAttackTime;
	float ai_hitReactionDelay;

	// Debug
	int	  ai_debug;


	SCVars()
	{
		memset(this, 0, sizeof(SCVars));
	}

	~SCVars() { ReleaseCVars(); }

	void InitCVars();
	void ReleaseCVars();
};

extern SCVars * g_pGameCVars;

#endif