/**
 * @file Controller.cpp
 * @authors Vineet Amarjeet Dogra
 * @date 10/1/19
 * @brief This file governs how the controller component is supposed to work and manages certain scenarios where the player should or should not be able to move.
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../pch.h"
#include "Controller.h"
#include "Transform.h"
#include "../GameObject.h"
#include "../../Manager/DXInputManager.h"
#include "..//..//Manager/MemoryManager.h"
#include "../../Manager/ObjectFactoryManager.h"
#include "../../Manager/GraphicsManager.h"
#include "../../Manager/ScriptManager.h"
#include "../../Manager/AudioManager.h"
#include "../../Manager/LevelEditor.h"


#include "..//Entity/Player.h"
#include "..//Entity/Bullet.h"
#include "..//Entity/HitboxH.h"
#include "..//Entity/HitBoxV.h"

 /**
	In this constructor we initialize the variables.
 */
float epsilon = 0.1f;
Controller::Controller() : Component(CONTROLLER)
{
	key = -1;
	debug = false;
}

Controller::~Controller()
{
}

/**
   This functions serializes the controller component.
*/


void Controller::Serialize(Json::Value root, unsigned int key)
{
	this->key = key;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pCr = this;
	(*MemoryManager::GetInstance()->ObjectPool)[key].pCr->mpEnowner = (*MemoryManager::GetInstance()->ObjectPool)[key].pEn;
}

void Controller::DeSerialize(Json::Value& root)
{
	root["Controller"] = (Json::objectValue);
}

/**
   This functions updates the position or runtime object generation by the player based on the key inputs. It also governs if the player is able to move in a particular direction or not.
   \param rdeltaTime | Delta time between frames.
*/

