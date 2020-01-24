/**
 * @file Controller.cpp
 * @authors Vineet Amarjeet Dogra
 * @date 10/1/19
 * @brief This file governs how the controller component is supposed to work and manages certain scenarios where the player should or should not be able to move.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "../pch.h"
#include "Component.h"
class Body;
class Controller : public Component
{
public:
	Controller();
	~Controller();
	void Update(float deltaTime);
	
	void HandleEvent(Event *EventToHandle);

	void Serialize(Json::Value root, unsigned int key);
	void DeSerialize(Json::Value& root);

	void Attack(Body* ptr, int atkm);

	unsigned int key;
	bool debug;
	int ammo_check = 0;
	bool usable = true;

	bool rolling = false;
private:
	
};
#endif // !CONTROLLER