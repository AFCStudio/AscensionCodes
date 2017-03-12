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

#include "StdAfx.h"
#include "Actor.h"

#include "GamePlugin.h"
#include "Game/GameRules.h"

#include "Actions/MoveAction.h"
#include "Actions/MovementAction.h"

#include "Entities/Gameplay/SpawnPoint.h"

CActor::CActor()
	: m_bAlive(false)
	, m_bIsRun(false)
	, m_bIsPlayer(false)
	, m_mass(90.f)
	, m_walkSpeed(1.8f)
	, m_runSpeed(3.7f)
	, m_turnSpeed(7.0f)
	, m_actorEyeHeight(0.935f)
	, m_pActionController(nullptr)
	, m_pAnimationContext(nullptr)
	, m_pSlaveActor(nullptr)
	, m_weaponType(EWeaponType::NoWeapon)
{
	m_pCharacterGeometry = "Objects/Characters/TheMagician/TheMagician.cdf";

	m_actorMannequinInfo.m_pActorMannequinContext = "Char3P";
	m_actorMannequinInfo.m_pActorAnimationDatabase = "Animations/Mannequin/ADB/MagicianDataBase.adb";
	m_actorMannequinInfo.m_pActorControllerDefinition = "Animations/Mannequin/ADB/MagicianControllerDefs.xml";
	m_actorMannequinInfo.m_pSlaveAnimationDatabase = "Animations/Mannequin/ADB/AISlave.adb";
}

CActor::~CActor()
{
	SAFE_RELEASE(m_pActionController);
	SAFE_DELETE(m_pAnimationContext);
}

bool CActor::Init(IGameObject *pGameObject)
{
	SetGameObject(pGameObject);

	return pGameObject->BindToNetwork();
}

void CActor::PostInit(IGameObject *pGameObject)
{
	// Register with the actor system
	gEnv->pGameFramework->GetIActorSystem()->AddActor(GetEntityId(), this);
}

void CActor::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	UpdateActorTurnAngle();

	if (m_pActionController != nullptr)
	{
		m_pActionController->Update(ctx.fFrameTime);
	}
}

void CActor::ProcessEvent(SEntityEvent& event)
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

void CActor::SetHealth(float health)
{
	// Find a spawn point and move the entity there
	SelectSpawnPoint();

	// Note that this implementation does not handle the concept of death, SetHealth(0) will still revive the player.
	if (m_bAlive)
		return;

	m_bAlive = true;

	// Unhide the entity in case hidden by the Editor
	GetEntity()->Hide(false);

	// Make sure that the player spawns upright
	GetEntity()->SetWorldTM(Matrix34::Create(Vec3(1, 1, 1), IDENTITY, GetEntity()->GetWorldPos()));

	// Set the player geometry, this also triggers physics proxy creation
	SetActorModel();
}

void CActor::SetActorModel()
{
	// Load the third person model
	GetEntity()->LoadCharacter(eGeometry_Character, m_pCharacterGeometry);

	// Do the same for animations so that Mannequin data can be initialized
	SetActorMannequin();

	// Now create the physical representation of the entity
	Physicalize();
}

const Vec3 CActor::GetMoveDirection() const
{
	return Vec3();
}

const float CActor::GetMoveAngle() const
{
	return 0.0f;
}

const float &CActor::GetMoveSpeed() const
{
	return m_bIsRun ? m_runSpeed : m_walkSpeed;
}

void CActor::SelectWeapon(EWeaponType weaponType)
{
	if (m_weaponType != weaponType)
	{
		m_weaponType = weaponType;

		SetWeaponTag(weaponType);

		if (weaponType == EWeaponType::Sword)
		{
			PlayFragment("SelectSword", PP_Sword);
		}
		else
		{
			PlayFragment("DeSelectSword", PP_Sword);
		}
	}
}

void CActor::SelectSpawnPoint()
{
	// We only handle default spawning below for the Launcher
	// Editor has special logic in CEditorGame
	if (gEnv->IsEditor())
		return;

	// Spawn at first default spawner
	auto *pEntityIterator = gEnv->pEntitySystem->GetEntityIterator();
	pEntityIterator->MoveFirst();

	auto *pSpawnerClass = gEnv->pEntitySystem->GetClassRegistry()->FindClass("SpawnPoint");
	auto extensionId = gEnv->pGameFramework->GetIGameObjectSystem()->GetID("SpawnPoint");

	while (!pEntityIterator->IsEnd())
	{
		IEntity *pEntity = pEntityIterator->Next();

		if (pEntity->GetClass() != pSpawnerClass)
			continue;

		auto *pGameObject = gEnv->pGameFramework->GetGameObject(pEntity->GetId());
		if (pGameObject == nullptr)
			continue;

		auto *pSpawner = static_cast<CSpawnPoint *>(pGameObject->QueryExtension(extensionId));
		if (pSpawner == nullptr)
			continue;

		pSpawner->SpawnEntity(*GetEntity());

		break;
	}
}

