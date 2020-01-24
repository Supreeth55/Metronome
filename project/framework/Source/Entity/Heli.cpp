#include "../pch.h"
#include "Heli.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

Heli::Heli() : Entity(ENTITY_TYPE::HELI)
{
	key = -1;
	pBd = nullptr;
}

Heli::~Heli() {}

void Heli::Serialize(Json::Value root, unsigned int position)
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
	}

	//Body
	dataRoot = root["Body"];

	if (!dataRoot.isNull())
	{
		Body* pBd = new Body();
		pBd->Serialize(dataRoot, position);
		this->pBd = pBd;
	}


	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Heli";
	key = position;
	this->complete = true;

}

void Heli::Update(float deltaTime)
{
	if (pBd)
		pBd->Update(deltaTime);

}
