/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: GameState.h
Purpose: Header for the GameState class. This class shall be used to maintain the current state
of the game. i.e.:- Pause, Play, Menu states.
Platform: Windows 10, x64
Project: romil.tendulkar, GAM550
Author: Romil Tendulkar, romil.tendulkar, 60001218
Creation date: 26/09/2019
- End Header --------------------------------------------------------*/

#include "../pch.h"
#include "PlayState.h"
#include "..\GameObject.h"
#include "..\..\Manager\ObjectFactoryManager.h"
#include "..\..\Manager\AudioManager.h"
#include "..\..\Manager\ImGUIManager.h"
#include "..\..\Manager\MemoryManager.h"



PlayState::PlayState(int type) : GameState(GAME_STATE_TYPE::PLAY_STATE)
{
	if (type == 0)
	{
		mPType = PROT_TYPE::RPG;
	}
	if (type == 1)
	{
		mPType = PROT_TYPE::STEALTH;
	}
	if (type == 2)
	{
		mPType = PROT_TYPE::RUNNER;
	}
}

PlayState::~PlayState()
{
	//destroy the managers
}

void PlayState::Init(GameStateManager* _owner, std::string level)
{
	//set the initial values for everything
	ObjectFactoryManager::GetInstance()->LoadLevel(level);
	if (mPType == PROT_TYPE::RPG)
	{
		AudioManager::GetInstance()->LoadLoopingAudio("Ezio's Family.mp3");
	}
	if (mPType == PROT_TYPE::STEALTH)
	{
		AudioManager::GetInstance()->LoadLoopingAudio("cat_vs_dog-dominik_hauser-128_proud_music_preview.mp3");
	}
	if (mPType == PROT_TYPE::RUNNER)
	{
		AudioManager::GetInstance()->LoadLoopingAudio("Escape_Chase.mp3");
	}
	mpOwner = _owner;
	MemoryManager::GetInstance()->mProtType = mPType;
	//things like load level go here
}

void PlayState::CleanUp()
{
	// reset camera
	// reset graphics window width and height


	//reset anything else thats created
}

void PlayState::UpdateState()
{
	//mPM->Update();
}

void PlayState::Reset(std::string _level)
{
	//set time aswell
	CleanUp();
	//call init
}

