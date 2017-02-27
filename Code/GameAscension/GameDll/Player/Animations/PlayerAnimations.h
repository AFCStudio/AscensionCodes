#ifndef __PLAYER_ANIMATIONS_H__
#define __PLAYER_ANIMATIONS_H__

#pragma once

#include "Entities/Helpers/ISimpleExtension.h"
#include "Actions/ActionBase.h"

#include <ICryMannequin.h>

enum EPlayerActionPriority;
enum EWeaponType;
class CPlayer;

////////////////////////////////////////////////////////
// Player extension to manage animation handling and playback via Mannequin
////////////////////////////////////////////////////////
class CPlayerAnimations
	: public CGameObjectExtensionHelper<CPlayerAnimations, ISimpleExtension>
{
public:
	CPlayerAnimations();
	virtual ~CPlayerAnimations();

	// ISimpleExtension
	virtual void PostInit(IGameObject* pGameObject) override;

	virtual void Update(SEntityUpdateContext& ctx, int updateSlot) override;

	virtual void ProcessEvent(SEntityEvent& event) override;
	// ~ISimpleExtension

	void OnPlayerModelChanged();

	void SetTagGroup(TagGroupID groupId, TagID tagId);
	void ClearTagGroup(TagGroupID groupId);

	void PlayMoveAction(char * fragmentName, bool isRotationForce, EPlayerActionPriority priority, TagState fragTags = TAG_STATE_EMPTY);
	void PlayMovementAction(char * fragmentName, EPlayerActionPriority priority, TagState fragTags = TAG_STATE_EMPTY);

	void PlayFragment(FragmentID fragmentID, EPlayerActionPriority priority, TagState tagState = TAG_STATE_EMPTY);
	void PlayFragment(char * fragmentName, EPlayerActionPriority priority, TagState tagState = TAG_STATE_EMPTY);

	void PlaySwordAction(EPlayerActionPriority priority = PP_Sword);

	void ForceFinishLastAction();

	void UpdatePlayerTurnAngle();

	void SetWeaponTag(EWeaponType weaponType);

protected:
	void ActivateMannequinContext(const char *contextName, ICharacterInstance &character, const SControllerDef &controllerDefinition, const IAnimationDatabase &animationDatabase);

protected:
	CPlayer * m_pPlayer;

	IActionController * m_pActionController;
	SAnimationContext * m_pAnimationContext;

	IActionPtr	m_pLastAction;
};

#endif