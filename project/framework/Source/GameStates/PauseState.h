/**
 * @file PauseState.h
 * @authors  Romil Tendulkar
 * @date 11/29/2019
 * @brief Used for handling of the pause state. This state is used for pause functionality

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once

#include "GameState.h"
#include "../Engine.h"


class PauseState : public GameState
{
public:
	PauseState();
	~PauseState();
	
	/**
		Loads the level and required objects

		\param _owner | pointer to the manager of the state
		\param level | name of the level file
	*/
	void Init(GameStateManager* _owner, std::string level = "");

	/**
		Destroys all objects in the level
	*/
	void CleanUp();

	/**
		Pause the state
	*/
	void Pause();

	/**
		Resume the state
	*/
	void Resume();

	/**	
		returns the current instance of the state
		
		\return the pointer to the current instance of this object
	*/
	static PauseState* GetInstance() {
		return &mPauseState;
	}

	/**
		Handle input for this state
	*/
	void HandleInput();

	/**
		Update functionality for the state
	*/
	void UpdateState();

	/**
		Draw all objects for the state
	*/
	void Draw();

	/**
		Reset the level 
		\param _level | name of the level file
	*/
	void Reset(std::string _level = "");

private:
	static PauseState mPauseState;
};

