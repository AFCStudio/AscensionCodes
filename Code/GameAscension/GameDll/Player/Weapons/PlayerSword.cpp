/* -------------------------------------------------------------
-	WeaponMagicianSword.cpp - Source file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for a new weapon sword melee system.
-	for main magician character. That based on a weapon,
-   selecting auto when attacking, deselecting auto when idle.
-
-	Also that class have infinite combo system.
-  -------------------------------------------------------------
-	Creation Date: 28-04-2016
-	Edit     Date: 25-02-2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#include "StdAfx.h"
#include "PlayerSword.h"

#include "Player/Player.h"
#include "Player/Input/PlayerInput.h"
#include "Player/Animations/PlayerAnimations.h"

CPlayerSword::CPlayerSword()
	: m_bIsAttacking(false)
	, m_bIsInSequence(false)
	, m_bIsSequenceattackInQueued(false)
	, m_animationAngle(0)
	, m_fBlockMoveTime(0.0f)
	, m_fTimeSinceAction(0.0f)
{
	GetRandomSequences();
}

CPlayerSword::~CPlayerSword()
{
}

void CPlayerSword::PostInit(IGameObject * pGameObject)
{
	m_pPlayer = static_cast<CPlayer *>(pGameObject->QueryExtension("Player"));

	// Make sure that this extension is updated regularly via the Update function below
	pGameObject->EnableUpdateSlot(this, 0);
}

void CPlayerSword::Update(SEntityUpdateContext& ctx, int slot)
{
	// This check is required because the GameObjectExtention system calls Update 255 times at initialization time.
	if (m_bIsAttacking)
	{
		if (m_fBlockMoveTime > 0)
		{
			m_fBlockMoveTime -= ctx.fFrameTime;
		}
		else if (m_bIsSequenceattackInQueued)
		{
			m_bIsSequenceattackInQueued = false;
			SwordAttack();
		}

		m_fTimeSinceAction += ctx.fFrameTime;

		if (m_fTimeSinceAction >= 4)
			StopAttack();
	}
}

void CPlayerSword::SwordAttack()
{
	if (CanSwordAttack())	
	{
		m_pPlayer->GetInput()->BlockMove(true);

		CreateNextSequence();

		m_fTimeSinceAction = 0.0f;
		m_fBlockMoveTime = GetSwordAnimationBlockTime();

		m_pPlayer->GetAnimationManager()->PlaySwordAction();

		m_bIsAttacking = true;
		m_bIsInSequence = true;
	}
	else
		m_bIsSequenceattackInQueued = true;
}

bool CPlayerSword::CanSwordAttack() const
{
	return m_fBlockMoveTime <= 0 && !m_bIsSequenceattackInQueued;
}

void CPlayerSword::StopAttack()
{
	m_bIsInSequence = false;

	m_bIsSequenceattackInQueued = false;
	m_bIsAttacking = false;
	m_fBlockMoveTime = 0.0f;
	m_fTimeSinceAction = 0.0f;

	m_pPlayer->GetInput()->BlockMove(false);
}

void CPlayerSword::OnTransitionStartSwordAction()
{
	// That should be filled to set WeaponMagicianSword ReadyForNextAttack
	// that accept a new sword action to queue.

	// But first, we must split sequence animations to three from two;
	// First attack, transitions (standing), second attack
}

void CPlayerSword::OnSelected(bool selected)
{
	m_fBlockMoveTime = 0.0f;
	m_fTimeSinceAction = 0.0f;
}

void CPlayerSword::Select(bool select)
{
	OnSelected(select);
}

void CPlayerSword::RestoreWeapon()
{
	m_fBlockMoveTime = 0.0f;
	m_fTimeSinceAction = 0.0f;
}

void CPlayerSword::CreateNextSequence()
{
	float moveAngle;

	if (m_pPlayer->GetInput()->GetLocalMoveDirection() == ZERO)
		moveAngle = 0.0f;
	else
	{
		moveAngle = RAD2DEG(m_pPlayer->GetInput()->GetMoveAngle());
	}
	if (moveAngle >= - 45 && moveAngle <= 45)
	{
		m_animationAngle = 0;
		m_currentSequence = (ESwordSequence)cry_random(0, (int)ESS_L2R_U2D);
	}
	else if (moveAngle >= -135 && moveAngle < -45)
	{
		m_animationAngle = -90;
		m_currentSequence = (ESwordSequence)cry_random((int)EES_Right_D2U, (int)EES_Right_U2D);
	}
	else if (moveAngle > 45 && moveAngle <= 135)
	{
		m_animationAngle = 90;
		m_currentSequence = (ESwordSequence)cry_random((int)EES_Left_D2U, (int)EES_Left_U2D);
	}
	else if (moveAngle >= -180 && moveAngle < -135)
	{
		m_animationAngle = -180;
		m_currentSequence = (ESwordSequence)cry_random((int)EES_Right_Back_D2U, (int)EES_Right_Back_U2D);
	}
	else if (moveAngle > 135 && moveAngle <= 180)
	{
		m_animationAngle = 180;
		m_currentSequence = (ESwordSequence)cry_random((int)EES_Left_Back_D2U, (int)EES_Left_Back_U2D);
	}
}

void CPlayerSword::GetRandomSequences()
{
	m_currentSequence = (ESwordSequence)cry_random(0, (int)ESS_L2R_U2D);
}

char * CPlayerSword::GetTagIDNameFromCurrentSequence() const
{
	switch (m_currentSequence)
	{
	case ESS_R2L_D2U:	return "R2L_D2U";
	case ESS_L2R_D2U:	return "L2R_D2U";
	case ESS_R2L_M:		return "R2L_M";
	case ESS_L2R_M:		return "L2R_M";
	case ESS_R2L_U2D:	return "R2L_U2D";
	case ESS_L2R_U2D:	return "L2R_U2D";

	case EES_Right_D2U:	return "Right_D2U";
	case EES_Left_D2U:	return "Left_D2U";
	case EES_Right_M:	return "Right_M";
	case EES_Left_M:	return "Left_M";
	case EES_Right_U2D:	return "Right_U2D";
	case EES_Left_U2D: 	return "Left_U2D";

	case EES_Right_Back_D2U:	return "RightBack_D2U";
	case EES_Left_Back_D2U:	return "LeftBack_D2U";
	case EES_Right_Back_M:	return "RightBack_M";
	case EES_Left_Back_M:	return "LeftBack_M";
	case EES_Right_Back_U2D:	return "RightBack_U2D";
	case EES_Left_Back_U2D: 	return "LeftBack_U2D";

	default:			return NULL;
	}
}

float CPlayerSword::GetSwordAnimationBlockTime() const
{
	switch (m_currentSequence)
	{
	case ESS_R2L_D2U:	return 1.39f;
	case ESS_L2R_D2U:	return 1.39f;
	case ESS_R2L_M:		return 1.20f;
	case ESS_L2R_M:		return 1.39f;
	case ESS_R2L_U2D:	return 1.10f;
	case ESS_L2R_U2D:	return 1.10f;

	case EES_Right_D2U:	return 1.34f;
	case EES_Left_D2U:	return 1.29f;
	case EES_Right_M:	return 1.29f;
	case EES_Left_M:	return 1.29f;
	case EES_Right_U2D:	return 1.29f;
	case EES_Left_U2D: 	return 1.10f;

	case EES_Right_Back_D2U:	return 1.20f;
	case EES_Left_Back_D2U:		return 1.20f;
	case EES_Right_Back_M:		return 1.20f;	// Less
	case EES_Left_Back_M:		return 1.20f;
	case EES_Right_Back_U2D:	return 1.20f;	// Less
	case EES_Left_Back_U2D: 	return 1.20f;

	default:			return 0.0f;
	}
}
