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

class CSpawnPoint;

enum EWeaponType { ewt_magic, ewt_sword, ewt_knife };

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
	CPlayerAnimations * GetAnimationManager() const { return m_pAnimations; }
	CPlayerSword * GetSword() const { return m_pSword; }

	EWeaponType GetWeaponType() { return m_weaponType; }

	// Select weapon
	// isForce can be used while initializing to ensure set weapon tag
	void SelectWeapon(EWeaponType weaponType, bool isForce = false);

	void SetSpaceKeyStatus(bool isPressed) { m_bIsRun = isPressed; }
	bool IsSpaceKeyPressed() const { return m_bIsRun; }

	const virtual Vec3 GetMoveDirection() const override;
	const virtual float GetMoveAngle() const override;

	const SPlayerViewParams &GetPlayerViewParams() const { return m_playerViewParams; }

	void SwordAttack();

protected:

	virtual void RegisterCVars();
	virtual void UnregisterCVars();

protected:
	CPlayerInput		* m_pInput;
	CPlayerView			* m_pView;
	CPlayerAnimations	* m_pAnimations;
	CPlayerStateManager * m_pStateManager;
	CPlayerSword * m_pSword;

	EWeaponType m_weaponType;

	SPlayerViewParams m_playerViewParams;
};

#endif
