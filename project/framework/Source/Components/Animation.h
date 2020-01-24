/**
 * @file Animation.h
 * @authors Ching-Yen Lin.
 * @date 9/25/19
 * @brief Animation component file.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#ifndef ANIMATION_H
#define ANIMATION_H
//#define _HAS_STD_BYTE 0
#include "Component.h"
#include "../pch.h"
using namespace std;
struct AnimationData
{
	string AnimationName;
	float AnimationSpeed;
	float AnimationTotalTime;
};
class Animation : public Component
{
public:
	/**
   Set everything to defualt
	*/
	Animation();
	~Animation();

	/**
	compute which the animation should run
	 */
	void Update(float deltaTime);

	/**
	Serialize animation
	 */
	void Serialize(Json::Value root, unsigned int key);

	/**
	DeSerialize animation
	 */
	void DeSerialize(Json::Value& root);

	/**
	set Current Status
	 */
	void SetCurrentStatus(string);

	/**
	Add Animation to the chain
	 */
	void AddAnimationChain(string);


	void HandleEvent(Event *EventToHandle);

	/**
	get Animation Total Time
	 */
	float GetTotalTime();

	/**
	get Animation Status
	 */
	string GetCurrentStatus();

	/**
	get Animation Speed
	 */
	float GetSpeed();

	/**
	chack Animation is Loop
	 */
	bool IsLoop();

	/**
	Change Animation
	 */
	void ChangeAnimation(unsigned int walks, std::string A_name, std::string A_walk, std::string A_stop, float speed, float time_start, float time_walk, float time_stop);
	
	/**
	Stop Animation
	 */
	void ChangeAnimationStop(std::string A_name, float speed, float time);
	
	/**
	Walk Animation
	 */
	void WalkAnimation(std::string walk_name, float speed, float time);
	unsigned int key;
private:
	float mTotalTime;
	string mCurrentStatus;
	float mSpeed;
	bool misLoop;

	vector<AnimationData> LAnimationData;
	queue<AnimationData> LAnimationChain;
	
	
};

#endif