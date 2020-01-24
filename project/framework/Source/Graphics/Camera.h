

#ifndef CAMERA_H
#define CAMERA_H
#include "../pch.h"
#include "../Util/Math.h"

class Camera
{
public:
	Camera();
	~Camera();

	/**
	Set Position
	*/
	void SetPosition(float x, float y, float z);

	/**
	Get Position
	*/
	void GetPosition(float&, float&, float&);

	/**
	Set Movement
	*/
	void SetMovement(float x, float y, float z);

	/**
	Set Pan
	*/
	void SetPan(float);

	/**
	Set Tilt
	*/
	void SetTilt(float);

	/**
	Set Roll
	*/
	void SetRoll(float);

	/**
	Get Pan Tilt Roll
	*/
	void GetPanTiltRoll(float&, float&,float&);

	/**
	Get View Matrix
	*/
	void GetViewMatrix(Matrix&);

	/**
	Get 3rd Person View
	*/
	void Get3rdPersonView(Matrix &);

	float mTx;
	float mTy;
	float mZoom;


private:
	/**
	Compute View Matrix
	*/
	void ComputeViewMatrix();

	/**
	Compute Rotate Matrix
	*/
	void ComputeRotateMatrix();

	/**
	Compute 3rdPerson View Matrix
	*/
	void Compute3rdPersonViewMatrix();
	float mPan, mTilt, mRoll;
	float mPosX, mPosY, mPosZ;
	Matrix mViewMatrix;
	Matrix m3PViewMatrix;
	Matrix mRotateMatrix;
};
#endif 1

