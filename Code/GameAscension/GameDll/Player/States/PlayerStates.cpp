// -------------------------------------------------------------
//	PlayerStates.cpp - Source file
// -------------------------------------------------------------
//	Description:
//
//		Defines player states for game.
//
// -------------------------------------------------------------
//	Creation Date: 21.02.2017
//										Writed by Özkan Afacan
//		Copyright (C), AFC Studios
// -------------------------------------------------------------

#include "StdAfx.h"
#include "PlayerStates.h"

#include "Player/Player.h"
#include "Player/Animations/PlayerAnimations.h"
#include "Player/Movement/PlayerMovement.h"

#include "Actions/ActionBase.h"

// Idle State
void CPlayerStateIdle::Enter()
{
	m_pPlayer->GetAnimationManager()->ForceFinishLastAction();
	m_pPlayer->GetAnimationManager()->PlayFragment("MotionIdle", PP_Lowest, TAG_STATE_EMPTY);
}
void CPlayerStateIdle::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	IEntity &entity = *(m_pPlayer->GetEntity());
	IPhysicalEntity *pPhysicalEntity = entity.GetPhysics();
	if (pPhysicalEntity == nullptr)
		return;

	// Obtain stats from the living entity implementation
	m_pPlayer->GetMovement()->GetLatestPhysicsStats(*pPhysicalEntity);

	// Send latest input data to physics indicating desired movement direction
	m_pPlayer->GetMovement()->UpdateMovementRequest(ctx.fFrameTime, *pPhysicalEntity);
}
void CPlayerStateIdle::Exit()
{}
// -----------------------------------------------------

// Move State
void CPlayerStateMove::Enter()
{
	m_pPlayer->GetAnimationManager()->PlayFragment("MotionMovement", PP_Movement, TAG_STATE_EMPTY);
}
void CPlayerStateMove::Update(SEntityUpdateContext& ctx, int updateSlot)
{
	IEntity &entity = *(m_pPlayer->GetEntity());
	IPhysicalEntity *pPhysicalEntity = entity.GetPhysics();
	if (pPhysicalEntity == nullptr)
		return;

	// Obtain stats from the living entity implementation
	m_pPlayer->GetMovement()->GetLatestPhysicsStats(*pPhysicalEntity);

	// Send latest input data to physics indicating desired movement direction
	m_pPlayer->GetMovement()->UpdateMovementRequest(ctx.fFrameTime, *pPhysicalEntity);
}
void CPlayerStateMove::Exit()
{
}
// -----------------------------------------------------

// Alnitak State
void CPlayerStateAlnitak::Enter()
{}
void CPlayerStateAlnitak::Update(SEntityUpdateContext& ctx, int updateSlot)
{}
void CPlayerStateAlnitak::Exit()
{}
// -----------------------------------------------------

// Capture State
void CPlayerStateCapture::Enter()
{}
void CPlayerStateCapture::Update(SEntityUpdateContext& ctx, int updateSlot)
{}
void CPlayerStateCapture::Exit()
{}
// -----------------------------------------------------

// Fly State
void CPlayerStateFly::Enter()
{}
void CPlayerStateFly::Update(SEntityUpdateContext& ctx, int updateSlot)
{}
void CPlayerStateFly::Exit()
{}
// -----------------------------------------------------

// Swim State
void CPlayerStateSwim::Enter()
{}
void CPlayerStateSwim::Update(SEntityUpdateContext& ctx, int updateSlot)
{}
void CPlayerStateSwim::Exit()
{}
// -----------------------------------------------------