#ifndef MATH_H
#define MATH_H
#include "../pch.h"
const float pi = 3.14159f;

class VEC2
{
public:
	float V[2];
	VEC2()
	{
		V[0] = 0; V[1] = 0;
	}
	VEC2(float x, float y)
	{
		V[0] = x; V[1] = y;
	}
	VEC2(float a)
	{
		V[0] = V[1] = a;
	}
	VEC2 operator-(const VEC2 in) {
		VEC2 out; out.V[0] = V[0] - in.V[0];
		out.V[1] = V[1] - in.V[1]; return out;
	}
	VEC2 operator+(const VEC2 in) {
		VEC2 out; out.V[0] = V[0] + in.V[0];
		out.V[1] = V[1] + in.V[1]; return out;
	}
	VEC2 operator*(const float in) {
		VEC2 out; out.V[0] = V[0] * in;
		out.V[1] = V[1] * in; return out;
	}
	VEC2 operator/(const float in) {
		VEC2 out; out.V[0] = V[0] / in;
		out.V[1] = V[1] / in; return out;
	}
	float& operator[](const int i) { return V[i]; }
	const float& operator[](const int i) const { return V[i]; }
};
class IVEC3
{
public:
	int V[3];
	IVEC3()
	{
		V[0] = 0; V[1] = 0; V[2] = 0;
	}
	IVEC3(int x, int y, int z)
	{
		V[0] = x; V[1] = y; V[2] = z;
	}
	IVEC3(int a)
	{
		V[0] = V[1] = a; V[2] = a;
	}
	IVEC3 operator-(const IVEC3 in) {
		IVEC3 out; out.V[0] = V[0] - in.V[0];
		out.V[1] = V[1] - in.V[1];
		out.V[2] = V[2] - in.V[2]; return out;
	}
	IVEC3 operator+(const IVEC3 in) {
		IVEC3 out; out.V[0] = V[0] + in.V[0];
		out.V[1] = V[1] + in.V[1];
		out.V[2] = V[2] + in.V[2]; return out;
	}
	IVEC3 operator*(const float in) {
		IVEC3 out; out.V[0] = V[0] * int(in);
		out.V[1] = V[1] * int(in);
		out.V[2] = V[2] * int(in); return out;
	}
	IVEC3 operator/(const float in) {
		IVEC3 out; out.V[0] = V[0] / int(in);
		out.V[1] = V[1] / int(in);
		out.V[2] = V[2] / int(in); return out;
	}
	int& operator[](const int i) { return V[i]; }
	const int& operator[](const int i) const { return V[i]; }
};
class VEC3
{
public:
	float V[3];
	VEC3()
	{
		for (int i = 0; i < 3; i++)
		{
			V[i] = 0;
		}
	}
	VEC3(float x, float y, float z)
	{
		V[0] = x; V[1] = y; V[2] = z;
	}
	VEC3(float a)
	{
		V[0] = V[1] = V[2] = a;
	}
	VEC3 operator-(const VEC3 in) {
		VEC3 out; out.V[0] = V[0] - in.V[0];
		out.V[1] = V[1] - in.V[1];
		out.V[2] = V[2] - in.V[2]; return out;
	}
	VEC3 operator+(const VEC3 in) {
		VEC3 out; out.V[0] = V[0] + in.V[0];
		out.V[1] = V[1] + in.V[1];
		out.V[2] = V[2] + in.V[2]; return out;
	}
	VEC3 operator*(const float in) {
		VEC3 out; out.V[0] = V[0] * in;
		out.V[1] = V[1] * in;
		out.V[2] = V[2] * in; return out;
	}
	VEC3 operator/(const float in) {
		VEC3 out; out.V[0] = V[0] / in;
		out.V[1] = V[1] / in;
		out.V[2] = V[2] / in; return out;
	}
	float& operator[](const int i) { return V[i]; }
	const float& operator[](const int i) const { return V[i]; }
};

class VEC4
{
public:
	float V[4];
	VEC4()
	{
		for (int i = 0; i < 4; i++)
		{
			V[i] = 0;
		}
	}
	VEC4(float x, float y, float z, float w)
	{
		V[0] = x; V[1] = y; V[2] = z; V[3] = w;
	}
	VEC4(float a)
	{
		V[0] = V[1] = V[2] = V[3] = a;
	}
	VEC4 operator-(const VEC4 in) {
		VEC4 out; out.V[0] = V[0] - in.V[0];
		out.V[1] = V[1] - in.V[1];
		out.V[2] = V[2] - in.V[2];
		out.V[3] = V[3] - in.V[3]; return out;
	}
	VEC4 operator+(const VEC4 in) {
		VEC4 out; out.V[0] = V[0] + in.V[0];
		out.V[1] = V[1] + in.V[1];
		out.V[2] = V[2] + in.V[2];
		out.V[3] = V[3] + in.V[3]; return out;
	}
	VEC4 operator*(const float in) {
		VEC4 out; out.V[0] = V[0] * in;
		out.V[1] = V[1] * in;
		out.V[2] = V[2] * in;
		out.V[3] = V[3] * in; return out;
	}
	VEC4 operator/(const float in) {
		VEC4 out; out.V[0] = V[0] / in;
		out.V[1] = V[1] / in;
		out.V[2] = V[2] / in;
		out.V[3] = V[3] / in; return out;
	}
	float& operator[](const int i) { return V[i]; }
	const float& operator[](const int i) const { return V[i]; }

};

