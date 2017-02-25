#include "StdAfx.h"
#include "PlayerAnimations.h"

#include "Player/Player.h"
#include "Player/Input/PlayerInput.h"

#include "Actions/MoveAction.h"
#include "Actions/MovementAction.h"

#include "Math/AFC_Math.h"

#include <CryAnimation/ICryAnimation.h>
#include <ICryMannequin.h>

CPlayerAnimations::CPlayerAnimations()
	: m_pActionController(nullptr)
	, m_pAnimationContext(nullptr)
{
}

CPlayerAnimations::~CPlayerAnimations()
{
	SAFE_RELEASE(m_pActionController);
	SAFE_DELETE(m_pAnimationContext);
}

void CPlayerAnimations::PostInit(IGameObject *pGameObject)
{
	m_pPlayer = static_cast<CPlayer *>(pGameObject->QueryExtension("Player"));

	// Make sure that this extension is updated regularly via the Update function below
	pGameObject->EnableUpdateSlot(this, 0);
}

void CPlayerAnimations::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	UpdatePlayerTurnAngle();

	if (m_pActionController != nullptr)
	{
		m_pActionController->Update(ctx.fFrameTime);
	}
}

void CPlayerAnimations::UpdatePlayerTurnAngle()
{
	Vec3 moveDir = m_pPlayer->GetInput()->GetMoveDirection();

	float turnAngle = GetVectorsAngleN(moveDir, GetEntity()->GetForwardDir());

	auto *pCharacter = m_pPlayer->GetEntity()->GetCharacter(CPlayer::eGeometry_ThirdPerson);

	pCharacter->GetISkeletonAnim()->SetDesiredMotionParam(eMotionParamID_TurnAngle, turnAngle, 0.f);
}

void CPlayerAnimations::SetWeaponTag(EWeaponType weaponType)
{
	TagGroupID groupId = m_pAnimationContext->controllerDef.m_tags.FindGroup("weaponType");
	TagID tagId = TAG_ID_INVALID;
	if (groupId != TAG_ID_INVALID)
	{
		switch (weaponType)
		{
		case ewt_magic:	tagId = m_pAnimationContext->controllerDef.m_tags.Find("magic"); break;
		case ewt_sword:	tagId = m_pAnimationContext->controllerDef.m_tags.Find("sword"); break;
		case ewt_knife:	tagId = m_pAnimationContext->controllerDef.m_tags.Find("knife"); break;
		}

		if (tagId != TAG_ID_INVALID)
			m_pAnimationContext->state.SetGroup(groupId, tagId);
		else
			CryLog("Weapon tag id could not found!");
	}
	else
		CryLog("weaponType tag group id could not found!");
}

void CPlayerAnimations::ProcessEvent(SEntityEvent& event)
{
	switch (event.event)
	{
		case ENTITY_EVENT_ANIM_EVENT:
		{
			const AnimEventInstance *pAnimEvent = reinterpret_cast<const AnimEventInstance *>(event.nParam[0]);
			ICharacterInstance *pCharacter = reinterpret_cast<ICharacterInstance *>(event.nParam[1]);

			m_pActionController->OnAnimationEvent(pCharacter, *pAnimEvent);
		}
		break;
	}
}

