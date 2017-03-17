#include "StdAfx.h"
#include "Player.h"

#include "Input/PlayerInput.h"
#include "View/PlayerView.h"
#include "States/PlayerStateManager.h"
#include "Weapons/PlayerSword.h"

#include "GamePlugin.h"
#include "Game/GameRules.h"

#include "Actions/AttackAction.h"

#include "AI/Systems/AIFightingSystem.h"

#include "Entities/Gameplay/SpawnPoint.h"

#include <CryRenderer/IRenderAuxGeom.h>

class CPlayerRegistrator
	: public IEntityRegistrator
{
	virtual void Register() override
	{
		CGamePlugin::RegisterEntityWithDefaultComponent<CPlayer>("Player");

		CGamePlugin::RegisterEntityComponent<CPlayerInput>("PlayerInput");
		CGamePlugin::RegisterEntityComponent<CPlayerView>("PlayerView");
		CGamePlugin::RegisterEntityComponent<CPlayerStateManager>("PlayerStateManager");
		CGamePlugin::RegisterEntityComponent<CPlayerSword>("PlayerSword");
	}

	virtual void Unregister() override
	{
	}
};

CPlayerRegistrator g_playerRegistrator;

CPlayer::CPlayer()
	: m_pInput(nullptr)
	, m_pView(nullptr)
{
	m_bIsPlayer = true;
	m_weaponType = EWeaponType::Magic;

	RegisterCVars();

	m_pAIFightingSystem = new CAIFightingSystem();
}

CPlayer::~CPlayer()
{
	UnregisterCVars();

	SAFE_RELEASE(m_pActionController);
	SAFE_DELETE(m_pAnimationContext);

	gEnv->pGameFramework->GetIActorSystem()->RemoveActor(GetEntityId());
}

void CPlayer::RegisterCVars()
{
	REGISTER_CVAR2("pl_mass", &m_mass, 90.f, VF_CHEAT, "Mass of the player entity in kg");
	REGISTER_CVAR2("pl_walkSpeed", &m_walkSpeed, 1.8f, VF_CHEAT, "Player walking speed");
	REGISTER_CVAR2("pl_runSpeed", &m_runSpeed, 3.7f, VF_CHEAT, "Player running speed");
	REGISTER_CVAR2("pl_rotationSpeed", &m_turnSpeed, 7.0f, VF_CHEAT, "Player rotation speed while moving");

	REGISTER_CVAR2("pl_rotationSpeedYaw", &m_playerViewParams.m_rotationSpeedYaw, 0.05f, VF_CHEAT, "Speed at which the player rotates entity yaw");
	REGISTER_CVAR2("pl_rotationSpeedPitch", &m_playerViewParams.m_rotationSpeedPitch, 0.05f, VF_CHEAT, "Speed at which the player rotates entity pitch");

	REGISTER_CVAR2("pl_rotationLimitsMinPitch", &m_playerViewParams.m_rotationLimitsMinPitch, -0.84f, VF_CHEAT, "Minimum entity pitch limit");
	REGISTER_CVAR2("pl_rotationLimitsMaxPitch", &m_playerViewParams.m_rotationLimitsMaxPitch, 1.5f, VF_CHEAT, "Maximum entity pitch limit");

	REGISTER_CVAR2("pl_eyeHeight", &m_actorEyeHeight, 0.935f, VF_CHEAT, "Height of the player's eyes from ground");
}

void CPlayer::UnregisterCVars()
{
	IConsole* pConsole = gEnv->pConsole;
	if (pConsole)
	{
		pConsole->UnregisterVariable("pl_mass");
		pConsole->UnregisterVariable("pl_walkSpeed");
		pConsole->UnregisterVariable("pl_runSpeed");
		pConsole->UnregisterVariable("pl_rotationSpeed");
		pConsole->UnregisterVariable("pl_rotationSpeedYaw");
		pConsole->UnregisterVariable("pl_rotationSpeedPitch");
		pConsole->UnregisterVariable("pl_rotationLimitsMinPitch");
		pConsole->UnregisterVariable("pl_rotationLimitsMaxPitch");
		pConsole->UnregisterVariable("pl_eyeHeight");
	}
}

bool CPlayer::Init(IGameObject *pGameObject)
{
	SetGameObject(pGameObject);

	return pGameObject->BindToNetwork();
}

void CPlayer::PostInit(IGameObject *pGameObject)
{
	m_pInput = static_cast<CPlayerInput *>(GetGameObject()->AcquireExtension("PlayerInput"));
	m_pStateManager = static_cast<CPlayerStateManager *>(GetGameObject()->AcquireExtension("PlayerStateManager"));
	m_pSword = static_cast<CPlayerSword *>(GetGameObject()->AcquireExtension("PlayerSword"));

	m_pView = static_cast<CPlayerView *>(GetGameObject()->AcquireExtension("PlayerView"));

	// Register with the actor system
	gEnv->pGameFramework->GetIActorSystem()->AddActor(GetEntityId(), this);

	pGameObject->EnableUpdateSlot(this, 0);
}

