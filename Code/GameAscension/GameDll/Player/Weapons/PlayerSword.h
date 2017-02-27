/* -------------------------------------------------------------
-	PlayerSword.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		That class created for a new weapon sword melee system.
-	for main magician character. That based on a weapon.
-
-	Main targets are to store sequence time, blocking time,
-			next attack time.
-
-	Also that class have infinite combo system.
-  -------------------------------------------------------------
-	Creation Date: 28-04-2016
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */
#pragma once

#ifndef __PLAYER_SWORD_H__
#define __PLAYER_SWORD_H__

#include "Entities/Helpers/ISimpleExtension.h"

class CPlayer;

class CPlayerSword : public CGameObjectExtensionHelper<CPlayerSword, ISimpleExtension>
{
public:

	enum ESwordSequence
	{
		ESS_R2L_D2U,
		ESS_L2R_D2U,
		ESS_R2L_M,
		ESS_L2R_M,
		ESS_R2L_U2D,
		ESS_L2R_U2D,

		EES_Right_D2U,	// 1.03338, continue: 1.1248
		EES_Right_M,	// 0.93544
		EES_Right_U2D,

		EES_Right_Back_D2U,	// -0.97211
		EES_Right_Back_M,
		EES_Right_Back_U2D,

		EES_Left_D2U, // Single Hand: -0.93715, Two-hand: -0.84619 but there is Y a bit
		EES_Left_M,
		EES_Left_U2D,

		EES_Left_Back_D2U,
		EES_Left_Back_M,		// -0.50865
		EES_Left_Back_U2D,

		ESS_LAST	// Must be at last
	};

	ESwordSequence GetCurrentSequence() { return m_currentSequence; }

protected:

	virtual void PostInit(IGameObject * pGameObject);
	virtual void Update(SEntityUpdateContext& ctx, int slot);

	virtual void Select(bool select);
	virtual void OnSelected(bool selected);
	virtual bool CanSwordAttack() const;

public:

	CPlayerSword();
	virtual ~CPlayerSword();

	virtual void SwordAttack();
	virtual void StopAttack();

	char * GetTagIDNameFromCurrentSequence() const;

	virtual void	OnTransitionStartSwordAction();

	void	CreateNextSequence();
	void	GetRandomSequences();

	int		GetAnimationAngle() { return m_animationAngle; }

	float	GetSwordAnimationBlockTime() const;

	float	GetBlockTime() const { return m_fBlockMoveTime; }

	bool	IsInSequence() const { return m_bIsInSequence; }

private:
	void	RestoreWeapon();

private:

	bool	m_bIsInSequence;
	bool    m_bIsAttacking;
	bool	m_bIsSequenceattackInQueued;

	float	m_fBlockMoveTime;
	float	m_fTimeSinceAction;

	int		m_animationAngle;

	CPlayer * m_pPlayer;

	ESwordSequence m_currentSequence;
};

#endif // !__PLAYER_SWORD_H__

