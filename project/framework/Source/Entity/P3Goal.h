/**
 * @file P3Goal.h
 * @authors  Romil Tendulkar
 * @date 11/29/2019
 * @brief Used for the Goal in the third prototype

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once

#include "Entity.h"
#include "..//..//Manager/MemoryManager.h"

class Body;
class Transform;

class P3Goal : public Entity
{
public:
	P3Goal();
	~P3Goal();

	/**
		This function serializes the entity

		\param root | the root for reading from the json
		\param position | the key value of this object in the memory manager
	*/
	void Serialize(Json::Value root, unsigned int position);

	/**
		Functionality specific to the entity goes here

		\param deltaTime | the time passed between frames
	*/
	void Update(float deltaTime);
public:
	Transform* pTr;
	Body* pBd;
	unsigned int key;
};