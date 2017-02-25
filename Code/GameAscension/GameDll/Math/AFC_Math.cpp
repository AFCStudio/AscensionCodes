/* -------------------------------------------------------------
-	AFC_Math.cpp - Source file
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

#include "AFC_Math.h"

float GetVectorsAngle(Vec3 v1, Vec3 v2)
{
	v1.Normalize();
	v2.Normalize();

	float angle = acosf(v1.dot(v2));

	angle *= v1.cross(v2).z >= 0 ? 1 : -1;

	return angle;
}
float GetVectorsAngleN(Vec3 v1, Vec3 v2)
{
	float angle = acosf(v1.dot(v2));

	angle *= v1.cross(v2).z >= 0 ? 1 : -1;

	return angle;
}
