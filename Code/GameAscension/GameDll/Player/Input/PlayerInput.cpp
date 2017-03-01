#include "StdAfx.h"
#include "PlayerInput.h"

#include "Player/Player.h"
#include "Player/States/PlayerStateManager.h"

#include <CryAnimation/ICryAnimation.h>

CPlayerInput::CPlayerInput()
	: m_moveAngle(0.0f)
	, m_mouseDeltaRotation(ZERO)
	, m_moveDirection(ZERO)
	, m_localMoveDirection(ZERO)
	, m_lookOrientation(ZERO)
	, m_isMoveBlocked(false)
{}

void CPlayerInput::PostInit(IGameObject *pGameObject)
{
	m_pPlayer = static_cast<CPlayer *>(pGameObject->QueryExtension("Player"));

	// NOTE: Since CRYENGINE 5.3, the game is responsible to initialize the action maps
	IActionMapManager *pActionMapManager = gEnv->pGameFramework->GetIActionMapManager();
	pActionMapManager->InitActionMaps("Libs/config/defaultprofile.xml");
	pActionMapManager->Enable(true);
	pActionMapManager->EnableActionMap("player", true);

	if (IActionMap *pActionMap = pActionMapManager->GetActionMap("player"))
	{
		pActionMap->SetActionListener(GetEntityId());
	}

	GetGameObject()->CaptureActions(this);

	// Make sure that this extension is updated regularly via the Update function below
	GetGameObject()->EnableUpdateSlot(this, 0);

	// Populate the action handler callbacks so that we get action map events
	InitializeActionHandler();
}

void CPlayerInput::Update(SEntityUpdateContext &ctx, int updateSlot)
{
	CalculateLookOrientation(ctx.fFrameTime);
	CalculateLocalMoveDirection();
	CalculateMoveDirection();
	CalculateMoveAngle();
	UpdatePlayerState();

	// Reset every frame
	m_mouseDeltaRotation = ZERO;
}

void CPlayerInput::OnPlayerRespawn()
{
	m_inputFlags = 0;
	m_mouseDeltaRotation = ZERO;
	m_lookOrientation = IDENTITY;
}

void CPlayerInput::HandleInputFlagChange(EInputFlags flags, int activationMode, EInputFlagType type)
{
	switch (type)
	{
		case eInputFlagType_Hold:
		{
			if (activationMode == eIS_Released)
			{
				m_inputFlags &= ~flags;
			}
			else
			{
				m_inputFlags |= flags;
			}
		}
		break;
		case eInputFlagType_Toggle:
		{
			if (activationMode == eIS_Released)
			{
				// Toggle the bit(s)
				m_inputFlags ^= flags;
			}
		}
		break;
	}
}

void CPlayerInput::CalculateLookOrientation(float frameTime)
{
	// Start by updating look dir
	Ang3 ypr = CCamera::CreateAnglesYPR(Matrix33(m_lookOrientation));

	CPlayer::SPlayerViewParams playerViewParams = m_pPlayer->GetPlayerViewParams();

	ypr.x += m_mouseDeltaRotation.x * playerViewParams.m_rotationSpeedYaw * frameTime;

	// TODO: Perform soft clamp here instead of hard wall, should reduce rot speed in this direction when close to limit.
	ypr.y = CLAMP(ypr.y + m_mouseDeltaRotation.y * playerViewParams.m_rotationSpeedPitch * frameTime,
					playerViewParams.m_rotationLimitsMinPitch, playerViewParams.m_rotationLimitsMaxPitch);

	ypr.z = 0;

	m_lookOrientation = Quat(CCamera::CreateOrientationYPR(ypr));
}
void CPlayerInput::CalculateMoveDirection()
{
	m_moveDirection = m_lookOrientation * m_localMoveDirection;
	m_moveDirection.z = 0.0f;
	m_moveDirection.Normalize();
}
void CPlayerInput::CalculateLocalMoveDirection()
{
	m_localMoveDirection = ZERO;

	if (m_inputFlags & CPlayerInput::eInputFlag_MoveLeft)
	{
		m_localMoveDirection.x -= 1;
	}
	if (m_inputFlags & CPlayerInput::eInputFlag_MoveRight)
	{
		m_localMoveDirection.x += 1;
	}
	if (m_inputFlags & CPlayerInput::eInputFlag_MoveForward)
	{
		m_localMoveDirection.y += 1;
	}
	if (m_inputFlags & CPlayerInput::eInputFlag_MoveBack)
	{
		m_localMoveDirection.y -= 1;
	}

	m_localMoveDirection.Normalize();
}
void CPlayerInput::CalculateMoveAngle()
{
	if (m_localMoveDirection != ZERO)
	{
		Vec3 playerDir = GetEntity()->GetForwardDir();

		m_moveAngle = acosf(playerDir.dot(m_moveDirection));

		m_moveAngle *= playerDir.cross(m_moveDirection).z >= 0 ? 1 : -1;
	}
	else
	{
		Vec3 cameraDir = m_lookOrientation.GetColumn1();
		Vec3 playerDir = GetEntity()->GetForwardDir();

		m_moveAngle = acosf(playerDir.dot(cameraDir));

		m_moveAngle *= playerDir.cross(cameraDir).z >= 0 ? 1 : -1;
	}
}

