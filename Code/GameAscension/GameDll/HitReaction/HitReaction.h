/* -------------------------------------------------------------
-	HitReaction.h - Header file
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

#ifndef __HIT_REACTION_H__
#define __HIT_REACTION_H__

#pragma once

class CActor;

enum class EHitTypes { Melee, Sword, Dagger, MagicBall };

class CHitReaction
{

public:
	CHitReaction(CActor * pActor);
	virtual ~CHitReaction() {}

	// Hit Reaction funciton calculate reaction direction and angle
	// using pAttacker entity.
	// That function also calculate damage and set new health
	// Finally play raection action with calculated tags.
	void HitReaction(IEntity * pAttacker, EHitTypes hitType);

private:
	void   PlayReactionAction();

	char * GetHitTypeString();
	void   CalculateDamage();

private:
	CActor    * m_pActor;

	Vec3		m_reactionDir;
	float		m_reactionAngle;
	float		m_damage;
	EHitTypes	m_hitType;
};

#endif