class MAT4
{
public:
	float M[4][4];
	//identity
	typedef float ROW4[4];
	MAT4()
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				M[i][j] = i == j ? 1.0f : 0.0f;
	}
	MAT4(const float *pArray)
	{
		M[0][0] = pArray[0];
		M[0][1] = pArray[1];
		M[0][2] = pArray[2];
		M[0][3] = pArray[3];

		M[1][0] = pArray[4];
		M[1][1] = pArray[5];
		M[1][2] = pArray[6];
		M[1][3] = pArray[7];

		M[2][0] = pArray[8];
		M[2][1] = pArray[9];
		M[2][2] = pArray[10];
		M[2][3] = pArray[11];

		M[3][0] = pArray[12];
		M[3][1] = pArray[13];
		M[3][2] = pArray[14];
		M[3][3] = pArray[15];
	}
	ROW4& operator[](const int i) { return M[i]; }
	const ROW4& operator[](const int i) const { return M[i]; }
	MAT4(const MAT4& q) {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				M[i][j] = q[i][j];
			}
		}
	}
	MAT4& operator=(const MAT4& q) {
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				M[i][j] = q[i][j];
			}
		}
		return *this;
	}
	float* Pntr();
	void print();
};
class MYQUATERNION
{
public:
	MYQUATERNION();
	MYQUATERNION(float is, float ix, float iy, float iz);
	MYQUATERNION(float scalar, const VEC3& v);
	MYQUATERNION(const VEC4& v);
	MYQUATERNION(const float *pArray);

	MYQUATERNION(const MYQUATERNION& q);
	MYQUATERNION& operator=(const MYQUATERNION& q);

	MYQUATERNION(MYQUATERNION&& q);
	MYQUATERNION& operator=(MYQUATERNION&& q);

	operator VEC4() const;

	// Comparison operators
	bool operator == (const MYQUATERNION& q) const;
	bool operator != (const MYQUATERNION& q) const;

	// Assignment operators
	MYQUATERNION& operator= (const VEC4& F);
	MYQUATERNION& operator+= (const MYQUATERNION& q);
	MYQUATERNION& operator-= (const MYQUATERNION& q);
	MYQUATERNION& operator*= (const MYQUATERNION& q);
	MYQUATERNION& operator*= (float S);
	MYQUATERNION& operator/= (const MYQUATERNION& q);

	// Unary operators
	MYQUATERNION operator+ () const;
	MYQUATERNION operator- () const;

	// MYQUATERNION operations
	float Length() const;
	float LengthSquared() const;

	void Normalize();
	void Normalize(MYQUATERNION& result) const;

	void Rotate(VEC3);
	void ToMatrix(MAT4&);
	MAT4 ToMatrix();

	void Conjugate();
	void Conjugate(MYQUATERNION& result) const;

	void Inverse(MYQUATERNION& result) const;

	float Dot(const MYQUATERNION& Q) const;

	// Static functions
	//static MYQUATERNION CreateFromAxisAngle(const Vector3& axis, float angle);
	//static MYQUATERNION CreateFromYawPitchRoll(float yaw, float pitch, float roll);
	//static MYQUATERNION CreateFromRotationMatrix(const Matrix& M);

	//static void Lerp(float t, const MYQUATERNION& q1, const MYQUATERNION& q2, MYQUATERNION& result);
	//static MYQUATERNION Lerp(float t, const MYQUATERNION& q1, const MYQUATERNION& q2);

	static void Slerp(float t, const MYQUATERNION& Q1, const MYQUATERNION& Q2, MYQUATERNION& result);
	static MYQUATERNION Slerp(float t, const MYQUATERNION& Q1, const MYQUATERNION& Q2);

	//static void Concatenate(const MYQUATERNION& q1, const MYQUATERNION& q2, MYQUATERNION& result);
	//static MYQUATERNION Concatenate(const MYQUATERNION& q1, const MYQUATERNION& q2);

	// Constants
	static const MYQUATERNION Identity;

	float s; float x; float y; float z;
	float Angle;
	VEC3 Axis;
};

// Binary operators
MYQUATERNION operator+ (const MYQUATERNION& Q1, const MYQUATERNION& Q2);
MYQUATERNION operator- (const MYQUATERNION& Q1, const MYQUATERNION& Q2);
MYQUATERNION operator* (const MYQUATERNION& Q1, const MYQUATERNION& Q2);
MYQUATERNION operator* (const MYQUATERNION& Q, float S);
MYQUATERNION operator/ (const MYQUATERNION& Q1, const MYQUATERNION& Q2);
MYQUATERNION operator* (float S, const MYQUATERNION& Q);


MAT4 Rotate(const int i, const float theta);
MAT4 Scale(const float x, const float y, const float z);
MAT4 Translate(const float x, const float y, const float z);
MAT4 Perspective(const float rx, const float ry,
	const float front, const float back);
MAT4 LookAt(const float cameraX, const float cameraY, const float cameraZ,
	const float centerX, const float centerY, const float centerZ,
	const float cameraUpX, const float cameraUpY, const float cameraUpZ);

Matrix MyLookAt(Vector3 rpos, Vector3 rlook_dir, Vector3 rup);
Vector3 PerpendicularFinder(Vector3 rvector);
Matrix MyPerspective(const float rx, const float ry,
	const float front, const float back);

MAT4 operator* (const MAT4 A, const MAT4 B);
VEC3 cross(VEC3 a, VEC3 b);
VEC3 normalize(VEC3 x);
VEC4 normalize(VEC4 x);
float dot(VEC3 x, VEC3 y);
float dot(VEC4 x, VEC4 y);
bool invert(const MAT4* mat, MAT4* inv);

#endif 1
