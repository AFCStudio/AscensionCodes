#ifndef __PLAYER_ANIMATIONS_H__
#define __PLAYER_ANIMATIONS_H__

#pragma once

#include "Entities/Helpers/ISimpleExtension.h"

#include <ICryMannequin.h>

enum EPlayerActionPriority;
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

	void PlayMoveAction(FragmentID fragmentID, EPlayerActionPriority priority, TagState fragTags);
	void PlayMoveForceAction(FragmentID fragmentID, EPlayerActionPriority priority, TagState fragTags);

	void PlayFragment(FragmentID fragmentID, EPlayerActionPriority priority, TagState tagState);
	void PlayFragment(char * fragmentName, EPlayerActionPriority priority, TagState tagState);

	void ForceFinishLastAction();

protected:
	void ActivateMannequinContext(const char *contextName, ICharacterInstance &character, const SControllerDef &controllerDefinition, const IAnimationDatabase &animationDatabase);

protected:
	CPlayer * m_pPlayer;

	IActionController * m_pActionController;
	SAnimationContext * m_pAnimationContext;

	_smart_ptr<IAction> m_pIdleFragment;

	TagID m_rotateTagId;
	TagID m_walkTagId;

	IActionPtr	m_pLastAction;
};

#endif