void CActor::Physicalize()
{
	// Physicalize the player as type Living.
	// This physical entity type is specifically implemented for players
	SEntityPhysicalizeParams physParams;
	physParams.type = PE_LIVING;

	physParams.mass = m_mass;

	pe_player_dimensions playerDimensions;

	// Prefer usage of a cylinder instead of capsule
	playerDimensions.bUseCapsule = 0;

	// Specify the size of our cylinder
	playerDimensions.sizeCollider = Vec3(0.45f, 0.45f, m_actorEyeHeight * 0.5f);

	// Keep pivot at the player's feet (defined in player geometry) 
	playerDimensions.heightPivot = 0.f;
	// Offset collider upwards
	playerDimensions.heightCollider = 1.f;
	playerDimensions.groundContactEps = 0.004f;

	physParams.pPlayerDimensions = &playerDimensions;

	pe_player_dynamics playerDynamics;
	playerDynamics.kAirControl = 0.f;
	playerDynamics.mass = physParams.mass;

	physParams.pPlayerDynamics = &playerDynamics;

	GetEntity()->Physicalize(physParams);
}

// Animations and Mannequin
// ------------------------------------------------------------------
void CActor::Enslave(CActor * pTarget)
{
	m_pSlaveActor = pTarget;

	IActionController * pTargetAC = pTarget->GetActionController();
	if (pTargetAC)
	{
		uint32 targetScopeId = m_pAnimationContext->controllerDef.m_scopeContexts.Find("SlaveChar");

		m_pActionController->SetSlaveController(*pTargetAC, targetScopeId, true, m_pSlaveADB);
	}
}
void CActor::ReleaseSlave()
{
	if (m_pSlaveActor)
	{
		IActionController * pTargetAC = m_pSlaveActor->GetActionController();
		if (pTargetAC)
		{
			uint32 targetScopeId = m_pAnimationContext->controllerDef.m_scopeContexts.Find("SlaveChar");
			m_pActionController->SetSlaveController(*m_pSlaveActor->GetActionController(),
				targetScopeId, false, m_pSlaveADB);
		}

		m_pSlaveActor = nullptr;
	}
}

void CActor::SetWeaponTag(EWeaponType weaponType)
{
	TagGroupID groupId = m_pAnimationContext->controllerDef.m_tags.FindGroup("weaponType");
	TagID tagId = TAG_ID_INVALID;
	if (groupId != TAG_ID_INVALID)
	{
		switch (weaponType)
		{
		case EWeaponType::Magic:	tagId = m_pAnimationContext->controllerDef.m_tags.Find("magic"); break;
		case EWeaponType::Sword:	tagId = m_pAnimationContext->controllerDef.m_tags.Find("sword"); break;
		case EWeaponType::Knife:	tagId = m_pAnimationContext->controllerDef.m_tags.Find("knife"); break;
		}

		if (tagId != TAG_ID_INVALID)
			m_pAnimationContext->state.SetGroup(groupId, tagId);
		else
			CryLog("Weapon tag id could not found!");
	}
	else
		CryLog("weaponType tag group id could not found!");
}

void CActor::UpdateActorTurnAngle()
{
	float turnAngle = GetMoveAngle();

	auto *pCharacter = GetEntity()->GetCharacter(CActor::eGeometry_Character);

	pCharacter->GetISkeletonAnim()->SetDesiredMotionParam(eMotionParamID_TurnAngle, turnAngle, 0.f);
}

void CActor::ActivateMannequinContext(const char *contextName, ICharacterInstance &character, const SControllerDef &controllerDefinition, const IAnimationDatabase &animationDatabase)
{
	IEntity &entity = *GetEntity();

	const TagID scopeContextId = controllerDefinition.m_scopeContexts.Find(contextName);
	if (scopeContextId == TAG_ID_INVALID)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Failed to find %s scope context id in controller definition.", contextName);
		return;
	}

	// Setting Scope contexts can happen at any time, and what entity or character instance we have bound to a particular scope context
	// can change during the lifetime of an action controller.
	m_pActionController->SetScopeContext(scopeContextId, entity, &character, &animationDatabase);
}

