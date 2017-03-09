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
	// Player Camera Settings
	float cam_viewOffsetX;
	float cam_viewOffsetY;
	float cam_viewOffsetZ;

	float cam_nearPlane;
	float cam_colliderRadius;
	float cam_colliderExtraLength;
	float cam_colliderTransitionSpeed;

	float ai_maxAttackTime;
	float ai_minAttackTime;

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