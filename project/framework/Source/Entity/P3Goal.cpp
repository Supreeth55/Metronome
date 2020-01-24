#include "../pch.h"
#include "P3Goal.h"
#include "..\..\Manager\CollisionManager.h"
#include "..\..\Manager\GraphicsManager.h"
#include "..\..\Manager\ImGUIManager.h"

P3Goal::P3Goal() : Entity(ENTITY_TYPE::P3GOAL)
{
	
}
P3Goal::~P3Goal()
{

}

void P3Goal::Serialize(Json::Value root, unsigned int position)
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

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "P3Goal";
	key = position;
	this->complete = true;
}

void P3Goal::Update(float deltaTime)
{
	Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
	if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(pBd->mShape, pBd->mPos_x, pBd->mPos_y, pBd->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
	{

		GraphicsManager::GetInstance()->mCompletion = true;
	}
}
