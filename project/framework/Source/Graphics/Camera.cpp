#include "../pch.h"
#include "Camera.h"

Camera::Camera()
{
	mPan = 0.0f; mTilt = 30.0f; mRoll = 0.0f;//45 90
	mPosX = mPosY = mPosZ = 0;
	mTx=0;	mTy = 0; mZoom = 25.0f;
	ComputeViewMatrix();
	Compute3rdPersonViewMatrix();
}
Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	mPosX = x; mPosY = y; mPosZ = z;
	ComputeViewMatrix();
	Compute3rdPersonViewMatrix();
}
void Camera::GetPosition(float& x, float &y, float &z)
{
	x = mPosX; y = mPosY; z = mPosZ;
}
void Camera::SetMovement(float x, float y, float z)
{
	 Vector3 direction =Vector3::Transform(Vector3(x, y, z), mRotateMatrix);
	 mPosX += direction.x; mPosY += direction.y; mPosZ += direction.z;
	 ComputeViewMatrix();
	 Compute3rdPersonViewMatrix();
}
void Camera::SetPan(float v)
{
	mPan = v;
	//std::cout << "P " << v << std::endl;
	ComputeRotateMatrix();
	ComputeViewMatrix();
	Compute3rdPersonViewMatrix();
}

void Camera::SetTilt(float v)
{
	mTilt = v;
	//std::cout << "T " << v << std::endl;
	ComputeRotateMatrix();
	ComputeViewMatrix();
	Compute3rdPersonViewMatrix();
}

void Camera::SetRoll(float v)
{
	mRoll = v;
	//std::cout << "R " << v << std::endl;
	ComputeRotateMatrix();
	ComputeViewMatrix();
	Compute3rdPersonViewMatrix();
}

void Camera::GetPanTiltRoll(float& p, float& t, float& r)
{
	p = mPan;t = mTilt;r = mRoll;
}

//NEED TODO: modify it so only the camera is change and recalculate after that
//			Also change it no recalculate from origin
void Camera::GetViewMatrix(Matrix & vm)
{
	vm = mViewMatrix;
}

void Camera::Get3rdPersonView(Matrix &tpv)
{
	tpv = m3PViewMatrix;
}

void Camera::ComputeViewMatrix()
{
	Vector3 up, position, lookAt;
	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 0.0f;
	up.z = 1.0f;

	// Setup the position of the camera in the world.
	position.x = mPosX;
	position.y = mPosY;
	position.z = mPosZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 1.0f;
	lookAt.z = 0.0f;

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = Vector3::Transform(lookAt, mRotateMatrix);
	up = Vector3::Transform(up, mRotateMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	mViewMatrix = Matrix::CreateLookAt(position, lookAt, up);
}

void Camera::Compute3rdPersonViewMatrix()
{
	Matrix pos = Matrix::CreateTranslation(0, -2, -mZoom);
	Matrix xrot = Matrix::CreateRotationX((mTilt - 90)*DEGREE_TO_RADIAN);
	Matrix zrot = Matrix::CreateRotationZ(mRoll*DEGREE_TO_RADIAN);
	m3PViewMatrix = Matrix::CreateTranslation(mTx, mTy, 0)*zrot*xrot*pos;
}


void Camera::ComputeRotateMatrix()
{
	float pan, tilt, roll;
	// Set the pan (Y axis), Tilt (X axis), and roll (Z axis) rotations in radians.
	pan = mPan * DEGREE_TO_RADIAN;
	tilt = mTilt * DEGREE_TO_RADIAN;
	roll = mRoll * DEGREE_TO_RADIAN;
	// Create the rotation matrix from the yaw, pitch, and roll values.
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, pan, tilt, roll);
	Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(0, 0, roll);
	Matrix rotationMatrix1 = Matrix::CreateFromYawPitchRoll(0, tilt, 0);
	Matrix rotationMatrix2 = Matrix::CreateFromYawPitchRoll(pan, 0, 0);
	mRotateMatrix = rotationMatrix2 * rotationMatrix1*rotationMatrix;
}
