/**
 * @file Scriptmanager.h
 * @author Supreeth Rao Pejawar
 * @date 10/01/19
 * @brief This file has functions related to creating a level editor
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */
#pragma once
#define SOL_ALL_SAFETIES_ON 1

#include "../Source/pch.h"


class ScriptManager
{
public:
	ScriptManager();
	template <typename...Args>
	explicit ScriptManager(sol::state *pLuaState, const std::string &luaClassName, const Args&...args);
	ScriptManager(const ScriptManager&other) = delete;
	ScriptManager& operator=(const ScriptManager&other) = delete;

	/**
		In the destructor, the object is set  to nil and then let the garbage collection do all the work.
	*/
	~ScriptManager();

	template <typename...Args>
	bool Init(sol::state *pLuaState, const std::string &scriptName, const Args&...args);

	/**
		This functions takes the function name as the function parameter and returns after checking if the  function exists and if it is valid
	*/
	bool HasFunction(const std::string &fnName);

	/**
		This function returns the variables names
	*/
	const std::string& GetScriptVarName()const;

	/**
		 This function returns the the lua class name names
	*/
	const std::string& GetLuaClassName()const;
	template <typename RetType = void, typename...Args>
	RetType CallFunction(const std::string &fnName, const Args&...args);
	template <typename RetType = void, typename...Args>
	RetType UnsafeCallFunction(const std::string &fnName, const Args&...args);
	template <typename T>
	T Get(const std::string& name) const;
	template <typename T>
	bool  TryGet(const std::string& name, T* pValue) const;
	template <typename T>
	void Set(const std::string& name, const T& value);
private:
	template <typename T, typename...Args>
	static std::string convertArgsToLuaArgs(const T& t, const Args&...args);
	template <typename T>
	static std::string convertArgsToLuaArgs(const T& t);

	/**
		This function is used to convert the lua argument into argument, by taking in the string as the parameter
	*/
	static std::string convertArgsToLuaArgs(const std::string &str);

	/**
		This function is used to convert the argument into lua argument with const string parameter
	*/
	static std::string convertArgsToLuaArgs(const char* str);

	/**
		Our usage works in a recursive manner, in which a variable is ‘processed?in every recursion, and removed from the parameter pack. It will continue on until there’s no more arguments and will call the empty function.
		Due to that, we have to define an empty function with the same name.
`	*/
	static std::string convertArgsToLuaArgs();
private:
	std::string mluaClassName;
	std::string mscriptVarName;
	sol::table mluaObjectData;
	sol::state *mpLuaState;
	bool minitialized;
};
template <typename...Args>
ScriptManager::ScriptManager(sol::state *pLuaState, const std::string &luaClassName, const Args&...args)
	:
	mpLuaState(nullptr),
	minitialized(false)
{
	Init(pLuaState, luaClassName, args...);
}

/**

	This function creates a Lua object of type luaClassName and gives them a unique name for use in the luaState. The name is then saved in m_scriptVarName.
	A lambda function is created in this function to suppress an exception being thrown if the lua code called is not valid.

*/
template <typename...Args>
bool ScriptManager::Init(sol::state *pLuaState, const std::string &luaClassName, const Args&...args)
{
	if (!pLuaState) return false;
	mpLuaState = pLuaState;
	minitialized = true;
	mluaClassName = luaClassName;
	static std::size_t scriptID = 0;
	mscriptVarName = mluaClassName + "_" + std::to_string(scriptID++).c_str();
	bool isValidCreation = true;
	std::string script = mscriptVarName + " = " + mluaClassName + ":new(" + convertArgsToLuaArgs(args...) + ")";
	mpLuaState->script(script,
		[&isValidCreation](lua_State* state, sol::protected_function_result res) { isValidCreation = false; return res; });
	if (isValidCreation)
	{
		mluaObjectData = (*mpLuaState)[mscriptVarName];
	}
	bool success = isValidCreation && mluaObjectData.valid();
	if (!success)
	{
		minitialized = false;
		mluaClassName = "";
		mscriptVarName = "";
		mluaObjectData = sol::userdata();
	}
	return success;
}

/**
	this function can take multiple amount of argumentsand convert it into a format usable as input for our Lua functions.

*/
template <typename T, typename...Args>
std::string ScriptManager::convertArgsToLuaArgs(const T& t, const Args&...args)
{
	return convertArgsToLuaArgs(t) + "," + convertArgsToLuaArgs(args...);
}
template <typename T>
std::string ScriptManager::convertArgsToLuaArgs(const T& t)
{
	return std::to_string(t);
}

/**
	This helper function helps us call the lua object’s member functions.
*/
template <typename RetType, typename...Args>
RetType ScriptManager::CallFunction(const std::string &fnName, const Args&...args)
{
	if (minitialized && mpLuaState)
	{
		if (HasFunction(fnName))
			return UnsafeCallFunction<RetType, Args...>(fnName, args...);
	}
	return NULL;
}

/**
	This function creates the function to a safe function call
*/
template <typename RetType, typename...Args>
RetType ScriptManager::UnsafeCallFunction(const std::string &fnName, const Args&...args)
{
	std::string scriptStr = std::string("return ") + mscriptVarName + ":" + fnName + "(" + convertArgsToLuaArgs(args...) + ")";
	return static_cast<RetType>(mpLuaState->script(scriptStr));
}

/**
	This function gets data initialised in the lua class
*/
template <typename T>
T ScriptManager::Get(const std::string& name) const
{
	return m_luaObjectData[name];
}

/**
	This function gets the memeber and its value from the function and checks if the value is valid  and returns 
*/
template <typename T>
bool ScriptManager::TryGet(const std::string& name, T* pValue) const
{
	bool valid = m_luaObjectData[name].valid();
	if (valid && pValue)
	{
		*pValue = m_luaObjectData[name];
	}
	return valid && pValue;
}

/**
	This function sets the value of the object thats created in the function
*/
template <typename T>
void ScriptManager::Set(const std::string& name, const T& value)
{
	m_luaObjectData[name] = value;
}

