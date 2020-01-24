/**
 * @file EventManager.cpp
 * @author Romil Tendulkar
 * @date 10/10/2019
 * @brief This Class interacts with the events that are required for the game.
		It broadcasts event to objects, adds objects as subscribers to events
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "EventManager.h"
#include "..\Manager\GameObjectManager.h"

/**
	Initializes the owner gamestate for the event manager.
	Also initializes the array of timed events

	\param pOwner | The pointer to the Game state that initialized the Event Manager
*/
EventManager::EventManager(GameObjectManager *pOwner)
{
	mTimedEventsQueue = std::vector<Event *>();
	mpOwner = pOwner;
}

/**
	Clears all events in the timed events array
*/
EventManager::~EventManager()
{
	for (auto me : mTimedEventsQueue)
	{
		delete me;
	}
	mTimedEventsQueue.clear();
}

/**
	The update function updates the timer on all the events in the timed events array

	\param rdeltaTime | Time passed since the last frame. This is used to update the time for the event
*/
void EventManager::Update(float rdeltaTime)
{
	std::vector<Event *>::iterator it= mTimedEventsQueue.begin();
	while (it != mTimedEventsQueue.end())
	{
		Event *pE = *it;
		pE->mEventTime -= rdeltaTime;
		if (pE->mEventTime <= 0.0f)
		{
			BroadcastEvent(pE);
			it = mTimedEventsQueue.erase(it);
		}
		else
		{
			++it;
		}
	}
}

/**
	This function calls the handle event function on all components for all objects

	\param pEventToBroadcast | Pointer to the event that has to be broadcast
*/
void EventManager::BroadcastEvent(Event *pEventToBroadcast)
{
	for (auto c : mpOwner->LGameObject)
	{
		c->HandleEvent(pEventToBroadcast);
	}
}

/**
	This function calls the handle event function for all objects subscribed to the event

	\param pEventToBroadcast | Pointer to the event that has to be broadcast
*/
void EventManager::BroadcastToSubs(Event *pEventToBroadcast)
{
	for (auto c : mSubscriberLists[pEventToBroadcast->mEventType])
	{
		c->HandleEvent(pEventToBroadcast);
	}
}

/**
	This function adds events to the timed events array
	
	\param pEventToAdd | Pointer to the event that is to be added to the timed events array
*/
void EventManager::AddTimedEvent(Event *pEventToAdd)
{
	mTimedEventsQueue.push_back(pEventToAdd);
}

/**
	This function adds objects to the subscription list for an event

	\param type | The type of the Event, this is an enum type declared in the events header
	\param pObj | The pointer to the object that is to be added to the subscribers for the event type
*/
void EventManager::Subscribe(EVENT_TYPE type, GameObject *pObj)
{
	std::unordered_map<EVENT_TYPE, std::vector<GameObject *>>::iterator f = mSubscriberLists.find(type);
	if (f == mSubscriberLists.end())
	{
		std::vector<GameObject *> vec;
		vec.push_back(pObj);
		mSubscriberLists.insert(std::make_pair(type,vec));
	}
	else
	{
		std::vector<GameObject *> vec = f->second;
		vec.push_back(pObj);
	}
}