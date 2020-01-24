/**
 * @file Animation.cpp
 * @authors Ching-Yen Lin.
 * @date 9/25/19
 * @brief Animation component file.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#include "../pch.h"
#include "Animation.h"
#include "../GameObject.h"
#include "../../Manager/MemoryManager.h"
#include "../Graphics/Shape.h"
//#include "../../Manager/ScriptManager.h"
Animation::Animation() : Component(ANIMATION)
{
	mTotalTime = 0.0f;
	mCurrentStatus = "";
	mSpeed=0.0f;
	misLoop=false;
	key = -1;
}


Animation::~Animation()
{
}

void Animation::Update(float deltaTime)
{
	mTotalTime += deltaTime;

	while (LAnimationChain.size())
	{
		if (LAnimationChain.front().AnimationTotalTime/ LAnimationChain.front().AnimationSpeed > mTotalTime)
		{
			mCurrentStatus = LAnimationChain.front().AnimationName;
			mSpeed = LAnimationChain.front().AnimationSpeed;
			return;
		}
		if (misLoop)
		{
			LAnimationChain.push(LAnimationChain.front());
		}
		LAnimationChain.pop();
		mTotalTime = 0;
	}
	mTotalTime = 0;
	mCurrentStatus = "";



}
void Animation::Serialize(Json::Value root, unsigned int key)
{
	/*sol::state state;
	state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
	state.script_file("Source/Script/Animation.lua");
	string animation = state["config"]["animation"];*/

	//initial like this
	
	mSpeed = 1.0f;
	misLoop = true;
	//lAnimationData.push_back({ "Armature|start", 1.0, 8.0f / 24.0f });
	//lAnimationData.push_back({ "Armature|walk", 1.0, 7.0f / 24.0f });
	//lAnimationData.push_back({ "Armature|stop", 1.0, 30.0f / 24.0f });
	Json::Value objectRoot = root["AnimationList"];
	for (unsigned int i = 0; i < objectRoot.size(); i++)
	{
		LAnimationData.push_back({ objectRoot[i][0].asString(),
			objectRoot[i][1].asFloat(),
			objectRoot[i][2].asFloat() / objectRoot[i][3].asFloat() });
	}
	objectRoot = root["AnimationAddtionalList"];
	for (unsigned int i = 0; i < objectRoot.size(); i++)
	{
		FBX* pFBX = static_cast<FBX*>((*MemoryManager::GetInstance()->ObjectPool)[key].pAs->tshape);
		pFBX->AddAnimationFromFile("./Resource/Model/" + objectRoot[i][0].asString(), objectRoot[i][1].asString());
	}

	//lAnimationChain.push({ "mixamo.com", 1.0, 546 / 30.0f });
	//lAnimationChain.push({ "dance2", 1.0, 460 / 30.0f });
	//lAnimationChain.push({ "run", 1.0, 19 / 30.0f });
	//lAnimationChain.push({ "roll", 1.0, 76 / 30.0f });
	LAnimationChain.push({ "idle", 1.0, 59 / 30.0f });

	//summe there is animation chain
	/*lAnimationChain.push({ "Armature|start", 1.0, 8.0f / 24.0f });
	lAnimationChain.push({ "Armature|walk", 1.0, 30.0f / 24.0f });
	lAnimationChain.push({ "Armature|walk", 1.0, 30.0f / 24.0f });
	lAnimationChain.push({ "Armature|walk", 1.0, 30.0f / 24.0f });
	lAnimationChain.push({ "Armature|walk", 1.0, 30.0f / 24.0f });
	lAnimationChain.push({ "Armature|stop", 1.0, 7.0f / 24.0f });*/
		
	
	(*MemoryManager::GetInstance()->ObjectPool)[key].pAn = this;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pAn->mpEnowner = (*MemoryManager::GetInstance()->ObjectPool)[key].pEn;
	
	this->key = key;
}

void Animation::DeSerialize(Json::Value& root)
{
	root["Animation"] = (Json::objectValue);
}

/**
  This function takes in name of the animation and its time creates a small animation chain
*/

void Animation::ChangeAnimation(unsigned int walks, std::string A_start, std::string A_walk, std::string A_stop, float speed, float time_start, float time_walk, float time_stop)
{
	while (!LAnimationChain.empty())
	{
		LAnimationChain.pop();
	}
	misLoop = false;
	LAnimationChain.push({ A_start, speed, time_start });
	for (unsigned int i = 0; i < walks; ++i)
	{
		LAnimationChain.push({ A_walk, speed, 30.0f / 24.0f });
	}
	LAnimationChain.push({ A_stop, speed, time_stop });
}

/**
   This function takes in name of the animation and its time and stops the animation
*/
void Animation::ChangeAnimationStop(std::string A_stop, float speed, float time)
{

	while (!LAnimationChain.empty())
	{
		LAnimationChain.pop();
	}
	LAnimationChain.push({ A_stop, speed, time });
	misLoop = false;
}

/**
   This function takes in name of the animation and its time creates a walking animation
*/
void Animation::WalkAnimation(std::string walk_name, float speed, float time)
{
	if (!LAnimationChain.empty())
	{
		if (LAnimationChain.front().AnimationName == walk_name)
		{
			LAnimationChain.front().AnimationSpeed = speed;
			return;
		}
	}
	while (!LAnimationChain.empty())
	{
		LAnimationChain.pop();
	}
	LAnimationChain.push({ walk_name, speed, time });
	mTotalTime = 0.0f;
	misLoop = 1;
}

void Animation::SetCurrentStatus(string name)
{
	mTotalTime = 0.0f;
	if (LAnimationChain.front().AnimationName == name)
	{
		return;
	}
	for (size_t i = 0; i < LAnimationData.size(); i++)
	{
		if (LAnimationData[i].AnimationName == name)
		{
			queue<AnimationData>empty;
			swap(LAnimationChain, empty);
			LAnimationChain.push(LAnimationData[i]);
			break;
		}
	}
}

void Animation::AddAnimationChain(string name)
{
	for (size_t i = 0; i < LAnimationData.size(); i++)
	{
		if (LAnimationData[i].AnimationName == name)
		{
			LAnimationChain.push(LAnimationData[i]);
			break;
		}
	}
}

float Animation::GetTotalTime()
{
	return mTotalTime;
}

string Animation::GetCurrentStatus()
{

	return mCurrentStatus;
}

float Animation::GetSpeed()
{
	return mSpeed;
}

bool Animation::IsLoop()
{
	return misLoop;
}

void Animation::HandleEvent(Event *EventToHandle)
{

}
