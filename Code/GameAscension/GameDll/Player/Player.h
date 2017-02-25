#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma once

#include "Player/ISimpleActor.h"

#include <CryMath/Cry_Camera.h>

class CPlayerInput;
class CPlayerView;
class CPlayerAnimations;
class CPlayerStateManager;

class CPlayer;

class CSpawnPoint;

////////////////////////////////////////////////////////
// Represents a player participating in gameplay
////////////////////////////////////////////////////////
class CPlayer 
	: public CGameObjectExtensionHelper<CPlayer, ISimpleActor>
{
public:
	enum EGeometrySlots
	{
		eGeometry_ThirdPerson = 0,
	};

	struct SExternalCVars
	{
		float m_mass;
		float m_walkSpeed;
		float m_runSpeed;

		float m_rotationSpeedYaw;
		float m_rotationSpeedPitch;

		float m_rotationLimitsMinPitch;
		float m_rotationLimitsMaxPitch;

		float m_playerEyeHeight;

		ICVar *m_pThirdPersonGeometry;
		
		ICVar *m_pThirdPersonMannequinContext;
		ICVar *m_pThirdPersonAnimationDatabase;
		ICVar *m_pThirdPersonControllerDefinition;
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

	const SExternalCVars &GetCVars() const;


	void SetSpaceKeyStatus(bool isPressed) { m_bIsSpaceKey = isPressed; }
	bool IsSpaceKeyPressed() const { return m_bIsSpaceKey; }

	float GetMoveSpeed() const;

protected:
	void SelectSpawnPoint();
	void SetPlayerModel();

	void Physicalize();

protected:
	CPlayerInput		* m_pInput;
	CPlayerView			* m_pView;
	CPlayerAnimations	* m_pAnimations;
	CPlayerStateManager * m_pStateManager;

	bool m_bAlive;
	bool m_bIsSpaceKey;

};

#endif
