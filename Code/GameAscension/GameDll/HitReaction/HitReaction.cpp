/* -------------------------------------------------------------
-	HitReaction.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-			That class calculate reaction direction and angle.
-		As well calculate damage using actor strength or
-		projectile speed or strength, then set new health for
-		owned actor.
-
-			That class also calculate tag states to play final
-		animation action.
-
-  -------------------------------------------------------------
-	Creation Date: 15-03-2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#include "StdAfx.h"
#include "HitReaction.h"

#include "Actor/Actor.h"
#include "Actions/MoveAction.h"

CHitReaction::CHitReaction(CActor * pActor)
	: m_pActor(pActor)
	, m_reactionAngle(0.0f)
	, m_damage(0.0f)
	, m_reactionDir(ZERO)
	, m_hitType(EHitTypes::Sword)
{
}

char * CHitReaction::GetHitTypeString()
{
	switch (m_hitType)
	{
	case EHitTypes::Melee:			return "Melee";
	case EHitTypes::Sword:			return "Sword";
	case EHitTypes::Dagger:			return "Dagger";
	case EHitTypes::MagicBall:		return "MagicBall";
	default:						return nullptr;
	}
}

void CHitReaction::CalculateDamage()
{
	// This function should be calcaulte damage according
	// to hit type and attacker strength
	CryLog("Damage has been given!");
	m_damage = 10.0f;
}

void CHitReaction::HitReaction(IEntity * pAttacker, EHitTypes hitType)
{
	if (!pAttacker)
	{
		CryLog("Error: Hit reaction attacker entity is invalid!");
		return;
	}

	Vec3 actorDir = m_pActor->GetEntity()->GetForwardDir();

	// Reaction direction is opposite of coming attack direction
	m_reactionDir = pAttacker->GetPos() - m_pActor->GetEntity()->GetPos();
	m_reactionDir.z = 0.0f;
	m_reactionDir.Normalize();

	// Reaction angle to decide animation tag
	m_reactionAngle = acosf(m_reactionDir.dot(actorDir));
	m_reactionAngle *= m_reactionDir.cross(actorDir).z >= 0 ? 1 : -1;

	m_hitType = hitType;

	// Set health
	CalculateDamage();
	m_pActor->SetHealth(m_pActor->GetHealth() - m_damage);

	PlayReactionAction();
}

void CHitReaction::PlayReactionAction()
{
	// Get Tag Defination
	IActionController * pAC = m_pActor->GetActionController();
	if (pAC)
	{
		FragmentID fragID = pAC->GetContext().controllerDef.m_fragmentIDs.Find("reaction");
		const CTagDefinition * const pTagDefinition = pAC->GetTagDefinition(fragID);
		TagState tagState = TAG_STATE_EMPTY;

		EActionPriority priority = PP_HitReaction;

		TagGroupID tagGroupId = pTagDefinition->FindGroup("AttackFrom");
		TagID tagId;

		// Hit Direction Type
		if (m_reactionAngle >= DEG2RAD(-45) && m_reactionAngle <= 0.0f)
		{
			tagId = pTagDefinition->Find("ForwardLeft");
		}
		else if (m_reactionAngle >= 0.0f && m_reactionAngle <= DEG2RAD(45))
		{
			tagId = pTagDefinition->Find("ForwardRight");
		}
		else if (m_reactionAngle >= DEG2RAD(-135) && m_reactionAngle < DEG2RAD(-45))
		{
			tagId = pTagDefinition->Find("Left");
		}
		else if (m_reactionAngle > DEG2RAD(45) && m_reactionAngle <= DEG2RAD(135))
		{
			tagId = pTagDefinition->Find("Right");
		}
		else if (m_reactionAngle >= DEG2RAD(-180) && m_reactionAngle < DEG2RAD(-135))
		{
			tagId = pTagDefinition->Find("BackLeft");
		}
		else if (m_reactionAngle > DEG2RAD(135) && m_reactionAngle <= DEG2RAD(180))
		{
			tagId = pTagDefinition->Find("BackRight");
		}

		if (tagGroupId != TAG_ID_INVALID || tagId != TAG_ID_INVALID)
		{
			pTagDefinition->SetGroup(tagState, tagGroupId, tagId);
		}
		else
		{
			CryLog("Attackfrom tag group ID or group member ID could not be founded by hit reaction!");
		}

		// Hit Type Tag
		tagGroupId = pTagDefinition->FindGroup("HitType");
		tagId = pTagDefinition->Find(GetHitTypeString());
		if (tagGroupId != TAG_ID_INVALID && tagId != TAG_ID_INVALID)
		{
			pTagDefinition->SetGroup(tagState, tagGroupId, tagId);
		}
		else
		{
			CryLog("Hit type tag group ID or group member ID could not be founded by sword reaction!");
		}

		// Death Tag
		if (m_pActor->IsDead())
		{
			tagId = pTagDefinition->Find("Death");
			if (tagId != TAG_ID_INVALID)
			{
				pTagDefinition->Set(tagState, tagId, true);

				priority = PP_Death;
			}
			else
			{
				CryLog("Death tag ID could not be founded for sword reaction!");
			}
		}

		// Play Animation
		IActionPtr pAction = new CMoveAction(m_pActor, true, priority, fragID, tagState);
		pAC->Queue(*pAction);
	}
}


