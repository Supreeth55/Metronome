/**
 * @file ObjectFactoryManager.h
 * @authors Pratyush Gawai, Vineet Amarjeet Dogra
 * @date 9/13/2019
 * @brief This file has methods to load levels and serialize gameobjects.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#pragma once
#include "../Source/pch.h"
#include "../../Dependencies/jsoncpp/json/json.h"
#include "..//Source/Components/Body.h"
class GameObject;
class ObjectFactoryManager
{
public:
	ObjectFactoryManager();
	~ObjectFactoryManager();

	/**
	load object from pFileName
	*/
	void LoadObject(std::string pFileName, unsigned int key);

	/**
	load Level from pFileName
	*/
	void LoadLevel(std::string pFileName);

	/**
	ReSerialize object
	*/
	void ReSerialize(Json::Value objectRoot, unsigned int key);

	/**
	create Fire Bullet
	*/
	void FireBullet(Body* ptr, int);

	/**
	create Drop Box
	*/
	void DropBox(const char* pFileName);

	/**
	Delete RunTime Object
	*/
	void DeleteRunTimeObject();

	/**
	Create Ammo Pack
	*/
	void CreateAmmoPack();

	/**
	Delete Object
	*/
	void DeleteObject();

	/**
	Save Level
	*/
	void SaveLevel();

	/**
	return manager it self
	*/
	static ObjectFactoryManager* GetInstance();

	void Update(float rdeltaTime);

	/**
	delete manager it self
	*/
	void Destroy();

	int start = 0;
	int mUDct = 0;
	

	std::vector<unsigned int> LWallPositions;
	std::vector<unsigned int> LLightPositions;
	std::vector<unsigned int> LBoxPositions;
private:
	std::vector<int> LDynamicList;
	static ObjectFactoryManager* mInstance;
};