void CPlayerInput::UpdatePlayerState()
{
	if (!m_isMoveBlocked && m_localMoveDirection != ZERO)
	{
		m_pPlayer->GetStateManager()->ChangeState(epsMove);
	}
	else
	{
		m_pPlayer->GetStateManager()->ChangeState(epsIdle);
	}
}

void CPlayerInput::InitializeActionHandler()
{
	m_actionHandler.AddHandler(ActionId("moveleft"), &CPlayerInput::OnActionMoveLeft);
	m_actionHandler.AddHandler(ActionId("moveright"), &CPlayerInput::OnActionMoveRight);
	m_actionHandler.AddHandler(ActionId("moveforward"), &CPlayerInput::OnActionMoveForward);
	m_actionHandler.AddHandler(ActionId("moveback"), &CPlayerInput::OnActionMoveBack);

	m_actionHandler.AddHandler(ActionId("mouse_rotateyaw"), &CPlayerInput::OnActionMouseRotateYaw);
	m_actionHandler.AddHandler(ActionId("mouse_rotatepitch"), &CPlayerInput::OnActionMouseRotatePitch);

	m_actionHandler.AddHandler(ActionId("space"), &CPlayerInput::OnActionSpace);

	m_actionHandler.AddHandler(ActionId("mouse_left"), &CPlayerInput::OnActionMouseLeft);

	m_actionHandler.AddHandler(ActionId("select_sword"), &CPlayerInput::OnActionSelectSword);
}

void CPlayerInput::OnAction(const ActionId &action, int activationMode, float value)
{
	// This function is called when inputs trigger action map events
	// The handler below maps the event (see 'action') to a callback, further below in this file.
	m_actionHandler.Dispatch(this, GetEntityId(), action, activationMode, value);
}

bool CPlayerInput::OnActionMoveLeft(EntityId entityId, const ActionId& actionId, int activationMode, float value)
{
	HandleInputFlagChange(eInputFlag_MoveLeft, activationMode);
	return true;
}

bool CPlayerInput::OnActionMoveRight(EntityId entityId, const ActionId& actionId, int activationMode, float value)
{
	HandleInputFlagChange(eInputFlag_MoveRight, activationMode);
	return true;
}

bool CPlayerInput::OnActionMoveForward(EntityId entityId, const ActionId& actionId, int activationMode, float value)
{
	HandleInputFlagChange(eInputFlag_MoveForward, activationMode);
	return true;
}

bool CPlayerInput::OnActionMoveBack(EntityId entityId, const ActionId& actionId, int activationMode, float value)
{
	HandleInputFlagChange(eInputFlag_MoveBack, activationMode);
	return true;
}

bool CPlayerInput::OnActionMouseRotateYaw(EntityId entityId, const ActionId& actionId, int activationMode, float value)
{
	m_mouseDeltaRotation.x -= value;
	return true;
}

bool CPlayerInput::OnActionMouseRotatePitch(EntityId entityId, const ActionId& actionId, int activationMode, float value)
{
	m_mouseDeltaRotation.y -= value;
	return true;
}

bool CPlayerInput::OnActionSpace(EntityId entityId, const ActionId & actionId, int activationMode, float value)
{
	m_pPlayer->SetSpaceKeyStatus(activationMode != eIS_Released);
	return true;
}

bool CPlayerInput::OnActionMouseLeft(EntityId entityId, const ActionId& actionId, int activationMode, float value)
{
	m_pPlayer->SwordAttack();
	return true;
}

bool CPlayerInput::OnActionSelectSword(EntityId entityId, const ActionId & actionId, int activationMode, float value)
{
	if (m_pPlayer->GetWeaponType() == ewt_sword)
		m_pPlayer->SelectWeapon(ewt_magic);
	else
		m_pPlayer->SelectWeapon(ewt_sword);

	return true;
}
