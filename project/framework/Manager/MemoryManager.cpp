/**
 * @file MemoryManager.cpp
 * @authors Vineet Amarjeet Dogra
 * @date 09/27/19
 * @brief This file governs how the Memory Manager is supposed to work.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "MemoryManager.h"
#include "TimerManager.h"
MemoryManager* MemoryManager::GetInstance()
{
	if (mInstance == nullptr)
	{
		mInstance = new MemoryManager();
	}
	return mInstance;
}


void MemoryManager::Destroy()
{
	Deallocate();
	delete mInstance;
}

void MemoryManager::Initialize()
{
	unsigned int i;
	for (i = 0; i < max_object_number; i++)
	{
		
		ObjectPool[i];//initializing the map.
	}
	std::cout << i << std::endl;
}

unsigned int MemoryManager::Allocate()
{
	if (current_position < max_object_number)
		return current_position;
	else
		return -1;
}

void MemoryManager::Deallocate()
{
	//check for memory leak in case of not freeing the allocated elements in the data structure
	delete ObjectPool;
}

void MemoryManager::Free()
{
	if (current_position > 0)
		current_position--;
}

void MemoryManager::FreeAt(int key)
{
	(*ObjectPool)[key] = (*ObjectPool)[current_position - 1];
	
	current_position--;
}
//DEPRECATED : SHIFTED TO GAMEOBJECTMANAGER WHERE IT SHOULD BE
//void MemoryManager::Update() 
//{
//	unsigned int key = current_position;
//	for (unsigned int i = 0; i < key; ++i)
//	{
//		if ((*ObjectPool)[i].pTr)
//		{
//			(*ObjectPool)[i].pTr->Update(TimerManager::GetInstance()->getDeltaTime());
//		}
//		if ((*ObjectPool)[i].pCr)
//		{
//			(*ObjectPool)[i].pCr->Update(TimerManager::GetInstance()->getDeltaTime());
//		}
//		if ((*ObjectPool)[i].pAn)
//		{
//			(*ObjectPool)[i].pAn->Update(TimerManager::GetInstance()->getDeltaTime());
//		}
//	}
//}