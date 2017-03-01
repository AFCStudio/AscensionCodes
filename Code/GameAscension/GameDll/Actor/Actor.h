/* -------------------------------------------------------------
-	Actor.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		Actor class is base class for Player and AI.
-	That class also has common functions including mannequin
-	animation system.
-
-  -------------------------------------------------------------
-	Creation Date: 27-02-2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __ACTOR_H__
#define __ACTOR_H__

#pragma once

#include "ISimpleActor.h"
#include "Actions/ActionBase.h"

class CActorAnimations;
class CActor;
class CSpawnPoint;

////////////////////////////////////////////////////////
// Represents a base of human participating in gameplay
////////////////////////////////////////////////////////
class CActor
	: public ISimpleActor
{
public:
	enum EGeometrySlots
	{
		eGeometry_Character = 0,
	};

	struct SActorMannequinInfo
	{
		char *m_pActorMannequinContext;
		char *m_pActorAnimationDatabase;
		char *m_pActorControllerDefinition;
	};

public:
	CActor();
	virtual ~CActor();

	// ISimpleActor
	virtual bool Init(IGameObject* pGameObject) override;
	virtual void PostInit(IGameObject* pGameObject) override;
	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override;
	virtual void ProcessEvent(SEntityEvent& event) override;

	virtual void SetHealth(float health) override;
	virtual float GetHealth() const override { return m_bAlive ? GetMaxHealth() : 0.f; }
	// ~ISimpleActor

	const virtual Vec3 GetMoveDirection() const;
	const virtual float GetMoveAngle() const;
	const virtual float &GetMoveSpeed() const;
	const virtual float &GetTurnSpeed() const { return m_turnSpeed; }
	const virtual char* GetCharacterModelName() const { return m_pCharacterGeometry; }

	virtual bool IsRunning() const { return m_bIsRun; }

	const virtual SActorMannequinInfo &GetActorMannequinInfo() const { return m_actorMannequinInfo; }

protected:
	virtual void SelectSpawnPoint();

	virtual void SetActorModel();

	virtual void Physicalize();

protected:

	bool m_bAlive;
	bool m_bIsRun;
	bool m_bIsPlayer;

	float m_mass;
	float m_walkSpeed;
	float m_runSpeed;
	float m_turnSpeed;

	float m_actorEyeHeight;

	char *m_pCharacterGeometry;

	SActorMannequinInfo m_actorMannequinInfo;

	// Animations and Mannequin
public:

	virtual void SetActorMannequin();

	virtual void UpdateActorTurnAngle();

	void SetTagGroup(TagGroupID groupId, TagID tagId);
	void ClearTagGroup(TagGroupID groupId);

	void PlayMoveAction(char * fragmentName, bool isRotationForce, EPlayerActionPriority priority, TagState fragTags = TAG_STATE_EMPTY);
	void PlayMovementAction(char * fragmentName, EPlayerActionPriority priority, TagState fragTags = TAG_STATE_EMPTY);

	void PlayFragment(FragmentID fragmentID, EPlayerActionPriority priority, TagState tagState = TAG_STATE_EMPTY);
	void PlayFragment(char * fragmentName, EPlayerActionPriority priority, TagState tagState = TAG_STATE_EMPTY);

	void ForceFinishLastAction();

protected:
	void ActivateMannequinContext(const char *contextName, ICharacterInstance &character, const SControllerDef &controllerDefinition, const IAnimationDatabase &animationDatabase);

protected:

	IActionController * m_pActionController;
	SAnimationContext * m_pAnimationContext;

	IActionPtr	m_pLastAction;

	// ~Animations and Mannequin
};

#endif
