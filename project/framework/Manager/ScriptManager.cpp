/**
 * @file ScriptManager.cpp
 * @author Supreeth Rao Pejawar
 * @date 10/01/19
 * @brief This file has templatized functions to initialise, convert to lua argument, to convert to lua class and to process the data  written in the lua script
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#include "../Source/pch.h"
#include "ScriptManager.h"


ScriptManager::ScriptManager()
	:
	mpLuaState(nullptr),
	minitialized(false)
{
}



ScriptManager::~ScriptManager()
{
	if (minitialized)
	{
		mpLuaState->script(mscriptVarName + " = nil");
	}
}



bool ScriptManager::HasFunction(const std::string &fnName)
{
	return minitialized && (mluaObjectData[fnName].valid());
}




const std::string& ScriptManager::GetScriptVarName()const
{
	return mscriptVarName;
}



const std::string& ScriptManager::GetLuaClassName()const
{
	return mluaClassName;
}




std::string ScriptManager::convertArgsToLuaArgs()
{
	return "";
}




std::string ScriptManager::convertArgsToLuaArgs(const std::string &str)
{
	return "\'" + str + "\'";
}




std::string ScriptManager::convertArgsToLuaArgs(const char *str)
{
	return convertArgsToLuaArgs(std::string(str));
}