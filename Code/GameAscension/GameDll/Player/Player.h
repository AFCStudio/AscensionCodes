#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma once

#include "Actor/Actor.h"

#include <CryMath/Cry_Camera.h>

class CPlayerInput;
class CPlayerView;
class CPlayerAnimations;
class CPlayerStateManager;
class CPlayerSword;

class CPlayer;
class CAIEnemy;

class CAIFightingSystem;

class CSpawnPoint;

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayer 
	: public CGameObjectExtensionHelper<CPlayer, CActor>
{
public:

	struct SPlayerViewParams
	{
		float m_rotationSpeedYaw;
		float m_rotationSpeedPitch;

		float m_rotationLimitsMinPitch;
		float m_rotationLimitsMaxPitch;
	};

public:
	CPlayer();
	virtual ~CPlayer();

	// ISimpleActor
	virtual bool Init(IGameObject* pGameObject) override;
	virtual void PostInit(IGameObject* pGameObject) override;
	virtual void ProcessEvent(SEntityEvent& event) override;

	virtual void SetHealth(float health) override;
	virtual float GetHealth() const override { return m_bAlive ? GetMaxHealth() : 0.f; }
	// ~ISimpleActor

	CPlayerInput * GetInput() const { return m_pInput; }
	CPlayerStateManager * GetStateManager() const { return m_pStateManager; }
	CPlayerSword * GetSword() const { return m_pSword; }

	CAIFightingSystem * GetAIFightingSystem() { return m_pAIFightingSystem; }

	virtual void AddEnemyFighter(CAIEnemy * pEnemy) override;
	virtual void RemoveEnemyFighter(CAIEnemy * pEnemy) override;

	virtual void AddAttackToQueue(CAIEnemy * pEnemy) override;
	virtual void RemoveAttackFromQueue(CAIEnemy * pEnemy) override;

	const virtual Vec3 GetMoveDirection() const override;
	const virtual float GetMoveAngle() const override;

	const SPlayerViewParams &GetPlayerViewParams() const { return m_playerViewParams; }

	// Called when creating next sequence from PlayerSword
	// to identify whether there is a AI enemy target or not.
	const CAIEnemy * IdentifyTargetEnemy() const;
	const CAIEnemy * GetTargetEnemy() const;
	const float GetTargetEnemyAngle() const;

	void SwordAttack();

	void DefendAttack();

	// Animations and Mannequin
	virtual void SetActorMannequin() override;

	void PlaySwordAction(EActionPriority priority = PP_Attack);
	// ~Animations and Mannequin

protected:

	virtual void RegisterCVars();
	virtual void UnregisterCVars();

protected:
	CPlayerInput		* m_pInput;
	CPlayerView			* m_pView;
	CPlayerStateManager * m_pStateManager;
	CPlayerSword * m_pSword;

	SPlayerViewParams m_playerViewParams;

	CAIFightingSystem * m_pAIFightingSystem;
};

#endif