void Controller::Update(float deltaTime)
{
	sol::state state;
	
	state.open_libraries(sol::lib::base, sol::lib::package);
	std::string package_path = state["package"]["path"];
	state["package"]["path"] = (package_path + ";Source/Script/middleclass.lua").c_str();
	state.script_file("Source/Script/Controller.lua");
	
	ScriptManager obj(&state, "Class");




	float mpositiony = state["config"]["mpositiony"];
	float mpositionx = state["config"]["mpositionx"];
	float mpositionz = state["config"]["mpositionz"];

	float StopNum = state["config"]["StopNum"];
	float StartNum = state["config"]["StartNum"];
	float WalkNum = state["config"]["WalkNum"];

	float StopDen = state["config"]["StopDen"];
	float StartDen = state["config"]["StartDen"];
	float WalkDen = state["config"]["WalkDen"];

	float speed = state["config"]["speed"];

	int walks = state["config"]["noofsteps"];

	//on surface movement
	float l_velo_x_s = state["left_surface"]["mVelo_x"];
	float l_velo_y_s = state["left_surface"]["mVelo_y"];
	float l_velo_z_s = state["left_surface"]["mVelo_z"];

	float r_velo_x_s = state["right_surface"]["mVelo_x"];
	float r_velo_y_s = state["right_surface"]["mVelo_y"];
	float r_velo_z_s = state["right_surface"]["mVelo_z"];

	float u_velo_x_s = state["up_surface"]["mVelo_x"];
	float u_velo_y_s = state["up_surface"]["mVelo_y"];
	float u_velo_z_s = state["up_surface"]["mVelo_z"];

	float d_velo_x_s = state["down_surface"]["mVelo_x"];
	float d_velo_y_s = state["down_surface"]["mVelo_y"];
	float d_velo_z_s = state["down_surface"]["mVelo_z"];

	//on falling movement
	float l_velo_x_f = state["left_falling"]["mVelo_x"];
	float l_velo_y_f = state["left_falling"]["mVelo_y"];
	float l_velo_z_f = state["left_surface"]["mVelo_z"];

	float r_velo_x_f = state["right_falling"]["mVelo_x"];
	float r_velo_y_f = state["right_falling"]["mVelo_y"];
	float r_velo_z_f = state["right_falling"]["mVelo_z"];

	float u_velo_x_f = state["up_falling"]["mVelo_x"];
	float u_velo_y_f = state["up_falling"]["mVelo_y"];
	float u_velo_z_f = state["up_falling"]["mVelo_z"];

	float d_velo_x_f = state["down_falling"]["mVelo_x"];
	float d_velo_y_f = state["down_falling"]["mVelo_y"];
	float d_velo_z_f = state["down_falling"]["mVelo_z"];

	//space movement
	float l_velo_x_space = state["left_space"]["mVelo_x"];
	float l_velo_y_space = state["left_space"]["mVelo_y"];
	float l_velo_z_space = state["left_space"]["mVelo_z"];

	float r_velo_x_space = state["right_space"]["mVelo_x"];
	float r_velo_y_space = state["right_space"]["mVelo_y"];
	float r_velo_z_space = state["right_space"]["mVelo_z"];

	float u_velo_x_space = state["up_space"]["mVelo_x"];
	float u_velo_y_space = state["up_space"]["mVelo_y"];
	float u_velo_z_space = state["up_space"]["mVelo_z"];

	float d_velo_x_space = state["down_space"]["mVelo_x"];
	float d_velo_y_space = state["down_space"]["mVelo_y"];
	float d_velo_z_space = state["down_space"]["mVelo_z"];



	std::string shape = state["config"]["shape"];
	const char* shape_char = shape.c_str();

	

	std::string walk_animation = state["config"]["walk_animation"];
	std::string stop_animation = state["config"]["stop_animation"];
	std::string start_animation = state["config"]["start_animation"];

	//std::string walk_animation = "Armature|walk";
	//std::string start_animation = "Armature|start";
	//std::string stop_animation = "Armature|stop";
	//state2.do_file("Source/scripts/Animation.lua");

	if (ammo_check == 0)
	{
		ObjectFactoryManager::GetInstance()->CreateAmmoPack();
		ammo_check++;
	}

	float StopTime = obj.CallFunction<float>("StopTime", StopNum, StopDen);
	
	float StartTime = obj.CallFunction<float>("StartTime", StartNum, StartDen);
	float WalkTime = obj.CallFunction<float>("WalkTime", WalkNum, WalkDen);
	//std::cout << "Return Value: " << WalkTime << std::endl;
	//float velo_y_space = state["config"]["mVelo_y"];
	//float velo_x_ = state["config"]["mVelo_y"];


	//unsigned int key = MemoryManager::GetInstance()->current_position;
	//	if ((*MemoryManager::GetInstance()->ObjectPool)[key].pCr)
	if(usable)
	{
		Transform* pTr = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr;
		Body* pBd = (*MemoryManager::GetInstance()->ObjectPool)[key].pBd;
		Animation* pAn = (*MemoryManager::GetInstance()->ObjectPool)[key].pAn;
		Player* pl = static_cast<Player*>((*MemoryManager::GetInstance()->ObjectPool)[key].pEn);
		HitBoxH* hx = static_cast<HitBoxH*>((*MemoryManager::GetInstance()->ObjectPool)[0].pEn);
		HitBoxV* hv = static_cast<HitBoxV*>((*MemoryManager::GetInstance()->ObjectPool)[1].pEn);
		//hx->mFrameCounter = 20;
		if (LevelEditor::GetInstance()->levelEditoris_on == false)
		{
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_C) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_X))
			{
				//lAnimationChain.push({ "roll", 1.0, 76 / 30.0f });
				AudioManager::GetInstance()->LoadAudio("Bite-SoundBible.com.wav");
				pl->mDashCounter = 30;
				rolling = true;
				pAn->WalkAnimation("roll",2.43f, 35 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_UP) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP)) && (DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->body_State == ON_SURFACE)
			{
				pBd->mLeftMovement = false;
				pBd->mRightMovement = false;
				pBd->mDownMovement = false;
				pBd->mVelo_y = u_velo_y_space;
				pBd->mFacingUp = true;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
			}
			//if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->mCollidingSideOut == true && pBd->body_State == FALLING)
			//{
			//	//pTr->mPositionX += 0.1;
			//	pBd->mDownMovement = true;
			//	pBd->mLeftMovement = false;
			//	pBd->mRightMovement = false;
			//	pBd->mUpMovement = false;
			//	pBd->mVelo_y = -3;
			//	pBd->mVelo_z = 5;
			//	pBd->mFacingUp = false;
			//	pBd->mFacingDown = true;
			//	pBd->mFacingRight = false;
			//	pBd->mFacingLeft = false;
			//}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_UP) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP)) && pBd->mUpMovement == true && rolling == false &&hx->mFrameCounter<=0 && hv->mFrameCounter<=0)
			{
				//pTr->mPositionY += mpositiony;
				pBd->mVelo_y = u_velo_y_s;
				pBd->mFacingUp = true;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
				//lAnimationChain.push({ "run", 1.0, 19 / 30.0f });
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 180;
				if (pAn)
					pAn->WalkAnimation("run", 1, 19 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_UP) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP)) && pBd->mUpMovement == true && pBd->body_State == FALLING)
			{
				pBd->mLeftMovement = false;
				pBd->mRightMovement = false;
				pBd->mDownMovement = false;
				pBd->mVelo_y = u_velo_y_f;
				pBd->mFacingUp = true;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_DOWN) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_DOWN)) && (DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->body_State == ON_SURFACE)
			{
				pBd->mLeftMovement = false;
				pBd->mRightMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_y = d_velo_y_space;
				pBd->mFacingUp = false;
				pBd->mFacingDown = true;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
			}
			//if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_A)) && pBd->mCollidingSideIn == true && pBd->body_State == FALLING)
			//{
			//	//pTr->mPositionX += 0.1;
			//	pBd->mDownMovement = false;
			//	pBd->mLeftMovement = false;
			//	pBd->mRightMovement = false;
			//	pBd->mUpMovement = true;
			//	pBd->mVelo_y = 3;
			//	pBd->mVelo_z = 5;
			//	pBd->mFacingUp = true;
			//	pBd->mFacingDown = false;
			//	pBd->mFacingRight = false;
			//	pBd->mFacingLeft = false;
			//}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_DOWN) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->mDownMovement == true && pBd->body_State == FALLING)
			{
				//pTr->mPositionY -= mpositiony;
				pBd->mLeftMovement = false;
				pBd->mRightMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_y = d_velo_y_s;
				pBd->mFacingUp = false;
				pBd->mFacingDown = true;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_DOWN) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_DOWN)) && pBd->mDownMovement == true && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				//pTr->mPositionY -= mpositiony;
				pBd->mVelo_y = d_velo_y_f;
				pBd->mFacingUp = false;
				pBd->mFacingDown = true;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = false;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 0;
				if (pAn)
					pAn->WalkAnimation("run", 1, 19 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_LEFT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && (DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_A)) && pBd->body_State == ON_SURFACE)
			{
				pBd->mDownMovement = false;
				pBd->mRightMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_x = l_velo_x_space;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = true;
			}
			//if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadTrigger(GAMEPAD_A)) && pBd->mCollidingSideLeft == true && pBd->body_State == FALLING)
			//{
			//	//pTr->mPositionX += 0.1;
			//	pBd->mDownMovement = false;
			//	pBd->mLeftMovement = false;
			//	pBd->mRightMovement = true;
			//	pBd->mUpMovement = false;
			//	pBd->mVelo_x = 2;
			//	pBd->mVelo_z = 10;
			//	pBd->mFacingUp = false;
			//	pBd->mFacingDown = false;
			//	pBd->mFacingRight = true;
			//	pBd->mFacingLeft = false;
			//}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_LEFT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && pBd->mLeftMovement == true && pBd->body_State == FALLING)
			{
				pBd->mDownMovement = false;
				pBd->mRightMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_x = l_velo_x_s;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = true;
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_LEFT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && pBd->mLeftMovement == true && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				//pTr->mPositionX -= 0.1;
				pBd->mVelo_x = l_velo_x_f;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = false;
				pBd->mFacingLeft = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = -90;
				if (pAn)
					pAn->WalkAnimation("run", 1, 19 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_RIGHT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) && pBd->body_State == ON_SURFACE)
			{
				pBd->mDownMovement = false;
				pBd->mLeftMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_x = r_velo_x_space;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = true;
				pBd->mFacingLeft = false;
			}
			else if ((DXInputManager::GetInstance()->isKeyPressed(DIK_RIGHT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && pBd->mRightMovement == true && pBd->body_State && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				//pTr->mPositionX += 0.1;
				pBd->mVelo_x = r_velo_x_f;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = true;
				pBd->mFacingLeft = false;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 90;
				if (pAn)
					pAn->WalkAnimation("run", 1, 19 / 30.0f);
			}
			//if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->mCollidingSideRight == true && pBd->body_State == FALLING)
			//{
			//	//pTr->mPositionX += 0.1;
			//	pBd->mDownMovement = false;
			//	pBd->mLeftMovement = true;
			//	pBd->mRightMovement = false;
			//	pBd->mUpMovement = false;
			//	pBd->mVelo_x = -3;
			//	pBd->mVelo_z = 5;
			//	pBd->mFacingUp = false;
			//	pBd->mFacingDown = false;
			//	pBd->mFacingRight = false;
			//	pBd->mFacingLeft = true;
			//}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_RIGHT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && pBd->mRightMovement == true && pBd->body_State == FALLING)
			{
				//pTr->mPositionX += 0.1;
				pBd->mDownMovement = false;
				pBd->mLeftMovement = false;
				pBd->mUpMovement = false;
				pBd->mVelo_x = r_velo_x_s;
				pBd->mFacingUp = false;
				pBd->mFacingDown = false;
				pBd->mFacingRight = true;
				pBd->mFacingLeft = false;
			}

			//if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_SPACE) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && pBd->body_State == ON_SURFACE)
			//{
			//	//pTr->mPositionZ += 1;
			//	pBd->body_State = FALLING;
			//	pBd->mVelo_z = 8;
			//}

			//if (DXInputManager::GetInstance()->isKeyPressed(DIK_M))
			//{
			//	//pTr->mPositionZ -= mpositionz;
			//	pBd->mPos_z -= 0.1f;
			//}
		
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_F))
			{
			ObjectFactoryManager::GetInstance()->DropBox("Block");
			}
			if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_Z) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_A)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				Attack(pBd, 1);
				//pAn->WalkAnimation("roll", 1, 35 / 30.0f);
				pAn->WalkAnimation("hatk", 1, 72.0f / 30.0f);
				AudioManager::GetInstance()->LoadAudio("Realistic_Punch-Mark_DiAngelo.wav");
				//cout << "atk" << endl;
				/*ObjectFactoryManager::GetInstance()->CreateRunTimeObject("Hitbox", pBd,1);
				std::cout << (*MemoryManager::GetInstance()->ObjectPool)[MemoryManager::GetInstance()->current_position - 1].pEn->mEntityName << std::endl;*/

			}
			if ((DXInputManager::GetInstance()->isKeyTrigger(DIK_X) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_B)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				Attack(pBd, 2);
				AudioManager::GetInstance()->LoadAudio("Realistic_Punch-Mark_DiAngelo.wav");
				pAn->WalkAnimation("vatk", 1, 68.0f / 30.0f);
			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_DELETE))
			{
				//ObjectFactoryManager::GetInstance()->DeleteRunTimeObject();
			}
			if (DXInputManager::GetInstance()->isKeyPressed(DIK_0))
			{
				if (pAn)
				{
					pAn->ChangeAnimation(walks, start_animation, walk_animation, stop_animation, speed, StartTime, WalkTime, StopTime);
				}
			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_I))
			{
				debug = !debug;
				GraphicsManager::GetInstance()->SetDebug(debug);

			}
			if (DXInputManager::GetInstance()->isKeyPressed(DIK_1))
			{
				if (pAn)
					pAn->ChangeAnimationStop(stop_animation, speed, StopTime);

			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_UP) && DXInputManager::GetInstance()->isKeyPressed(DIK_LEFT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP) && DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				pBd->mFacingUp = true;
				pBd->mFacingLeft = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = -135;
				if (pAn)
					pAn->WalkAnimation("run", 1, 19 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_UP) && DXInputManager::GetInstance()->isKeyPressed(DIK_RIGHT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_UP) && DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				pBd->mFacingUp = true;
				pBd->mFacingRight = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 135;
				if (pAn)
					pAn->WalkAnimation("run", 1, 19 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_DOWN) && DXInputManager::GetInstance()->isKeyPressed(DIK_LEFT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_DOWN) && DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_LEFT)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				pBd->mFacingDown = true;
				pBd->mFacingLeft = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = -45;
				if (pAn)
					pAn->WalkAnimation("run", 1, 19 / 30.0f);
			}
			if ((DXInputManager::GetInstance()->isKeyPressed(DIK_DOWN) && DXInputManager::GetInstance()->isKeyPressed(DIK_RIGHT) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_DOWN) && DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_RIGHT)) && rolling == false && hx->mFrameCounter <= 0 && hv->mFrameCounter <= 0)
			{
				pBd->mFacingDown = true;
				pBd->mFacingRight = true;
				(*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mRotationZ = 45;
				if (pAn)
					pAn->WalkAnimation("run", 1, 19 / 30.0f);
			}
			if (DXInputManager::GetInstance()->isKeyTrigger(DIK_K) || DXInputManager::GetInstance()->GamepadPressed(GAMEPAD_Y))
			{
				int begin = ObjectFactoryManager::GetInstance()->start;
				for (int i = begin; i < ObjectFactoryManager::GetInstance()->start + 9; i++)
				{
					if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->active && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_x == 0 && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_y == 0)//if active fire.
					{
						Bullet* b = static_cast<Bullet*>((*MemoryManager::GetInstance()->ObjectPool)[i].pEn);
						b->framecounter = 0;
						ObjectFactoryManager::GetInstance()->FireBullet(pBd, i);
						AudioManager::GetInstance()->LoadAudio("Small Fireball-SoundBible.com.wav");
						break;
					}
					else if ((*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_x == 0 && (*MemoryManager::GetInstance()->ObjectPool)[i].pBd->mVelo_y == 0)
					{
						Bullet* b = static_cast<Bullet*>((*MemoryManager::GetInstance()->ObjectPool)[i].pEn);
						b->framecounter = 0;
						(*MemoryManager::GetInstance()->ObjectPool)[i].pBd->active = true;
						ObjectFactoryManager::GetInstance()->FireBullet(pBd, i);
						break;
					}
				}
			}
			

			if (abs(pBd->mVelo_x) <=epsilon && abs(pBd->mVelo_y) <= epsilon && pl->mDashCounter<=0 &&hx->mFrameCounter<=0 &&hv->mFrameCounter<=0)
			{
				
				pAn->WalkAnimation("idle", 1, 59.0f / 30.0f);
			}
			if (pl->mDashCounter <= 0)
			{
				rolling = false;
			}
		}
	}
}

