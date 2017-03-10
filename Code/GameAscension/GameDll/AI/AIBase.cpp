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

CAIBaseRegistrator g_AIBaseRegistrator;

CAIBase::CAIBase()
	: m_pTargetEntity(nullptr)
	, m_targetDistance(-1.0f)
	, m_stopOffset(0.0f)
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

void CAIBase::SerializeProperties(Serialization::IArchive & archive)
{
	archive(m_mass, "Mass", "Mass");
	archive(m_actorEyeHeight, "Eye Height", "Eye Height");

	archive(m_walkSpeed, "Walking Speed", "Walking Speed");
	archive(m_runSpeed, "Running Speed", "Running Speed");
	archive(m_turnSpeed, "Turning Speed", "Turning Speed");

	archive(Serialization::CharacterPath<string>(m_pCharacterGeometry), "Character", "Character");
	archive(Serialization::GeneralFilename<string>(m_actorMannequinInfo.m_pActorMannequinContext), "Mannequin Context", "Mannequin Context");
	archive(Serialization::GeneralFilename<string>(m_actorMannequinInfo.m_pActorAnimationDatabase), "Animation Database", "Animation Database");
	archive(Serialization::GeneralFilename<string>(m_actorMannequinInfo.m_pActorControllerDefinition), "Controller Definition", "Controller Definition");
	archive(Serialization::GeneralFilename<string>(m_actorMannequinInfo.m_pSlaveAnimationDatabase), "Slave Animation Database", "Slave Animation Database");

	if (archive.isInput())
	{
		SetHealth(GetMaxHealth());
	}
}

void CAIBase::PostInit(IGameObject *pGameObject)
{
	// Register with the actor system
	gEnv->pGameFramework->GetIActorSystem()->AddActor(GetEntityId(), this);

	pGameObject->EnableUpdateSlot(this, 0);

	SetHealth(GetMaxHealth());
}

void CAIBase::Update(SEntityUpdateContext & ctx, int updateSlot)
{
	BaseClass::Update(ctx, updateSlot);

	CalculateTargetDistance();
}

void CAIBase::CalculateTargetDistance()
{
	m_targetDistance = m_pTargetEntity ?
			GetEntity()->GetPos().GetDistance(m_pTargetEntity->GetPos()) : -1.0f;
}

const float CAIBase::GetStopDistance() const
{
	return m_targetDistance - m_stopOffset;
}

const Vec3 CAIBase::GetMoveDirection() const
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

const float CAIBase::GetMoveAngle() const
{ 
	return 0.0;
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

