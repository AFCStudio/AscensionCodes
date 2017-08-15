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

#include "HitReaction/HitReaction.h"

#include <sstream>


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
	, m_isAttackInQueue(false)
	, m_reactionDelay(0.0f)
	, m_attackingTimer(0.0f)
	, m_fightingGroup(EFightingGroup::NoFighting)
{
}

CAIEnemy::~CAIEnemy()
{
	SAFE_RELEASE(m_pActionController);
	SAFE_DELETE(m_pAnimationContext);

	gEnv->pGameFramework->GetIActorSystem()->RemoveActor(GetEntityId());

	RemoveFromAIFightingSystem();
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

void CAIEnemy::Attack()
{
	if (m_pTargetActor)
	{
		m_pTargetActor->AddAttackToQueue(this);

		m_reactionDelay = g_pGameCVars->ai_hitReactionDelay;
		m_isAttackInQueue = true;
	}

	PlayMoveAction("AI_Attack", true, PP_Attack);
}
void CAIEnemy::PostInit(IGameObject *pGameObject)
{
	m_pStateManager = static_cast<CAIStateManager *>(GetGameObject()->AcquireExtension("AIStateManager"));

	// Register with the actor system
	gEnv->pGameFramework->GetIActorSystem()->AddActor(GetEntityId(), this);

	pGameObject->EnableUpdateSlot(this, 0);

	SetHealth(GetMaxHealth());

	m_pTargetEntity = gEnv->pGameFramework->GetClientActor()->GetEntity();
	m_pTargetActor = static_cast<CActor *>(gEnv->pGameFramework->GetClientActor());
	if (m_pTargetEntity && m_pTargetActor)
	{
		CryLog("Client Actor has been found!");
	}
}

void CAIEnemy::Update(SEntityUpdateContext & ctx, int updateSlot)
{
	BaseClass::Update(ctx, updateSlot);

	if (m_isAttackInQueue)
	{
		m_reactionDelay -= ctx.fFrameTime;

		if (m_reactionDelay <= 0.0f)
		{
			m_isAttackInQueue = false;

			if (m_pTargetActor)
			{
				float distance = GetEntity()->GetPos().GetDistance(m_pTargetActor->GetEntity()->GetPos());
				if (distance <= g_pGameCVars->ai_successAttackDistance + g_pGameCVars->ai_distanceThreshold)
					m_pTargetActor->HitReaction(this->GetEntity());
			}
		}
	}
}

void CAIEnemy::SetFightingGroup(EFightingGroup fightingGroup)
{
	switch (fightingGroup)
	{
	case EFightingGroup::Taunt:		m_stopOffset = g_pGameCVars->ai_tauntDistance;	break;
	case EFightingGroup::Attack:	m_stopOffset = g_pGameCVars->ai_successAttackDistance;	break;
	default:	m_stopOffset = 0.0f;	break;
	}

	m_fightingGroup = fightingGroup;
}

void CAIEnemy::ResetAttackTime()
{
	m_attackingTimer = cry_random(g_pGameCVars->ai_minAttackTime, g_pGameCVars->ai_maxAttackTime);
}

void CAIEnemy::CancelAttack()
{
	m_isAttackInQueue = false;
	ForceFinishLastAction();
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

void CAIEnemy::AddToAIFightingSystem()
{
	if (m_pTargetActor)
		m_pTargetActor->AddEnemyFighter(this);
}

void CAIEnemy::RemoveFromAIFightingSystem()
{
	if (m_pTargetActor)
		m_pTargetActor->RemoveEnemyFighter(this);
}

void CAIEnemy::DebugLog()
{
	std::ostringstream os;

	os << "State: " << m_pStateManager->GetStateName() << std::endl
		<< "Health: " << GetHealth() << std::endl
		<< "Target Actor: " << (m_pTargetActor ? m_pTargetActor->GetEntity()->GetName() : "NULL") << std::endl
		<< "Attacking Timer: " << m_attackingTimer << std::endl
		<< "Attack in Queue: " << (m_isAttackInQueue ? "True" : "False") << std::endl
		<< "Reaction Delay: " << m_reactionDelay << std::endl;

	Vec3 screenPos(ZERO);
	Vec3 actorPos = GetEntity()->GetPos();

	actorPos.z += 2.0f;

	gEnv->pRenderer->ProjectToScreen(actorPos.x, actorPos.y, actorPos.z, &screenPos.x, &screenPos.y, &screenPos.z);

	screenPos.x *= 0.01f * gEnv->pRenderer->GetWidth();
	screenPos.y *= 0.01f * gEnv->pRenderer->GetHeight();

	ColorF textColor(1.0f, 1.0f, 1.0f, 0.7f);

	gEnv->pRenderer->GetIRenderAuxGeom()->Draw2dLabel(screenPos.x, screenPos.y, 1.0f, textColor, true, os.str().c_str());
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

