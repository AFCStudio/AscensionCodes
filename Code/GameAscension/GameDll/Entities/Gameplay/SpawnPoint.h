#ifndef __SPAWN_POINT_H__
#define __SPAWN_POINT_H__

#pragma once

#include "Entities/Helpers/ISimpleExtension.h"

////////////////////////////////////////////////////////
// Entity responsible for spawning other entities
////////////////////////////////////////////////////////
class CSpawnPoint : public ISimpleExtension
{
public:
	virtual ~CSpawnPoint() {}

	void SpawnEntity(IEntity &otherEntity);
};

#endif