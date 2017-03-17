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
class CAIEnemy;
class CHitReaction;
class CSpawnPoint;

enum class EHitTypes;

enum class EWeaponType { NoWeapon, Magic, Sword, Knife };

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
		string m_pActorMannequinContext;
		string m_pActorAnimationDatabase;
		string m_pActorControllerDefinition;
		string m_pSlaveAnimationDatabase;
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

	virtual void SetRunning(bool running) { m_bIsRun = running; }
	virtual bool IsRunning() const { return m_bIsRun; }

	virtual const EWeaponType GetWeaponType() const { return m_weaponType; }
	virtual void SelectWeapon(EWeaponType weaponType);

	virtual CHitReaction * GetHitReaction() const { return m_pHitReaction; }
	virtual void HitReaction(IEntity * pAttacker, EHitTypes hitType) const;
	// That function use actor weapon to decide hit type
	virtual void HitReaction(IEntity * pAttacker) const;
	virtual EHitTypes GetHitTypeFromCurrentWeapon() const;

	const virtual SActorMannequinInfo &GetActorMannequinInfo() const { return m_actorMannequinInfo; }

	// This function defined to add enemy to AI fighting system.
	// Initially should be only used by Player.
	virtual void AddEnemyFighter(CAIEnemy * pEnemy) {}

	// This function defined to remove enemy from AI fighting system.
	// Initially should be only used by Player.
	virtual void RemoveEnemyFighter(CAIEnemy * pEnemy) {}

	// This function defined to add enemy to AI fighting system attacking queue.
	// Initially should be only used by Player.
	virtual void AddAttackToQueue(CAIEnemy * pEnemy) {}

	// This function defined to remove enemy from AI fighting system attacking queue.
	// Initially should be only used by Player.
	virtual void RemoveAttackFromQueue(CAIEnemy * pEnemy) {}

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

	CHitReaction * m_pHitReaction;

	string m_pCharacterGeometry;

	SActorMannequinInfo m_actorMannequinInfo;

	// Animations and Mannequin
public:

	virtual void SetActorMannequin();

	virtual void UpdateActorTurnAngle();

	// Activate or deactivate a global actor tag its using name
	virtual void SetTag(char * pTagName, bool enable);

	virtual void SetTagGroup(TagGroupID groupId, TagID tagId);
	virtual void ClearTagGroup(TagGroupID groupId);

	virtual void PlayIdleAction(char * fragmentName = "MotionIdle", EActionPriority priority = PP_Lowest, TagState fragTags = TAG_STATE_EMPTY);

	virtual void PlayMoveAction(char * fragmentName, bool isRotationForce, EActionPriority priority = PP_Movement, TagState fragTags = TAG_STATE_EMPTY, uint32 flags = 0);
	virtual void PlayMovementAction(char * fragmentName, EActionPriority priority = PP_Movement, TagState fragTags = TAG_STATE_EMPTY, uint32 flags = 0);

	virtual void PlayFragment(FragmentID fragmentID, EActionPriority priority, TagState tagState = TAG_STATE_EMPTY, uint32 flags = 0);
	virtual void PlayFragment(char * fragmentName, EActionPriority priority, TagState tagState = TAG_STATE_EMPTY, uint32 flags = 0);

	virtual void ForceFinishLastAction();

	virtual void Enslave(CActor * pTarget);
	virtual void ReleaseSlave();

	virtual IActionController * GetActionController() const { return m_pActionController; }
	virtual SAnimationContext * GetAnimationContext() const { return m_pAnimationContext; }

protected:
	void ActivateMannequinContext(const char *contextName, ICharacterInstance &character, const SControllerDef &controllerDefinition, const IAnimationDatabase &animationDatabase);

	virtual void SetWeaponTag(EWeaponType weaponType);

protected:

	CActor * m_pSlaveActor;

	const IAnimationDatabase * m_pSlaveADB;

	IActionController * m_pActionController;
	SAnimationContext * m_pAnimationContext;

	IActionPtr	m_pLastAction;

	EWeaponType m_weaponType;

	// ~Animations and Mannequin
};

#endif
