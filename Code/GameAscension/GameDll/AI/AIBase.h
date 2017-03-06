/* -------------------------------------------------------------
-	AIBase.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		AIBase class, that class defines a general AI
-	and its behviors.
-
-  -------------------------------------------------------------
-	Creation Date: 02-03-2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __AIBASE_H__
#define __AIBASE_H__

#pragma once

#include "Actor/Actor.h"

#include <CryMath/Cry_Camera.h>

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CAIBase 
	: public CGameObjectExtensionHelper<CAIBase, CActor>
{
public:
	CAIBase();
	virtual ~CAIBase();

	// ISimpleActor
	virtual void PostInit(IGameObject* pGameObject) override;
	// ~ISimpleActor

	// Animations and Mannequin
	virtual void SetActorMannequin() override;
	// ~Animations and Mannequin
};

#endif
