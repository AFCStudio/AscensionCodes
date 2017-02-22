/* -------------------------------------------------------------
-	MoveAction.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for player moving animations.
-
-	Class use pe_action_move to move player according to
-	velocity. And rotate player according relative movemement.
-
-  -------------------------------------------------------------
-	Creation Date: 05.05.2016
-	Edit     Date: 20.02.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#include "MoveAction.h"

CMoveAction::CMoveAction(
	IEntity * pEntity,
	int priority, FragmentID fragmentID, const TagState &fragTags,
	uint32 flags, ActionScopes scopeMask, uint32 userToken)
	:
	BaseClass(priority, fragmentID, fragTags, flags, scopeMask, userToken)
	, m_pEntity(pEntity)
{
	if (!m_pEntity)
		CryLog("Error: Actor is NULL!");
}

IAction::EStatus CMoveAction::Update(float timePassed)
{
	if (m_pEntity)
	{
			ICharacterInstance * pInstance = m_pEntity->GetCharacter(0);

			if (pInstance)
			{
				// Location
				Vec3 velocity = pInstance->GetISkeletonAnim()->GetCurrentVelocity();

				Vec3 forwardDir = m_pEntity->GetForwardDir().normalized();
				Vec3 rightDir = forwardDir.cross(Vec3(0.0f, 0.0f, 1.0f)).normalized();

				Vec3 worldVelocity = velocity.y * forwardDir;
				worldVelocity += velocity.x * rightDir;

				pe_action_move move;
				move.dir = worldVelocity;
				move.iJump = 1;

				m_pEntity->GetPhysics()->Action(&move);
			}
	}

	return BaseClass::Update(timePassed);
}
