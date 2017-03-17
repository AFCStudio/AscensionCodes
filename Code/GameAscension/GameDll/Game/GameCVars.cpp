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
	// Player Attack Settings
	REGISTER_CVAR(pl_attackDuration, 4.0f, VF_CHEAT, "Total attack duration of player for sequence tag.");
	REGISTER_CVAR(pl_maxAttackAngle, 1.2f, VF_CHEAT, "Maximum angle which player identify a target actor.");
	REGISTER_CVAR(pl_alignDistance, 1.75f, VF_CHEAT, "Alignment distance while player attacking to a enemy.");
	REGISTER_CVAR(pl_alignSpeed, 2.0f, VF_CHEAT, "Alignment speed while player attacking to a enemy.");
	REGISTER_CVAR(pl_alignThreshold, 0.1f, VF_CHEAT, "Alignment threshold while player attack to a enemy.");
	REGISTER_CVAR(pl_hitReactionDelay, 0.33f, VF_CHEAT, "Global hit reaction delay after attack animation started.");

	// Camera Settings
	REGISTER_CVAR(cam_viewOffsetX, 0.35f, VF_CHEAT, "View offset along the right axis from the player entity");
	REGISTER_CVAR(cam_viewOffsetY, -1.5f, VF_CHEAT, "View offset along the forward axis from the player entity");
	REGISTER_CVAR(cam_viewOffsetZ, 1.5f, VF_CHEAT, "View offset along the up axis from the player entity");
	REGISTER_CVAR(cam_nearPlane, 0.011f, VF_CHEAT, "Near clip plane camera for avoid clip planes in obstacles");
	REGISTER_CVAR(cam_colliderRadius, 0.3f, VF_CHEAT, "Primitive world intersection sphere radius.");
	REGISTER_CVAR(cam_colliderExtraLength, 0.02f, VF_CHEAT, "Primitive world intersection collider length.");
	REGISTER_CVAR(cam_colliderTransitionSpeed, 7.0f, VF_CHEAT, "Primitive world intersection collider interpolation speed.");

	// Fighting System
	REGISTER_CVAR(ai_attackDistance, 1.5f, VF_CHEAT, "AI attack distance to target.");
	REGISTER_CVAR(ai_tauntDistance, 4.0f, VF_CHEAT, "AI taunt distance to target.");
	REGISTER_CVAR(ai_distanceThreshold, 0.2f, VF_CHEAT, "AI distance threshold to stop location.");
	REGISTER_CVAR(ai_maxAttackerCount, 3, VF_CHEAT, "Maximum attack count in one time.");
	REGISTER_CVAR(ai_maxAttackTime, 5.0f, VF_CHEAT, "Maximum time between per attack for each enemy.");
	REGISTER_CVAR(ai_minAttackTime, 3.0f, VF_CHEAT, "Minimum time between per attack for each enemy.");
}

void SCVars::ReleaseCVars()
{
	IConsole* pConsole = gEnv->pConsole;
	if (pConsole)
	{
		// Player Attack Settings
		pConsole->UnregisterVariable("pl_attackDuration", true);
		pConsole->UnregisterVariable("pl_maxAttackAngle", true);
		pConsole->UnregisterVariable("pl_alignDistance", true);
		pConsole->UnregisterVariable("pl_alignSpeed", true);
		pConsole->UnregisterVariable("pl_alignThreshold", true);
		pConsole->UnregisterVariable("pl_hitReactionDelay", true);

		// Camera Settings
		pConsole->UnregisterVariable("cam_viewOffsetX", true);
		pConsole->UnregisterVariable("cam_viewOffsetY", true);
		pConsole->UnregisterVariable("cam_viewOffsetZ", true);
		pConsole->UnregisterVariable("cam_nearPlane", true);
		pConsole->UnregisterVariable("cam_colliderRadius", true);
		pConsole->UnregisterVariable("cam_colliderExtraLength ", true);
		pConsole->UnregisterVariable("cam_colliderTransitionSpeed ", true);

		// Fighting System
		pConsole->UnregisterVariable("ai_attackDistance ", true);
		pConsole->UnregisterVariable("ai_tauntDistance ", true);
		pConsole->UnregisterVariable("ai_distanceThreshold ", true);
		pConsole->UnregisterVariable("ai_maxAttackerCount ", true);
		pConsole->UnregisterVariable("ai_maxAttackTime ", true);
		pConsole->UnregisterVariable("ai_minAttackTime ", true);
	}
}