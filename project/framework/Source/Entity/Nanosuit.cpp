#include "../pch.h"
#include "Nanosuit.h"
#include "../Components/Transform.h"
#include "../Components/Asset.h"
#include "../Components/Body.h"

Nanosuit::Nanosuit() : Entity(ENTITY_TYPE::NANOSUIT)
{
	
}

Nanosuit::~Nanosuit() {}

void Nanosuit::Serialize(Json::Value root, unsigned int position)
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
	}


	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn = this;

	(*MemoryManager::GetInstance()->ObjectPool)[position].pEn->mEntityName = "Nanosuit";
	key = position;
	this->complete = true;

}

void Nanosuit::Update(float deltaTime)
{

}
