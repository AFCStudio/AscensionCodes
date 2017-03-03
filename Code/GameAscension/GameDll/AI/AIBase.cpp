/* -------------------------------------------------------------
-	AIBase.cpp - Source file
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

#include "StdAfx.h"
#include "AIBase.h"

#include "GamePlugin.h"
#include "Game/GameRules.h"

#include "Actions/MoveAction.h"

#include "Entities/Gameplay/SpawnPoint.h"

#include <CryRenderer/IRenderAuxGeom.h>

#include <CrySerialization/Decorators/Resources.h>

class CAIBaseRegistrator
	: public IEntityRegistrator
{
	virtual void Register() override
	{
		CGamePlugin::RegisterEntityWithDefaultComponent<CAIBase>("AIBase");
	}

	virtual void Unregister() override
	{
	}
};

CAIBaseRegistrator g_AIRegistrator;

CAIBase::CAIBase()
{
	m_bIsPlayer = false;

	m_weaponType = EWeaponType::Knife;
}

CAIBase::~CAIBase()
{
	SAFE_RELEASE(m_pActionController);
	SAFE_DELETE(m_pAnimationContext);

	gEnv->pGameFramework->GetIActorSystem()->RemoveActor(GetEntityId());
}

void CAIBase::PostInit(IGameObject *pGameObject)
{
	// Register with the actor system
	gEnv->pGameFramework->GetIActorSystem()->AddActor(GetEntityId(), this);

	pGameObject->EnableUpdateSlot(this, 0);

	SetHealth(GetMaxHealth());
}

// Animation and Mannequin
// ------------------------------------------------------------------------

void CAIBase::SetActorMannequin()
{
	CActor::SetActorMannequin();

	SetWeaponTag(EWeaponType::NoWeapon);
}

// ------------------------------------------------------------------------
// ~Animation and Mannequin

