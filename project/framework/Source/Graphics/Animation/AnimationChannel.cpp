/**
 * @file AnimationChannel.h
 * @author Ching-Yen Lin.
 * @date 9/21/19
 * @brief Channel for animations
 *  Copyright (C) 2019 DigiPen Institute of Technology.
	Reproduction or disclosure of this file or its contents without the prior
	written consent of DigiPen Institute of Technology is prohibited.
 */


#include "../../pch.h"
#include "AnimationChannel.h"
#include "../../Util/Math.h"

AnimationChannel::AnimationChannel()
{
}


AnimationChannel::~AnimationChannel()
{
}

void AnimationChannel::AddPositionKey(double time, float v0, float v1, float v2)
{
	vlist.push_back(PositionKey(time, Vector3(v0, v1, v2)));
}

void AnimationChannel::AddRotationKey(double time, float v0, float v1, float v2, float v3)
{
	qlist.push_back(RotationKey(time, Vector4(v0, v1, v2, v3)));
}

void AnimationChannel::AddScalingKey(double time, float v0, float v1, float v2)
{
	slist.push_back(ScalingKey(time, Vector3(v0, v1, v2)));
}

void AnimationChannel::Interpolate(float t)
{
	Vector3 p;
	MYQUATERNION r;
	Vector3 s;
	if (t <= vlist.front().Time)
	{
		p = vlist.front().V;
	}
	else if (t >= vlist.back().Time)
	{
		p = vlist.back().V;
	}
	else
	{
		UINT frontIndex = 0;
		UINT backIndex = vlist.size();
		while (backIndex - frontIndex > 1)
		{
			int m = frontIndex + (backIndex - frontIndex) / 2;

			if (vlist[m].Time == t)
			{
				frontIndex = m;
				backIndex = m+1;
				break;
			}
			else if (vlist[m].Time > t)
			{
				backIndex = m;
			}
			else //LArkLengthTable[m].y<d
			{
				frontIndex = m;
			}
		}
		float newt = (float)(t - vlist[frontIndex].Time);
		p = TranslationLERP(vlist[frontIndex].V, vlist[backIndex].V, newt);
	}

	if (t <= qlist.front().Time)
	{
		r = MYQUATERNION(qlist.front().Q.w,qlist.front().Q.x, qlist.front().Q.y, qlist.front().Q.z);
	}
	else if (t >= qlist.back().Time)
	{
		r = MYQUATERNION(qlist.back().Q.w,qlist.back().Q.x, qlist.back().Q.y, qlist.back().Q.z);
	}
	else
	{
		UINT frontIndex = 0;
		UINT backIndex = vlist.size();
		while (backIndex - frontIndex > 1)
		{
			int m = frontIndex + (backIndex - frontIndex) / 2;

			if (qlist[m].Time == t)
			{
				frontIndex = m;
				backIndex = m + 1;
				break;
			}
			else if (qlist[m].Time > t)
			{
				backIndex = m;
			}
			else //LArkLengthTable[m].y<d
			{
				frontIndex = m;
			}
		}
		float newt = (float)(t - qlist[frontIndex].Time);
		r = MYQUATERNION::Slerp(newt, MYQUATERNION(qlist[frontIndex].Q.w, qlist[frontIndex].Q.x, qlist[frontIndex].Q.y, qlist[frontIndex].Q.z)
			, MYQUATERNION(qlist[backIndex].Q.w, qlist[backIndex].Q.x, qlist[backIndex].Q.y, qlist[backIndex].Q.z));
	}

	if (t <= slist.front().Time)
	{
		s = slist.front().S;
	}
	else if (t >= slist.back().Time)
	{
		s = slist.back().S;
	}
	else
	{
		UINT frontIndex = 0;
		UINT backIndex = vlist.size();
		while (backIndex - frontIndex > 1)
		{
			int m = frontIndex + (backIndex - frontIndex) / 2;

			if (slist[m].Time == t)
			{
				frontIndex = m;
				backIndex = m + 1;
				break;
			}
			else if (slist[m].Time > t)
			{
				backIndex = m;
			}
			else //LArkLengthTable[m].y<d
			{
				frontIndex = m;
			}
		}
		float newt = (float)(t - slist[frontIndex].Time);
		s = ScaleLERP(slist[frontIndex].S, slist[backIndex].S, newt);
	}
	Matrix posM = Matrix::CreateTranslation(p).Transpose();
	Matrix rotationM = Matrix(&r.ToMatrix()[0][0]);
	Matrix scaM = Matrix::CreateScale(s);
	tran = posM* rotationM*scaM;
}

Vector3 TranslationLERP(Vector3 v0, Vector3 v1, float t)
{
	float x = (1.0f - t)*v0.x + v1.x * t;
	float y = (1.0f - t)*v0.y + v1.y * t;
	float z = (1.0f - t)*v0.z + v1.z * t;
	return Vector3(x, y, z);
}

Vector3 ScaleLERP(Vector3 s0, Vector3 s1, float t)
{
	float x = s0.x*powf(s1.x / s0.x, t);
	float y = s0.y*powf(s1.y / s0.y, t);
	float z = s0.z*powf(s1.z / s0.z, t);
	return Vector3(x, y, z);
}
