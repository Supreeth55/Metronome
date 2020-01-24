/**
 * @file EventManager.h
 * @author Romil Tendulkar
 * @date 10/10/2019
 * @brief This Class interacts with the events that are required for the game.
		It broadcasts event to objects, adds objects as subscribers to events
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once


#include "../Source/pch.h"
#include "..\Source\Events\Event.h"
//#include "..\Source\GameObject.h"
#include "..\Manager\GameObjectManager.h"

class EventManager
{
public:

	//functions
	EventManager(GameObjectManager *Owner);
	~EventManager();
	void Update(float rdeltaTime);
	void Subscribe(EVENT_TYPE type, GameObject* obj);
	void BroadcastEvent(Event *EventToBroadcast);
	void BroadcastToSubs(Event *EventToBroadcast);
	void AddTimedEvent(Event *EventToAdd);

	//variables
	std::vector<Event *>mTimedEventsQueue;
	std::unordered_map<EVENT_TYPE, std::vector<GameObject *>>mSubscriberLists;
	GameObjectManager *mpOwner;
};