void CActor::SetActorMannequin()
{
	// Release previous controller and context, if we are respawning
	SAFE_RELEASE(m_pActionController);
	SAFE_DELETE(m_pAnimationContext);

	// Now start loading the Mannequin data
	IMannequin &mannequinInterface = gEnv->pGameFramework->GetMannequinInterface();
	IAnimationDatabaseManager &animationDatabaseManager = mannequinInterface.GetAnimationDatabaseManager();

	const char *mannequinControllerDefinition = m_actorMannequinInfo.m_pActorControllerDefinition;

	// Load the Mannequin controller definition.
	// This is owned by the animation database manager
	const SControllerDef *pControllerDefinition = animationDatabaseManager.LoadControllerDef(mannequinControllerDefinition);
	if (pControllerDefinition == nullptr)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Failed to load controller definition.");
		return;
	}

	const char *mannequinContextName = m_actorMannequinInfo.m_pActorMannequinContext;
	const char *animationDatabasePath = m_actorMannequinInfo.m_pActorAnimationDatabase;
	const char *slaveAnimationDatabasePath = m_actorMannequinInfo.m_pSlaveAnimationDatabase;

	// Load the animation database
	auto *pAnimationDatabase = animationDatabaseManager.Load(animationDatabasePath);
	if (pAnimationDatabase == nullptr)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Failed to load animation database %s!", animationDatabasePath);
		return;
	}

	//Load the slave animation database
	m_pSlaveADB = animationDatabaseManager.Load(slaveAnimationDatabasePath);
	if (m_pSlaveADB == nullptr)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Failed to load slave animation database %s!", slaveAnimationDatabasePath);
		return;
	}

	// Create a new animation context for the controller definition we loaded above
	m_pAnimationContext = new SAnimationContext(*pControllerDefinition);

	// Now create the controller that will be handling animation playback
	m_pActionController = mannequinInterface.CreateActionController(GetEntity(), *m_pAnimationContext);
	CRY_ASSERT(m_pActionController != nullptr);

	ICharacterInstance *pCharacterInstance = GetEntity()->GetCharacter(CActor::eGeometry_Character);
	CRY_ASSERT(pCharacterInstance != nullptr);

	// Activate the Main context we'll be playing our animations in
	ActivateMannequinContext(mannequinContextName, *pCharacterInstance, *pControllerDefinition, *pAnimationDatabase);

	PlayFragment("MotionIdle", PP_Lowest, TAG_STATE_EMPTY);

	// Disable movement coming from the animation (root joint offset), we control this entirely via physics
	pCharacterInstance->GetISkeletonAnim()->SetAnimationDrivenMotion(1);
}

//------------------------------------------------------------------------
void CActor::SetTag(char * pTagName, bool enable)
{
	if (m_pAnimationContext)
	{
		TagID tagId = m_pAnimationContext->controllerDef.m_tags.Find(pTagName);

		if (tagId != TAG_ID_INVALID)
		{
			m_pAnimationContext->state.Set(tagId, enable);
		}
	}
}

void CActor::SetTagGroup(TagGroupID groupId, TagID tagId)
{
	if (m_pActionController)
	{
		SAnimationContext &animContext = m_pActionController->GetContext();

		animContext.state.SetGroup(groupId, tagId);
	}
}
void CActor::ClearTagGroup(TagGroupID groupId)
{
	if (m_pActionController)
	{
		SAnimationContext &animContext = m_pActionController->GetContext();

		animContext.state.ClearGroup(groupId);
	}
}

void CActor::PlayIdleAction(char * fragmentName, EActionPriority priority, TagState fragTags)
{
	PlayFragment(fragmentName, priority, fragTags, IAction::Interruptable);
}

//------------------------------------------------------------------------
void CActor::PlayMoveAction(char * fragmentName, bool isRotationForce, EActionPriority priority, TagState fragTags, uint32 flags)
{
	if (m_pActionController)
	{
		FragmentID fragID = m_pActionController->GetContext().controllerDef.m_fragmentIDs.Find(fragmentName);
		m_pLastAction = new CMoveAction(this, isRotationForce, priority, fragID, fragTags, flags);
		m_pActionController->Queue(*m_pLastAction);
	}
}
void CActor::PlayMovementAction(char * fragmentName, EActionPriority priority, TagState fragTags, uint32 flags)
{
	if (m_pActionController)
	{
		FragmentID fragID = m_pActionController->GetContext().controllerDef.m_fragmentIDs.Find(fragmentName);
		m_pLastAction = new CMovementAction(this, priority, fragID, fragTags, flags);
		m_pActionController->Queue(*m_pLastAction);
	}
}

//------------------------------------------------------------------------
void CActor::PlayFragment(FragmentID fragmentID, EActionPriority priority, TagState tagState, uint32 flags)
{
	if (m_pActionController)
	{
		m_pLastAction = new TPlayerAction(priority, fragmentID, tagState, flags);
		m_pActionController->Queue(*m_pLastAction);
	}
}
void CActor::PlayFragment(char * fragmentName, EActionPriority priority, TagState tagState, uint32 flags)
{
	if (m_pActionController)
	{
		FragmentID fragID = m_pActionController->GetContext().controllerDef.m_fragmentIDs.Find(fragmentName);
		m_pLastAction = new TPlayerAction(priority, fragID, tagState, flags);
		m_pActionController->Queue(*m_pLastAction);
	}
}

//------------------------------------------------------------------------
void CActor::ForceFinishLastAction()
{
	if (m_pLastAction)
		m_pLastAction->ForceFinish();
}
// ~Animations and Mannequin
// ------------------------------------------------------------------
