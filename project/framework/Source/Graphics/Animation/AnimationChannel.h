/**
 * @file AnimationChannel.h
 * @author Ching-Yen Lin.
 * @date 9/21/19
 * @brief Channel for animations
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */

#pragma once
//#define _HAS_STD_BYTE 0
#include "../../pch.h"
using namespace std;
struct PositionKey
{
	PositionKey(double in_time, Vector3 in_V)
	{
		Time = in_time;
		V = in_V;
	}
	double Time;
	Vector3 V;
};

struct RotationKey
{
	RotationKey(double in_time, Vector4 in_Q)
	{
		Time = in_time;
		Q = in_Q;
	}
	double Time;
	Vector4 Q;
};
struct ScalingKey
{
	ScalingKey(double in_time, Vector3 in_S)
	{
		Time = in_time;
		S = in_S;
	}
	double Time;
	Vector3 S;
};
class AnimationChannel
{
public:
	AnimationChannel();
	~AnimationChannel();
	
	/**
	Adds position key
	*/
	void AddPositionKey(double time, float v0, float v1, float v2);

	/**
		Adds rotation key
	*/
	void AddRotationKey(double time, float v0, float v1, float v2, float v3);
	
	/**
		Adds addScaling key
	*/
	void AddScalingKey(double time, float v0, float v1, float v2);
	
	/**
		Interpolates the animation channel
	*/
	void Interpolate(float t);
	Matrix tran;
private:
	vector<PositionKey> vlist;
	vector<RotationKey> qlist;
	vector<ScalingKey> slist;

};
/**
	Translation LERP
*/
Vector3 TranslationLERP(Vector3 v0, Vector3 v1, float t);

/**
	Scale LERP
*/
Vector3 ScaleLERP(Vector3 s0, Vector3 s1, float t);
