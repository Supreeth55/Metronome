/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: MoveEvent.cpp
Purpose: The class implementation of the Move Event. This event can be used to move particular objects.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 20/10/2019
- End Header --------------------------------------------------------*/
#include "../pch.h"
#include "MoveEvent.h"

MoveEvent::MoveEvent() : Event(EVENT_TYPE::MOVE_EVENT, 0)
{

}

MoveEvent::MoveEvent(float time) : Event(EVENT_TYPE::MOVE_EVENT, time)
{
	//this is for calling timed events.
	//may be useful for scripting the events
}

MoveEvent::~MoveEvent()
{

}