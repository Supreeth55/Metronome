/**
 * @file MemoryManager.h
 * @authors Vineet Amarjeet Dogra
 * @date 09/27/19
 * @brief This file governs how the Memory Manager is supposed to work.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
#include "../Source/pch.h"
#include "..//Source/Components/Transform.h"
#include "..//Source/Components/Asset.h"
#include "..//Source/Components/Controller.h"
#include "..//Source/Components/Animation.h"
#include "..//Source/Components/Body.h"
#include "..//Source/Components/Component.h"
#include "..//Source/Components/LightDetail.h"
#include "../Source/Entity/Entity.h"
#include "..\Source\GameStates\PlayState.h"

class MemoryManager
{
public:
	//changed concrete objects to pointers because of copy by value issue
	struct ObjectNode//cell for the object pool.
	{
		Transform* pTr=nullptr;
		Animation* pAn = nullptr;
		Asset* pAs = nullptr;
		Controller* pCr = nullptr;
		Body* pBd = nullptr;
		LightDetail* pLd = nullptr;

		Entity* pEn = nullptr;
		//bool assigned = false;//to check if the cell from the pool is assigned.
	};
	static MemoryManager* GetInstance();

	/**
   This functions deallocates the memory assigned by the memory manager via Deallocate().
	*/
	void Destroy();

	/**
	This functions initializes the nodes of the unordered map.
	*/
	void Initialize();


	/**
	   This functions returns the current position where memory can be allocated.
	*/
	unsigned int Allocate();

	/**
	   This functions deallocates the memory assigned by the memory manager.
	*/
	void Deallocate();

	/**
	   This functions frees the last nodes.
	*/
	void Free();

	/**
	This functions frees the node at the given key position.
	\param key, the node to free at.
	*/
	void FreeAt(int key);
	//void Update();
	std::unordered_map<unsigned, ObjectNode>* ObjectPool = new std::unordered_map<unsigned int, ObjectNode>;
	unsigned int current_position = 0;
	unsigned int max_object_number = 100000;
	PROT_TYPE mProtType;
private:
	static MemoryManager* mInstance;
};
