/**
 * @file ShapeX.h
 * @author Romil Tendulkar
 * @date 10/22/2019
 * @brief This file implements the shape classes that are used for collision detection
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "ShapeX.h"

ShapeX::ShapeX()
{
}

/**
	Initializes the type and the owner body of the shape

	\param pOwner | Pointer to the body that the shape is initialized for
	\param ptype | enum for the type of shape
*/
ShapeX::ShapeX(Body* pOwner,SHAPE_TYPE ptype)
{
	type = ptype;
	pOwnerBody = pOwner;
}

/**
	initializes a AABB shape object

	\param pOwner | Pointer to the body that the shape is initialized for
*/
ShapeAABB::ShapeAABB(Body* pOwner) : ShapeX(pOwner,SHAPE_AABB)
{
	mTop = mBottom = mLeft = mRight = mOut = mIn = 0;
}

ShapeAABB::ShapeAABB()
{
}

ShapeAABB::~ShapeAABB()
{

}