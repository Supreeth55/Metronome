#include "../pch.h"
#include "Bullet.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

Bullet::Bullet() : Entity(ENTITY_TYPE::BULLET)
{
	key = -1;
	pBd = nullptr;
}

Bullet::~Bullet() {}

void Bullet::Serialize(Json::Value root, unsigned int position)
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

	//Asset
	dataRoot = root["Asset"];
	
	if (!dataRoot.isNull())
	{
		Asset* pAs = new Asset();
		pAs->Serialize(dataRoot, position);
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}

	//Animation
	dataRoot = root["Animation"];

	if (!dataRoot.isNull())
	{
		Animation* pAn = new Animation();
		pAn->Serialize(dataRoot, position);
		this->pAn = pAn;
	}
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Bullet";
	key = position;
	this->complete = true;
	
}

void Bullet::Update(float deltaTime)
{
	if (pBd->mVelo_x != 0.0f || pBd->mVelo_y != 0.0f)
	{
		framecounter++;
	}
	if (framecounter >= 20)
	{
		framecounter = 0;
		pBd->active = false;
		pBd->mVelo_x = 0.0f;
		pBd->mVelo_y = 0.0f;
		pTr->mScaleX = 0.0f;
		pTr->mScaleY = 0.0f;
		pTr->mScaleZ = 0.0f;
	}
	if (pBd->active)
	{
		pTr->mScaleX = 0.3f;
		pTr->mScaleY = 0.3f;
		pTr->mScaleZ = 0.3f;
	}
	if (!pBd->active)
	{
		pTr->mScaleX = 0.0f;
		pTr->mScaleY = 0.0f;
		pTr->mScaleZ = 0.0f;
	}
	if (pBd)
		pBd->Update(deltaTime);
	if (pAn)
		pAn->Update(deltaTime);
}
