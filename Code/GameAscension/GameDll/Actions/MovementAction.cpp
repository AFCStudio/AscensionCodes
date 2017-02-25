/* -------------------------------------------------------------
-	MovementAction.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for player movement mechanic.
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

#include "MovementAction.h"
#include "Player/Player.h"
#include "Player/Input/PlayerInput.h"

#include <CryGame/GameUtils.h>		// For Interpolate


CMovementAction::CMovementAction(
	CPlayer * pPlayer,
	int priority, FragmentID fragmentID, const TagState &fragTags,
	uint32 flags, ActionScopes scopeMask, uint32 userToken)
	:
	BaseClass(priority, fragmentID, fragTags, flags, scopeMask, userToken)
	, m_pPlayer(pPlayer)
{
	if (!m_pPlayer)
		CryLog("Error: Actor is NULL!");
}

IAction::EStatus CMovementAction::Update(float timePassed)
{
	if (m_pPlayer)
	{
		IEntity * pEntity = m_pPlayer->GetEntity();

		ICharacterInstance * pCharacter = pEntity->GetCharacter(0);

		if (pCharacter)
		{
			float moveSpeed = m_pPlayer->GetMoveSpeed();
			pCharacter->GetISkeletonAnim()->SetDesiredMotionParam(eMotionParamID_TravelSpeed, moveSpeed, 0.f);

			Vec3 moveDir = m_pPlayer->GetInput()->GetMoveDirection();

			Vec3 currentDir = pEntity->GetForwardDir();
			Interpolate(currentDir, moveDir, m_pPlayer->GetCVars().m_rotationSpeed, timePassed);
			currentDir.z = 0;
			currentDir.Normalize();

			Quat targetOrientation = Quat::CreateRotationVDir(currentDir);

			pEntity->SetRotation(targetOrientation);

			pe_action_move move;
			move.dir = currentDir * moveSpeed;
			move.iJump = 1;

			pEntity->GetPhysics()->Action(&move);
		}
	}

	return BaseClass::Update(timePassed);
}