void CPlayerAnimations::OnPlayerModelChanged()
{
	// Release previous controller and context, if we are respawning
	SAFE_RELEASE(m_pActionController);
	SAFE_DELETE(m_pAnimationContext);

	// Now start loading the Mannequin data
	IMannequin &mannequinInterface = gEnv->pGameFramework->GetMannequinInterface();
	IAnimationDatabaseManager &animationDatabaseManager = mannequinInterface.GetAnimationDatabaseManager();

	const char *mannequinControllerDefinition = m_pPlayer->GetCVars().m_pThirdPersonControllerDefinition->GetString();

	// Load the Mannequin controller definition.
	// This is owned by the animation database manager
	const SControllerDef *pControllerDefinition = animationDatabaseManager.LoadControllerDef(mannequinControllerDefinition);
	if (pControllerDefinition == nullptr)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Failed to load controller definition.");
		return;
	}

	const char *mannequinContextName = m_pPlayer->GetCVars().m_pThirdPersonMannequinContext->GetString();
	const char *animationDatabasePath = m_pPlayer->GetCVars().m_pThirdPersonAnimationDatabase->GetString();

	// Load the animation database
	auto *pAnimationDatabase = animationDatabaseManager.Load(animationDatabasePath);
	if (pAnimationDatabase == nullptr)
	{
		CryWarning(VALIDATOR_MODULE_GAME, VALIDATOR_ERROR, "Failed to load animation database %s!", animationDatabasePath);
		return;
	}

	// Create a new animation context for the controller definition we loaded above
	m_pAnimationContext = new SAnimationContext(*pControllerDefinition);

	// Now create the controller that will be handling animation playback
	m_pActionController = mannequinInterface.CreateActionController(GetEntity(), *m_pAnimationContext);
	CRY_ASSERT(m_pActionController != nullptr);

	ICharacterInstance *pCharacterInstance = GetEntity()->GetCharacter(CPlayer::eGeometry_ThirdPerson);
	CRY_ASSERT(pCharacterInstance != nullptr);

	// Activate the Main context we'll be playing our animations in
	ActivateMannequinContext(mannequinContextName, *pCharacterInstance, *pControllerDefinition, *pAnimationDatabase);

	PlayFragment("MotionIdle", PP_Lowest, TAG_STATE_EMPTY);

	// Disable movement coming from the animation (root joint offset), we control this entirely via physics
	pCharacterInstance->GetISkeletonAnim()->SetAnimationDrivenMotion(1);

	SetWeaponTag(ewt_magic);
}

void CPlayerAnimations::ActivateMannequinContext(const char *contextName, ICharacterInstance &character, const SControllerDef &controllerDefinition, const IAnimationDatabase &animationDatabase)
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

//------------------------------------------------------------------------
void CPlayerAnimations::SetTagGroup(TagGroupID groupId, TagID tagId)
{
	if (m_pActionController)
	{
		SAnimationContext &animContext = m_pActionController->GetContext();

		animContext.state.SetGroup(groupId, tagId);
	}
}
void CPlayerAnimations::ClearTagGroup(TagGroupID groupId)
{
	if (m_pActionController)
	{
		SAnimationContext &animContext = m_pActionController->GetContext();

		animContext.state.ClearGroup(groupId);
	}
}

//------------------------------------------------------------------------
void CPlayerAnimations::PlayMoveAction(char * fragmentName, bool isRotationForce, EPlayerActionPriority priority, TagState fragTags)
{
	if (m_pActionController)
	{
		FragmentID fragID = m_pActionController->GetContext().controllerDef.m_fragmentIDs.Find(fragmentName);
		m_pLastAction = new CMoveAction(m_pPlayer, isRotationForce, priority, fragID, fragTags);
		m_pActionController->Queue(*m_pLastAction);
	}
}

void CPlayerAnimations::PlayMovementAction(char * fragmentName, EPlayerActionPriority priority, TagState fragTags)
{
	if (m_pActionController)
	{
		FragmentID fragID = m_pActionController->GetContext().controllerDef.m_fragmentIDs.Find(fragmentName);
		m_pLastAction = new CMovementAction(m_pPlayer, priority, fragID, fragTags);
		m_pActionController->Queue(*m_pLastAction);
	}
}

//------------------------------------------------------------------------
void CPlayerAnimations::PlayFragment(FragmentID fragmentID, EPlayerActionPriority priority, TagState tagState)
{
	if (m_pActionController)
	{
		m_pLastAction = new TPlayerAction(priority, fragmentID, tagState);
		m_pActionController->Queue(*m_pLastAction);
	}
}
void CPlayerAnimations::PlayFragment(char * fragmentName, EPlayerActionPriority priority, TagState tagState)
{
	if (m_pActionController)
	{
		FragmentID fragID = m_pActionController->GetContext().controllerDef.m_fragmentIDs.Find(fragmentName);
		m_pLastAction = new TPlayerAction(priority, fragID, tagState);
		m_pActionController->Queue(*m_pLastAction);
	}
}

//------------------------------------------------------------------------
void CPlayerAnimations::ForceFinishLastAction()
{
	if (m_pLastAction)
		m_pLastAction->ForceFinish();
}