void Controller::Attack(Body* ptr, int atkm)
{
	if (atkm == 1)
	{
		HitBoxH* hx = static_cast<HitBoxH*>((*MemoryManager::GetInstance()->ObjectPool)[0].pEn);
		hx->mFrameCounter = 60;
		(*MemoryManager::GetInstance()->ObjectPool)[0].pBd->active = true;
		if (ptr->mFacingDown == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX = ptr->mPos_x;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY = ptr->mPos_y - ptr->mDepth - 1;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX = ptr->mPos_x - ptr->mWidth - 1;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY = ptr->mPos_y;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX = ptr->mPos_x + ptr->mWidth + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY = ptr->mPos_y;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingUp == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX = ptr->mPos_x;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY = ptr->mPos_y + ptr->mDepth + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ = ptr->mPos_z;
		}
		(*MemoryManager::GetInstance()->ObjectPool)[0].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionX;
		(*MemoryManager::GetInstance()->ObjectPool)[0].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[0].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[0].pTr->mPositionZ;
	}
	else if (atkm == 2)
	{
		HitBoxV* hx = static_cast<HitBoxV*>((*MemoryManager::GetInstance()->ObjectPool)[1].pEn);
		hx->mFrameCounter = 60;
		(*MemoryManager::GetInstance()->ObjectPool)[1].pBd->active = true;
		if (ptr->mFacingDown == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX = ptr->mPos_x;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY = ptr->mPos_y - ptr->mDepth - 1;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingLeft == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX = ptr->mPos_x - ptr->mWidth - 1;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY = ptr->mPos_y;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingRight == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX = ptr->mPos_x + ptr->mWidth + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY = ptr->mPos_y;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ = ptr->mPos_z;
		}
		else if (ptr->mFacingUp == true)
		{
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX = ptr->mPos_x;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY = ptr->mPos_y + ptr->mDepth + 1;
			(*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ = ptr->mPos_z;
		}

		(*MemoryManager::GetInstance()->ObjectPool)[1].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionX;
		(*MemoryManager::GetInstance()->ObjectPool)[1].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[1].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[1].pTr->mPositionZ;


	}
	/*if ((*MemoryManager::GetInstance()->ObjectPool)[key].pBd)
	{
		(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_x = (*MemoryManager::GetInstance()->ObjectPool)[key].pTr->mPositionX;
		(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_y = (*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionY;
		(*MemoryManager::GetInstance()->ObjectPool)[key].pBd->mPos_z = (*MemoryManager::GetInstance()->ObjectPool)[curKey].pTr->mPositionZ;
	}*/
}

void Controller::HandleEvent(Event* EventToHandle)
{

}