void PlayState::Draw()
{
	MSG msg;
	bool done, result;

	//scripting
	sol::state state;
	state.open_libraries(sol::lib::base, sol::lib::package);
	state.script_file("Source/Script/CameraMovement.lua");

	float r_x_q = state["config"]["rotation_x_q"];
	float r_x_e = state["config"]["rotation_x_e"];
	float r_y = state["config"]["rotation_y"];
	float r_z = state["config"]["rotation_z"];

	float sensitivity = state["config"]["sensitivity"];

	float m_x_w = state["config"]["movement_x_w"];
	float m_x_a = state["config"]["movement_x_a"];
	float m_x_d = state["config"]["movement_x_d"];
	float m_x_s = state["config"]["movement_x_s"];

	float m_y_w = state["config"]["movement_y_w"];
	float m_y_a = state["config"]["movement_y_a"];
	float m_y_s = state["config"]["movement_y_s"];
	float m_y_d = state["config"]["movement_y_d"];

	float m_z_w = state["config"]["movement_z_w"];
	float m_z_a = state["config"]["movement_z_a"];
	float m_z_s = state["config"]["movement_z_s"];
	float m_z_d = state["config"]["movement_z_d"];

	float m_sensitivity = state["config"]["movement_sensitivity"];

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	TimerManager::GetInstance()->Reset();
	// Loop until there is a quit message from the window or the user.
	done = false;
	while (!done)
	{
		/*Engine::invoke();
		if (Engine::IOcheck.load() == INPUT_TAKEN)
		{
			Engine::IOcheck.store(INPUT_IDLE);
			InputManagerThread->join();
			delete InputManagerThread;
			InputManagerThread = nullptr;
		}*/
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		TimerManager::GetInstance()->Tick();
		mpOwner->pOwner->CalculateFrameStats();
		mpOwner->pOwner->deltaTime = TimerManager::GetInstance()->getDeltaTime();
		// If windows signals to end the application then exit out.
		if (!mPaused)
		{
			if (msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				result = mpOwner->pOwner->Frame(mpOwner->pOwner->deltaTime);
				if (!result)
				{
					done = true;
				}
			}
		}
		
		GraphicsManager::GetInstance()->Update(mpOwner->pOwner->deltaTime);
		mpOwner->pOwner->CallInputManager();

		//demo the input controll camera
		//auto dis = DXInputManager::GetInstance()->GetMouseWheel();
		//GraphicsManager::GetInstance()->setCameraDistance(float(dis / 50));
		//if (DXInputManager::GetInstance()->isMousePressed(0) == true)
		//{
		//	int dX, dY;

		//	DXInputManager::GetInstance()->GetMouseDelta(dX, dY);//stop mouse input if want capture mouse is true, to stop camera update on interacting with imgui windows
		//	GraphicsManager::GetInstance()->setCameraRotation(0.0f, (float)dY, (float)dX, 3.0f);
		//}

		/*auto zoomout = DXInputManager::GetInstance()->GamepadRightTriggers();
		GraphicsManager::GetInstance()->setCameraDistance(zoomout);

		auto zoomin = DXInputManager::GetInstance()->GamepadLeftTriggers();
		GraphicsManager::GetInstance()->setCameraDistance(-zoomin);

		float x, y;
		DXInputManager::GetInstance()->GamepadRightThumbSticks(y, x);
		GraphicsManager::GetInstance()->setCameraRotation(0.0f, x * 2, y * 2, 3.0f);*/
		if (mPType == PROT_TYPE::STEALTH)
		{
			GraphicsManager::GetInstance()->GetCamera()->SetPan(0);
			GraphicsManager::GetInstance()->GetCamera()->SetTilt(66);
			GraphicsManager::GetInstance()->GetCamera()->mZoom = 25;
		}
		if (mPType == PROT_TYPE::RUNNER|| mPType== PROT_TYPE::RPG)
		{
			GraphicsManager::GetInstance()->GetCamera()->SetPan(0);
			GraphicsManager::GetInstance()->GetCamera()->SetTilt(41);
			GraphicsManager::GetInstance()->GetCamera()->mZoom = 41;
		}

		
		/*if (DXInputManager::GetInstance()->isKeyPressed(DIK_Q) == true)
		{
			GraphicsManager::GetInstance()->setCameraRotation(r_x_q, r_y, r_z, sensitivity);
		}
		if (DXInputManager::GetInstance()->isKeyPressed(DIK_E) == true)
		{
			GraphicsManager::GetInstance()->setCameraRotation(r_x_e, r_y, r_z, sensitivity);
		}
		if (DXInputManager::GetInstance()->isKeyPressed(DIK_W) == true)
		{
			GraphicsManager::GetInstance()->setCameraMovement(m_x_w, m_y_w, m_z_w, m_sensitivity);
		}
		if (DXInputManager::GetInstance()->isKeyPressed(DIK_S) == true)
		{
			GraphicsManager::GetInstance()->setCameraMovement(m_x_s, m_y_s, m_z_s, m_sensitivity);
		}
		if (DXInputManager::GetInstance()->isKeyPressed(DIK_A) == true)
		{
			GraphicsManager::GetInstance()->setCameraMovement(m_x_a, m_y_a, m_z_a, m_sensitivity);
		}
		if (DXInputManager::GetInstance()->isKeyPressed(DIK_D) == true)
		{
			GraphicsManager::GetInstance()->setCameraMovement(m_x_d, m_y_d, m_z_d, m_sensitivity);
		}*/
		if (DXInputManager::GetInstance()->isKeyRelease(DIK_TAB) == true)
		{
			mPaused = !mPaused;
			GraphicsManager::GetInstance()->mPaused = mPaused;
		}
		/*if (DXInputManager::GetInstance()->isKeyTrigger(DIK_O) == true)
		{
			GraphicsManager::GetInstance()->mCompletion = !GraphicsManager::GetInstance()->mCompletion;
		}*/
		
		//CollisionManager::GetInstance()->ResetGrid();
		if (!mPaused)
		{
			GameObjectManager::GetInstance()->Update(mpOwner->pOwner->deltaTime);
		}
	}
}

void PlayState::SetType(PROT_TYPE type)
{
	mPType = type;
}