void CPlayer::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
	case ENTITY_EVENT_RESET:
	{
		switch (event.nParam[0])
		{
		case 0: // Game ends
			break;
		case 1: // Game starts
				// Make sure to revive player when respawning in Editor

			// Set camera X and Y rotation to 0 angle while initializing.
			GetEntity()->SetRotation(Quat::CreateRotationZ(GetEntity()->GetRotation().GetRotZ()));

			SetHealth(GetMaxHealth());
			break;
		default:
			break;
		}
	}
	case ENTITY_EVENT_ANIM_EVENT:
	{
		const AnimEventInstance *pAnimEvent = reinterpret_cast<const AnimEventInstance *>(event.nParam[0]);
		ICharacterInstance *pCharacter = reinterpret_cast<ICharacterInstance *>(event.nParam[1]);

		m_pActionController->OnAnimationEvent(pCharacter, *pAnimEvent);
	}
	break;
	}
}

void CPlayer::SetHealth(float health)
{
	CActor::SetHealth(health);

	// Notify input that the player respawned
	m_pInput->OnPlayerRespawn();
}

void CPlayer::AddEnemyFighter(CAIEnemy * pEnemy)
{
	if (m_pAIFightingSystem)
		m_pAIFightingSystem->AddFighter(pEnemy);
	else
		CryLog("CPlayer::AIFighter Warning: Player has not a AIFighting system yet!");
}

void CPlayer::RemoveEnemyFighter(CAIEnemy * pEnemy)
{
	if (m_pAIFightingSystem)
		m_pAIFightingSystem->RemoveFighter(pEnemy);
}

void CPlayer::AddAttackToQueue(CAIEnemy * pEnemy)
{
	if (m_pAIFightingSystem)
		m_pAIFightingSystem->PushAttacking(pEnemy);
}

void CPlayer::RemoveAttackFromQueue(CAIEnemy * pEnemy)
{
	if (m_pAIFightingSystem)
		m_pAIFightingSystem->RemoveAttacking(pEnemy);
}

const Vec3 CPlayer::GetMoveDirection() const
{
	return GetInput()->GetMoveDirection();
}

const float CPlayer::GetMoveAngle() const
{ 
	return GetInput()->GetMoveAngle();
}

const CAIEnemy * CPlayer::IdentifyTargetEnemy() const
{
	return m_pAIFightingSystem ? m_pAIFightingSystem->IdentifyTargetEnemy(this) : nullptr;
}

const CAIEnemy * CPlayer::GetTargetEnemy() const
{
	return m_pAIFightingSystem->GetTargetEnemy();
}

const float CPlayer::GetTargetEnemyAngle() const
{
	return m_pAIFightingSystem->GetTargetAngle();
}

void CPlayer::SwordAttack()
{
	if (m_weaponType == EWeaponType::Sword)
		m_pSword->SwordAttack();
}

void CPlayer::DefendAttack()
{
	if (m_pAIFightingSystem)
	{
		CActor * pEnemy = (CActor*)m_pAIFightingSystem->PopAttacking();

		if (pEnemy)
		{
			Enslave(pEnemy);

			SetTag("slaveHuman", true);

			ForceFinishLastAction();
			PlayMoveAction("Defend", true, PP_Defend);
		}
	}
}

// Animation and Mannequin
// ------------------------------------------------------------------------

void CPlayer::SetActorMannequin()
{
	CActor::SetActorMannequin();

	SetWeaponTag(EWeaponType::Magic);
}

//------------------------------------------------------------------------
void CPlayer::PlaySwordAction(EActionPriority priority)
{
	if (m_pActionController)
	{
		FragmentID fragID = m_pActionController->GetContext().controllerDef.m_fragmentIDs.Find("melee_weapon");

		const CTagDefinition * const pTagDefinition = m_pActionController->GetTagDefinition(fragID);
		TagState tagState = TAG_STATE_EMPTY;

		TagGroupID tagGroupId = pTagDefinition->FindGroup("combatSequence");
		TagID tagId = pTagDefinition->Find(m_pSword->GetTagIDNameFromCurrentSequence());

		if (tagGroupId != TAG_ID_INVALID || tagId != TAG_ID_INVALID)
		{
			pTagDefinition->SetGroup(tagState, tagGroupId, tagId);
		}
		else
		{
			CryLog("Player sword tag group ID or group member ID could not be founded by sword PlaySwordAction!");
		}

		tagId = pTagDefinition->Find("inSequence");
		if (tagId != TAG_ID_INVALID)
		{
			pTagDefinition->Set(tagState, tagId, m_pSword->IsInSequence());
		}
		else
		{
			CryLog("Player sword sequence tag ID could not be founded by sword PlaySwordAction!");
		}

		m_pLastAction = new CAttackAction(this, priority, fragID, tagState);
		m_pActionController->Queue(*m_pLastAction);
	}
}

// ------------------------------------------------------------------------
// ~Animation and Mannequin

