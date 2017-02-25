/* -------------------------------------------------------------
-	AFC_Math.h - Header file
-  -------------------------------------------------------------
-	Description:
-
-		That class stores basic math function.
-
-  -------------------------------------------------------------
-	Creation Date: 25.02.2017
-										Writed by Özkan Afacan
-		Copyright (C), AFC Studios
---------------------------------------------------------------- */

#ifndef __AFC_MATH_H__
#define __AFC_MATH_H__

#pragma once

#include "IGameObject.h"

// Find angle between vectors.
// Input vectors are going to normalize.
float GetVectorsAngle(Vec3 v1, Vec3 v2);

// Find angle between vectors
// Input vectprs have to be normalized.
float GetVectorsAngleN(Vec3 v1, Vec3 v2);

#endif