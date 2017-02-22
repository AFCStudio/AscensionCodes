/* -------------------------------------------------------------
-	MoveForceAction.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for player move animations with
-		forced rotation according to animation.
-
-	Class use pe_action_move to move player according to
-	velocity.
-
-	That class behave as MoveAction except player rotation.
-
-  -------------------------------------------------------------
-	Creation Date: 05.05.2016
-	Edit     Date: 20.02.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */


#include "MoveForceAction.h"

CMoveForceAction::CMoveForceAction(
	IEntity * pEntity,
	int priority, FragmentID fragmentID, const TagState &fragTags,
	uint32 flags, ActionScopes scopeMask, uint32 userToken)
	:
	BaseClass(pEntity, priority, fragmentID, fragTags, flags, scopeMask, userToken)
{
}

IAction::EStatus CMoveForceAction::Update(float timePassed)
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

			// Rotation
			//SCharacterMoveRequest& request = pPlayer->GetMoveRequest();
			//request.rotation = pInstance->GetISkeletonAnim()->GetRelMovement().q;
		}
	}

	return BaseClass::Update(timePassed);
}

void CMoveForceAction::Enter()
{
	//CPlayerStateUtil::DisableTurning(true);

	BaseClass::Enter();
}

void CMoveForceAction::Exit()
{
	//CPlayerStateUtil::DisableTurning(false);

	BaseClass::Exit();
}
