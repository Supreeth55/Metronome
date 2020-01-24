#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H
#include "math.h"
#define EPSILON 0.0001f
#define PI      3.1415926535897932384626433832795

struct Vector3
{
	float x, y, z;
};



/*
This function sets the coordinates of the 2D vector (pResult) to 0
*/
void Vector3Zero(Vector3* pResult);

/*
This function sets the coordinates of the 2D vector (pResult) to x y z
*/
void Vector3Set(Vector3* pResult, float x, float y, float z);

/*
In this function, pResult will be set to the opposite of pVec0
*/
void Vector3Neg(Vector3* pResult, Vector3* pVec0);

/*
In this function, pResult will be the sum of pVec0 and pVec1
*/
void Vector3Add(Vector3* pResult, Vector3* pVec0, Vector3* pVec1);

/*
In this function, pResult will be the difference between pVec0 *pVec1: pVec0 - pVec1
*/
void Vector3Sub(Vector3* pResult, Vector3* pVec0, Vector3* pVec1);

/*
In this function, pResult will be the unit vector of pVec0
*/
void Vector3Normalize(Vector3* pResult, Vector3* pVec0);

/*
In this function, pResult will be the vector pVec0 scaled by the value c
*/
void Vector3Scale(Vector3* pResult, Vector3* pVec0, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and added to pVec1
*/
void Vector3ScaleAdd(Vector3* pResult, Vector3* pVec0, Vector3* pVec1, float c);

/*
In this function, pResult will be the vector pVec0 scaled by c and pVec1 will be subtracted from it
*/
void Vector3ScaleSub(Vector3* pResult, Vector3* pVec0, Vector3* pVec1, float c);

/*
This function returns the length of the vector pVec0
*/
float Vector3Length(Vector3* pVec0);

/*
This function returns the square of pVec0's length. Avoid the square root
*/
float Vector3SquareLength(Vector3* pVec0);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The distance between these 2 2D points is returned
*/
float Vector3Distance(Vector3* pVec0, Vector3* pVec1);

/*
In this function, pVec0 and pVec1 are considered as 2D points.
The squared distance between these 2 2D points is returned. Avoid the square root
*/
float Vector3SquareDistance(Vector3* pVec0, Vector3* pVec1);

/*
This function returns the dot product between pVec0 and pVec1
*/
float Vector3DotProduct(Vector3* pVec0, Vector3* pVec1);
/*
This function returns the cross product between pVec0and pVec1
*/
void Vector3CrossProduct(Vector3* pResult , Vector3* pVec0, Vector3* pVec1);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Degrees
*/
void Vector3FromAngleDeg(Vector3* pResult, float angle);

/*
This function computes the coordinates of the vector represented by the angle "angle", which is in Radian
*/
void Vector3FromAngleRad(Vector3* pResult, float angle);

#endif
