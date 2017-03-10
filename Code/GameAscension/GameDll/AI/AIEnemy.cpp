/* -------------------------------------------------------------
-	AIEnemy.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-		AIEnemy class, that class defines a general Enemy AI
-	and its behviors.
-
-  -------------------------------------------------------------
-	Creation Date: 06-03-2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#include "StdAfx.h"
#include "AIEnemy.h"

#include "GamePlugin.h"
#include "Game/GameRules.h"
#include "Game/GameCVars.h"

#include "Actions/MoveAction.h"

#include "AI/States/AIStateManager.h"


class CAIEnemyRegistrator
	: public IEntityRegistrator
{
	virtual void Register() override
	{
		CGamePlugin::RegisterEntityWithDefaultComponent<CAIEnemy>("AIEnemy");

		CGamePlugin::RegisterEntityComponent<CAIStateManager>("AIStateManager");
	}

	virtual void Unregister() override
	{
	}
};

CAIEnemyRegistrator g_AIEnemyRegistrator;

CAIEnemy::CAIEnemy()
	: m_teamId(0)
	, m_pTargetActor(nullptr)
	, m_pStateManager(nullptr)
	, m_fightingGroup(EFightingGroup::NoFighting)
{
}

CAIEnemy::~CAIEnemy()
{
	SAFE_RELEASE(m_pActionController);
	SAFE_DELETE(m_pAnimationContext);

	gEnv->pGameFramework->GetIActorSystem()->RemoveActor(GetEntityId());
}

void CAIEnemy::SerializeProperties(Serialization::IArchive & archive)
{
	BaseClass::SerializeProperties(archive);

	archive(m_teamId, "TeamID", "TeamID");

	if (archive.isInput())
	{
		SetHealth(GetMaxHealth());
	}
}

void CAIEnemy::PostInit(IGameObject *pGameObject)
{
	m_pStateManager = static_cast<CAIStateManager *>(GetGameObject()->AcquireExtension("AIStateManager"));

	// Register with the actor system
	gEnv->pGameFramework->GetIActorSystem()->AddActor(GetEntityId(), this);

	pGameObject->EnableUpdateSlot(this, 0);

	SetHealth(GetMaxHealth());
}

void CAIEnemy::SetFightingGroup(EFightingGroup fightingGroup)
{
	switch (fightingGroup)
	{
	case EFightingGroup::Taunt:		m_stopOffset = g_pGameCVars->ai_tauntDistance;	break;
	case EFightingGroup::Attack:	m_stopOffset = g_pGameCVars->ai_attackDistance;	break;
	default:	m_stopOffset = 0.0f;	break;
	}

	m_fightingGroup = fightingGroup;
}

void CAIEnemy::ResetAttackTime()
{
	m_attackingTimer = cry_random(g_pGameCVars->ai_minAttackTime, g_pGameCVars->ai_maxAttackTime);
}

const Vec3 CAIEnemy::GetMoveDirection() const
{
	if (m_pTargetEntity)
	{
		Vec3 targetLocation = m_pTargetEntity->GetPos() - GetEntity()->GetPos();
		targetLocation.z = 0.0f;
		targetLocation.Normalize();
		return targetLocation;
	}
	else
	{
		return Vec3(ZERO);
	}
}

// Animation and Mannequin
// ------------------------------------------------------------------------

void CAIEnemy::SetActorMannequin()
{
	CActor::SetActorMannequin();

	SetWeaponTag(EWeaponType::NoWeapon);
}

// ------------------------------------------------------------------------
// ~Animation and Mannequin

