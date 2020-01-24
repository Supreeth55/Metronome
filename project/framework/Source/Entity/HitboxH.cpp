#include "../pch.h"
#include "HitBoxH.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"
#include "../../Manager/MemoryManager.h"

HitBoxH::HitBoxH() : Entity(ENTITY_TYPE::HITBOXH)
{
	key = -1;
	mFrameCounter = 0;
	pBd = nullptr;
}

HitBoxH::~HitBoxH() {}

void HitBoxH::Serialize(Json::Value root, unsigned int position)
{
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;
	Json::Value dataRoot = root["Transform"];

	if ((*MemoryManager::GetInstance()->ObjectPool)[position].pAn)
	{
		std::cout << "trash" << std::endl;
	}

	//Transform
	if (!dataRoot.isNull())
	{
		Transform* pTr = new Transform();
		pTr->Serialize(dataRoot, position);
	}

	//Asset
	dataRoot = root["Asset"];

	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
		this->pAs = pAs;
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		pBd->active = false;

		this->pBd = pBd;
	}

	//Animation
	dataRoot = root["Animation"];

	if (!dataRoot.isNull())
	{
		Animation* pAn = new Animation();
		pAn->Serialize(dataRoot, position);

	}
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "HitBoxH";
	key = position;
	this->complete = true;

}

void HitBoxH::Update(float deltaTime)
{
	//mFrameCounter++;
	if (mFrameCounter > 0)
	{
		mFrameCounter--;
		/*pAs->tshape->mActive = true;*/
		//std::cout << "Horizontal active" << std::endl;
		//MemoryManager::GetInstance()->FreeAt(key);
	}
	if (mFrameCounter <= 0)
	{
		pBd->active = false;
		/*pAs->tshape->mActive = false;*/
		//std::cout << "Horizontal inactive" << std::endl;
	}
	if (pBd)
		pBd->Update(deltaTime);
	//std::cout <<"HB: "<< pBd->mPos_x << ", " << pBd->mPos_y << ", " << pBd->mPos_z << std::endl;

}
