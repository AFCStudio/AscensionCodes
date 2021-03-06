/* -------------------------------------------------------------
-	AIEnemy.h - Header file
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

#ifndef __AIENEMY_H__
#define __AIENEMY_H__

#pragma once

#include "AIBase.h"

class CAIStateManager;

enum class EFightingGroup { NoFighting, Taunt, Attack };

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CAIEnemy
	: public CAIBase
{
	typedef CAIBase BaseClass;

public:
	CAIEnemy();
	virtual ~CAIEnemy();

	// ISimpleActor
	virtual void PostInit(IGameObject* pGameObject) override;
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override;
	// ~ISimpleActor

	// IEntityPropertyGroup
	virtual struct IEntityPropertyGroup* GetPropertyGroup() override { return this; }
	virtual const char*         GetLabel() const override { return "AI Enemy Properties"; }
	virtual void                SerializeProperties(Serialization::IArchive& archive) override;
	// ~IEntityPropertyGroup

	virtual void Attack();

	virtual CAIStateManager * GetStateManager() const { return m_pStateManager; }

	virtual const bool IsAttackGroup() const { return m_fightingGroup == EFightingGroup::Attack; }
	virtual const bool IsTauntGroup() const { return m_fightingGroup == EFightingGroup::Taunt; }
	virtual const EFightingGroup GetFightingGroup() const { return m_fightingGroup; }
	virtual void SetFightingGroup(EFightingGroup fightingGroup);

	virtual void SetAsAttackGroup() { SetFightingGroup(EFightingGroup::Attack); }
	virtual void SetAsTauntGroup() { SetFightingGroup(EFightingGroup::Taunt); }
	virtual void ClearFightingGroup() { SetFightingGroup(EFightingGroup::NoFighting); }

	virtual void ResetAttackTime();
	virtual const bool IsAttackTimeCompleted() const { return m_attackingTimer <= 0.0f; }
	virtual void RunOutAttackTime(float frameTime) { m_attackingTimer -= frameTime; }

	virtual void CancelAttack();

	virtual const Vec3 GetMoveDirection() const override;

	bool IsTargetAActor() { return m_pTargetActor != nullptr; }

	void AddToAIFightingSystem();
	void RemoveFromAIFightingSystem();

	virtual void DebugLog() override;

	// Animations and Mannequin
	virtual void SetActorMannequin() override;
	// ~Animations and Mannequin

protected:

	CActor * m_pTargetActor;

	EFightingGroup m_fightingGroup;

	CAIStateManager * m_pStateManager;

	int m_teamId;

	float m_attackingTimer;
	float m_reactionDelay;
	bool  m_isAttackInQueue;
};

#endif
