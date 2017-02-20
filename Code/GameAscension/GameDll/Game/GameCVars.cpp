// -------------------------------------------------------------
//	GameCVars.cpp - Header file
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

#include "GameCVars.h"

SCVars * g_pGameCVars = 0;

void SCVars::InitCVars()
{
	// Camera Settings
	REGISTER_CVAR(cam_viewOffsetX, 0.35f, VF_CHEAT, "View offset along the right axis from the player entity");
	REGISTER_CVAR(cam_viewOffsetY, -1.5f, VF_CHEAT, "View offset along the forward axis from the player entity");
	REGISTER_CVAR(cam_viewOffsetZ, 1.5f, VF_CHEAT, "View offset along the up axis from the player entity");
	REGISTER_CVAR(cam_nearPlane, 0.011f, VF_CHEAT, "Near clip plane camera for avoid clip planes in obstacles");
	REGISTER_CVAR(cam_colliderRadius, 0.3f, VF_CHEAT, "Primitive world intersection sphere radius.");
	REGISTER_CVAR(cam_colliderExtraLength, 0.02f, VF_CHEAT, "Primitive world intersection collider length.");
	REGISTER_CVAR(cam_colliderTransitionSpeed, 7.0f, VF_CHEAT, "Primitive world intersection collider interpolation speed.");
}

void SCVars::ReleaseCVars()
{
	IConsole* pConsole = gEnv->pConsole;
	if (pConsole)
	{
		// Camera Settings
		pConsole->UnregisterVariable("cam_viewOffsetX", true);
		pConsole->UnregisterVariable("cam_viewOffsetY", true);
		pConsole->UnregisterVariable("cam_viewOffsetZ", true);
		pConsole->UnregisterVariable("cam_nearPlane", true);
		pConsole->UnregisterVariable("cam_colliderRadius", true);
		pConsole->UnregisterVariable("cam_colliderExtraLength ", true);
		pConsole->UnregisterVariable("cam_colliderTransitionSpeed ", true);
	}
}