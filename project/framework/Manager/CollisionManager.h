/**
 * @file CollisionManager.h
 * @author Vineet Dogra, Romil Tendulkar
 * @date 09/26/2019
 * @brief This class implements the various collision detection functions that are needed
		As well as the data structure needed for spatial partitioning
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once

#ifndef EPSILON
#define EPSILON 0.000001
#endif
#include "../Source/pch.h"
#include "../Source/Components/Body.h" //not in the engine, please include.
//#include "shape.h"//not in the engine, please include.
#include "../Source/Util/Vector3D.h"
#include "../Source/Util/SpatialGrid.h"


class Contact
{
public:
	/**
	This constructor initializes the body pointers for the contact object
	*/
	Contact();
	~Contact();
	Body* mpBodies[2];
};

/*expected shapes:
cuboids (cubes are cuboids with equal side lengths)
Spheres*/

class CollisionManager
{
public:

	/**
		This constructor initializes the Spatial Partitioning data structure
	*/
	CollisionManager();
	~CollisionManager();

	/**
	Deletes all the contact objects. Resetting the list of contacts
	*/
	void Reset();

	/**
		This function is used to perform the SAT(Seperating Axis Theorem) test for collision on objects
		This is the first Broad Phase Detection that is performed.

		\param pShape1 | pointer to the shape object present in the body component of the first object
		\param pos1x | The position on the x axis for the first object
		\param pos1y | The position on the y axis for the first object
		\param pos1z | The position on the z axis for the first object

		\param pShape2 | pointer to the shape object present in the body component of the Second object
		\param pos2x | The position on the x axis for the second object
		\param pos2y | The position on the y axis for the second object
		\param pos2z | The position on the z axis for the second object

		\return returns true for no collision and false when collision occurs
	*/
	bool CheckCollisionCuboidCuboid(ShapeX* pShape1, float pos1x, float pos1y,float pos1z, ShapeX* pShape2, float pos2x, float pos2y, float pos2z);

	/**
	TThis function performs the secondary broad phase collision test

	\param pShape1 | pointer to the shape object present in the body component of the first object
	\param pos1x | The position on the x axis for the first object
	\param pos1y | The position on the y axis for the first object
	\param pos1z | The position on the z axis for the first object

	\param pShape2 | pointer to the shape object present in the body component of the Second object
	\param pos2x | The position on the x axis for the second object
	\param pos2y | The position on the y axis for the second object
	\param pos2z | The position on the z axis for the second object

	\return returns true for collision and false for no collision
	*/
	bool CheckBroadPhase2(ShapeX* pShape1, float pos1x, float pos1y, float pos1z, ShapeX* pShape2, float pos2x, float pos2y, float pos2z);
	/*bool CheckCollisionSphereCuboid(Shape* pShape1, float pos1x, float pos1y, Shape* pShape2, float pos2x, float pos2y, std::list<Contact*>& mContacts);
	bool CheckCollisionCuboidSphere(Shape* pShape1, float pos1x, float pos1y, Shape* pShape2, float pos2x, float pos2y, std::list<Contact*>& mContacts);
	bool CheckCollisionSphereSphere(Shape* pShape1, float pos1x, float pos1y, Shape* pShape2, float pos2x, float pos2y, std::list<Contact*>& mContacts);*/
	std::list<Contact*> mContacts;
	
	/**
	This function creates and returns an instance of Collision Manager.
	If an instance already exists it is returned instead.

	\return returns a static pointer of Collision Manager
	*/
	static CollisionManager* GetInstance();

	/**
		This function resets the spatial partitioning data structure
	*/
	void ResetGrid();

	SpatialGrid* mSpatial;
private:
	static CollisionManager* mInstance;
	
};