/* -------------------------------------------------------------
-	AttackAction.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-		That action class deals with attack animation move
-	and position and rotation alignment according to enemy.
-
-  -------------------------------------------------------------
-	Creation Date: 13.03.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#include "AttackAction.h"

#include "Player/Player.h"
#include "AI/AIEnemy.h"
#include "Game/GameCVars.h"

#include "HitReaction/HitReaction.h"

CAttackAction::CAttackAction(
	CPlayer * pPlayer,
	int priority, FragmentID fragmentID, const TagState &fragTags,
	uint32 flags, ActionScopes scopeMask, uint32 userToken)
	:
	BaseClass(priority, fragmentID, fragTags, flags, scopeMask, userToken)
	, m_pPlayer(pPlayer)
	, m_initialRot(ZERO)
	, m_totalRot(0.0f)
	, m_targetAngle(0.0f)
	, m_hitReactionTimer(0.0f)
	, m_isReactionDone(false)
{
	if (!m_pPlayer)
		CryLog("Error: Actor is NULL!");
}

IAction::EStatus CAttackAction::Update(float timePassed)
{
	if (m_pPlayer)
	{
			IEntity * pEntity = m_pPlayer->GetEntity();

			ICharacterInstance * pCharacter = pEntity->GetCharacter(0);

			if (pCharacter)
			{
				const CAIEnemy * pEnemy = m_pPlayer->GetTargetEnemy();

				if (pEnemy)
				{

					if (!m_isReactionDone)
					{
						m_hitReactionTimer -= timePassed;

						if (m_hitReactionTimer <= 0.0)
						{
							m_isReactionDone = true;

							if (TargetIsInRange(pEnemy->GetEntity()))
								pEnemy->HitReaction(pEntity, EHitTypes::Sword);
						}
					}

					Vec3 direction = pEnemy->GetEntity()->GetPos() - pEntity->GetPos();
					float distance2D = direction.GetLength2D() - g_pGameCVars->pl_alignDistance;

					if (abs(distance2D) > g_pGameCVars->pl_alignThreshold)
					{
						direction.z = 0.0f;
						direction.Normalize();

						if (distance2D < 0)
							direction *= -1;

						Vec3 velocity = pCharacter->GetISkeletonAnim()->GetCurrentVelocity();

						pe_action_move move;
						move.dir = direction * velocity.GetLength2D() * g_pGameCVars->pl_alignSpeed;
						move.iJump = 1;

						pEntity->GetPhysics()->Action(&move);
					}

					if (m_targetAngle != m_totalRot)
					{
						Quat relMovement = pCharacter->GetISkeletonAnim()->GetRelMovement().q;

						m_totalRot += relMovement.GetRotZ();

						if (m_targetAngle >= 0)
							m_totalRot = min(m_totalRot, m_targetAngle);
						else
							m_totalRot = max(m_totalRot, m_targetAngle);

						pEntity->SetRotation(m_initialRot * Quat::CreateRotationZ(m_totalRot));
					}
				}
				else    // If there is no a target enemy, then just translate player according to locator.
				{
					Vec3 velocity = pCharacter->GetISkeletonAnim()->GetCurrentVelocity();

					Vec3 forwardDir = pEntity->GetForwardDir().normalized();
					Vec3 rightDir = forwardDir.cross(Vec3(0.0f, 0.0f, 1.0f)).normalized();

					Vec3 worldVelocity = velocity.y * forwardDir;
					worldVelocity += velocity.x * rightDir;

					pe_action_move move;
					move.dir = worldVelocity;
					move.iJump = 1;

					pEntity->GetPhysics()->Action(&move);

					Quat relMovement = pCharacter->GetISkeletonAnim()->GetRelMovement().q;

					float relAngle = relMovement.GetRotZ();

					m_totalRot += relAngle;
					pEntity->SetRotation(m_initialRot * Quat::CreateRotationZ(m_totalRot));
				}
			}
	}

	return BaseClass::Update(timePassed);
}

void CAttackAction::Enter()
{
	m_totalRot = 0.0f;

	m_initialRot = m_pPlayer->GetEntity()->GetRotation();
	m_targetAngle = m_pPlayer->GetTargetEnemyAngle();

	m_hitReactionTimer = g_pGameCVars->pl_hitReactionDelay;
	m_isReactionDone = false;
}

bool CAttackAction::TargetIsInRange(IEntity * pEnemy)
{
	return m_pPlayer->GetEntity()->GetPos().GetSquaredDistance2D(pEnemy->GetPos()) <=
		g_pGameCVars->ai_attackDistance + g_pGameCVars->ai_distanceThreshold;
}
