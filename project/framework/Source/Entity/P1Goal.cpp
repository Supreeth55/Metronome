#include "../pch.h"
#include "P1Goal.h"
#include "..\..\Manager\CollisionManager.h"
#include "..\..\Manager\GraphicsManager.h"
#include "..\..\Manager\ImGUIManager.h"

P1Goal::P1Goal() : Entity(ENTITY_TYPE::P1GOAL)
{
	mGoalCounter = 0;
	mGoalReq = 3;
}
P1Goal::~P1Goal()
{

}

void P1Goal::Serialize(Json::Value root, unsigned int position)
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

	dataRoot = root["Goal"];
	if (!dataRoot.isNull())
	{
		dataRoot = dataRoot["Value"];
		if (!dataRoot.isNull())
		{
			mGoalReq = dataRoot.asInt();
		}	
	}

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "P1Goal";
	key = position;
	this->complete = true;
}

void P1Goal::Update(float deltaTime)
{
	Body* b2 = (*MemoryManager::GetInstance()->ObjectPool)[2].pBd;
	if (!CollisionManager::GetInstance()->CheckCollisionCuboidCuboid(pBd->mShape, pBd->mPos_x, pBd->mPos_y, pBd->mPos_z, b2->mShape, b2->mPos_x, b2->mPos_y, b2->mPos_z))
	{
		if (mGoalCounter == mGoalReq)
		{
			GraphicsManager::GetInstance()->mCompletion = true;
		}
		if (mGoalCounter < mGoalReq)
		{
			GraphicsManager::GetInstance()->mNeedCompletion = true;
		}
	}
}

void P1Goal::ReduceCounter()
{
	if (mGoalCounter < mGoalReq)
	{
		mGoalCounter++;
	}
}