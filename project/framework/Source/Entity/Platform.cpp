#include "../pch.h"
#include "Platform.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

Platform::Platform() : Entity(ENTITY_TYPE::PLATFORM)
{
}

Platform::~Platform() {}

void Platform::Serialize(Json::Value root, unsigned int position)
{

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	Json::Value dataRoot = root["Transform"];
	
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
		//pAs->obj->SetDiffuse(0.2f, 0.2f, 0.7f);
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
	
	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Platform";
	key = position;
	this->complete = true;
}
void Platform::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);
	if (pAn)
		pAn->Update(deltaTime);
}
