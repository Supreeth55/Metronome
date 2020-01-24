#include "../pch.h"
#include "P1Collectible.h"
#include "..\..\Manager\CollisionManager.h"
#include "..\..\Manager\MemoryManager.h"
#include "P1Goal.h"

P1Collectible::P1Collectible() : Entity(ENTITY_TYPE::P1COLLECTIBLE)
{
	pBd = nullptr;
	pTr = nullptr;
	active = true;
}

P1Collectible::~P1Collectible()
{
	
}

void P1Collectible::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];
	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
		this->pTr = pTr;
	}

	dataRoot = root["Asset"];
	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		//pAs->obj->SetDiffuse(1, 0, 0);
	}

	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "P1Goal";
	for (unsigned int i = 0; i < MemoryManager::GetInstance()->current_position; ++i)
	{
		if ((*MemoryManager::GetInstance()->ObjectPool)[i].pEn->mEntityType == ENTITY_TYPE::P1GOAL)
		{

			mGoalKey = i;
		}
	}
	key = position;
	this->complete = true;
}

void P1Collectible::Update(float deltaTime)
{
	if (active)
	{
		Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
		if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(pBd->mShape, pBd->mPos_x, pBd->mPos_y, pBd->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
		{
			active = false;
			pTr->mScaleX = pTr->mScaleY = pTr->mScaleZ = 0;
			P1Goal* p = static_cast<P1Goal *>((*MemoryManager::GetInstance()->ObjectPool)[mGoalKey].pEn);
			p->ReduceCounter();
		}
	}
}