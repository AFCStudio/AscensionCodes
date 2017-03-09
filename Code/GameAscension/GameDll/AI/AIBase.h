/* -------------------------------------------------------------
-	AIBase.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		AIBase class, that class defines a general AI
-	and its behviors.
-
-  -------------------------------------------------------------
-	Creation Date: 02-03-2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __AIBASE_H__
#define __AIBASE_H__

#pragma once

#include "Actor/Actor.h"

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CAIBase
	: public CGameObjectExtensionHelper<CAIBase, CActor>
	, public IEntityPropertyGroup
{
	typedef CActor BaseClass;

public:
	CAIBase();
	virtual ~CAIBase();

	// ISimpleActor
	virtual void PostInit(IGameObject* pGameObject) override;
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override;
	// ~ISimpleActor

	// IEntityPropertyGroup
	virtual struct IEntityPropertyGroup* GetPropertyGroup() override { return this; }
	virtual const char*         GetLabel() const override { return "AI Properties"; }
	virtual void                SerializeProperties(Serialization::IArchive& archive) override;
	// ~IEntityPropertyGroup

	virtual const bool IsThereTarget() const { return m_pTargetEntity != nullptr; }
	// Target is used walking some location for AIBase.
	virtual void SetTarget(IEntity * pTarget) { m_pTargetEntity = pTarget; }

	// Get target distance which calculated per frame in AIBase.
	virtual const float GetTargetDistance() const { return m_targetDistance; }
	virtual void CalculateTargetDistance();

	// This function return stop distance before the reach the target.
	// This function also use taunt and attack distance by inherited enemy class.
	virtual const float GetStopDistance() const;

	// Stop offset is used to stop AI before reach a target.
	virtual void SetStopOffset(float offset) { m_stopOffset = offset; }

	virtual const Vec3  GetMoveDirection() const override;
	virtual const float GetMoveAngle() const override;

	// Animations and Mannequin
	virtual void SetActorMannequin() override;
	// ~Animations and Mannequin

protected:

	float m_targetDistance;
	float m_stopOffset;

	IEntity * m_pTargetEntity;
};

#endif
