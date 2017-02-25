#include "StdAfx.h"
#include "PlayerView.h"

#include "Player/Player.h"
#include "Player/Input/PlayerInput.h"

#include "Game/GameCVars.h"

#include <IViewSystem.h>
#include <CryAnimation/ICryAnimation.h>

#include <CryGame\GameUtils.h>		// For Interpolate
#include <CryAISystem\IAISystem.h>	// For PhysSkipList

CPlayerView::CPlayerView()
{
}

CPlayerView::~CPlayerView()
{
	GetGameObject()->ReleaseView(this);
}

void CPlayerView::PostInit(IGameObject *pGameObject)
{
	m_pPlayer = static_cast<CPlayer *>(pGameObject->QueryExtension("Player"));

	// Register for UpdateView callbacks
	GetGameObject()->CaptureView(this);
}

void CPlayerView::UpdateView(SViewParams &viewParams)
{
	IEntity &entity = *GetEntity();

	viewParams.nearplane = g_pGameCVars->cam_nearPlane;

	// Start with changing view rotation to the requested mouse look orientation
	viewParams.rotation = Quat(m_pPlayer->GetInput()->GetLookOrientation());

	// Start with matching view to the entity position
	viewParams.position = entity.GetWorldPos();

	// Offset the player along the right axis (normally back)
	viewParams.position += m_pPlayer->GetInput()->GetLookOrientation().GetColumn0() * g_pGameCVars->cam_viewOffsetX;

	// Offset the view upwards
	viewParams.position.z += g_pGameCVars->cam_viewOffsetZ;

	SetCameraPosWithCollision(viewParams);
}

void CPlayerView::SetCameraPosWithCollision(SViewParams& viewParams)
{
	Vec3 targetCameraPos = viewParams.position + m_pPlayer->GetInput()->GetLookOrientation().GetColumn1() * g_pGameCVars->cam_viewOffsetY;
	Vec3 playerBodyPos = m_pPlayer->GetEntity()->GetPos();
	playerBodyPos.z += g_pGameCVars->cam_viewOffsetZ;

	Vec3 player2CameraVector = (targetCameraPos - playerBodyPos).normalized();

	PhysSkipList skipList;

	IPhysicalEntity* pSkipEntities[10];
	int nSkip = m_pPlayer->GetPhysicalSkipEntities(pSkipEntities, 10);
	for (int i = 0; i < nSkip; ++i)
	{
		stl::push_back_unique(skipList, pSkipEntities[i]);
	}

	primitives::sphere sphere;
	sphere.center = playerBodyPos;
	sphere.r = g_pGameCVars->cam_colliderRadius;
	geom_contact *pContact = 0;

	Vec3 colliderVector = player2CameraVector +
		player2CameraVector.normalized() * g_pGameCVars->cam_colliderExtraLength;

	float collideDistance = gEnv->pPhysicalWorld->PrimitiveWorldIntersection(
		sphere.type, &sphere, colliderVector,
		ent_living | ent_static | ent_rigid | ent_sleeping_rigid | ent_terrain,
		&pContact, 0, geom_colltype_player, 0, 0, 0, pSkipEntities, nSkip);

	if (collideDistance > 0 && pContact)
	{
		CryLog("Collide distance is %f", collideDistance);

		targetCameraPos = viewParams.position +
			m_pPlayer->GetInput()->GetLookOrientation().GetColumn1() * -collideDistance;
	}

	// Collider Enter Interpolation
	Vec3 lastPos = viewParams.GetPositionLast();
	Interpolate(lastPos, targetCameraPos, g_pGameCVars->cam_colliderTransitionSpeed, viewParams.frameTime);

	// Offset the player along the forward axis (normally back)
	viewParams.position = lastPos;
}