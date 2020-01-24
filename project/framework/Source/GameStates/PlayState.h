/**
 * @file PlayState.h
 * @authors  Romil Tendulkar
 * @date 11/29/2019
 * @brief This state is used for gameplay

 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
#include "GameState.h"
#include "..\..\Manager\PhysicsManager.h"
#include "..\..\\Manager\CollisionManager.h"

enum class PROT_TYPE
{
	RPG,
	STEALTH,
	RUNNER
};


class PlayState : public GameState
{
public:
	PlayState(int type);
	~PlayState();

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
		Update functionality for the state
	*/
	void UpdateState();

	/**
		Draw all objects for the state
	*/
	void Draw();

	/**
		Set the type for various gameplay functionalities
	*/
	void SetType(PROT_TYPE type);
	/*static PlayState* GetInstance() 
	{
		return &mPlayState;
	};*/
	virtual void Reset(std::string _level = "");

private:
	//static PlayState mPlayState;
	std::vector<std::string> levels;
	PROT_TYPE mPType;
};