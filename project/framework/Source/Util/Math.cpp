#include "../pch.h"
#include "Math.h"

float * MAT4::Pntr()
{
	return &(M[0][0]);
}

void MAT4::print()
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			printf("%9.4f%c", M[i][j], j == 3 ? '\n' : ' ');
	printf("\n");
}

MAT4 Rotate(const int i, const float theta)
{
	MAT4 R;
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	R[j][j] = R[k][k] = cos(theta*pi / 180.0f);
	R[k][j] = sin(theta*pi / 180.0f);
	R[j][k] = -R[k][j];
	return R;
}

MAT4 Scale(const float x, const float y, const float z)
{
	MAT4 S;
	S[0][0] = x;
	S[1][1] = y;
	S[2][2] = z;
	return S;
}

MAT4 Translate(const float x, const float y, const float z)
{
	MAT4 T;
	T[0][3] = x;
	T[1][3] = y;
	T[2][3] = z;
	return T;
}

//this is right hand side prespective and it has boild-in transpose
MAT4 Perspective(const float rx, const float ry, const float front, const float back)
{
	MAT4 P;
	P[0][0] = 1.0f / rx;
	P[1][1] = 1.0f / ry;
	P[2][2] = -(back + front) / (back - front);
	P[2][3] = -(2.0f*front*back) / (back - front);
	P[3][2] = -1;
	return P;
}
//this is right hand side LookAt and it has boild-in transpose
MAT4 LookAt(const float cameraX, const float cameraY, const float cameraZ, const float centerX, const float centerY, const float centerZ, const float cameraUpX, const float cameraUpY, const float cameraUpZ)
{
	VEC3 E(cameraX, cameraY, cameraZ);
	VEC3 C(centerX, centerY, centerZ);
	VEC3 U(cameraUpX, cameraUpY, cameraUpZ);
	VEC3 V = normalize(C - E);
	VEC3 A = normalize(cross(V, U));
	VEC3 B = cross(A, V);
	MAT4 result;
	result[0][0] = A[0]; result[0][1] = A[1]; result[0][2] = A[2]; result[0][3] = -dot(A, E);
	result[1][0] = B[0]; result[1][1] = B[1]; result[1][2] = B[2]; result[1][3] = -dot(B, E);
	result[2][0] = -V[0]; result[2][1] = -V[1]; result[2][2] = -V[2]; result[2][3] = dot(V, E);
	result[3][0] = 0; result[3][1] = 0; result[3][2] = 0; result[3][3] = 1.0f;

	return result;
}

Matrix MyLookAt(Vector3 rpos, Vector3 rlook_dir, Vector3 rup)
{
	Vector3 v= rlook_dir;
	v.Normalize();
	Vector3 a = v.Cross(rup);
	a.Normalize();
	Vector3 b = a.Cross(v);
	Matrix result;
	result._11 = a.x;result._12 = b.x; result._13 = -v.x; result._14 = 0;
	result._21 = a.y;result._22 = b.y; result._23 = -v.y; result._24 = 0;
	result._31 = a.z;result._32 = b.z; result._33 = -v.z; result._34 = 0;
	result._41 = -a.Dot(rpos); result._42 = -b.Dot(rpos); result._43 = v.Dot(rpos); result._44 = 1.0f;

	return result;
}

Vector3 PerpendicularFinder(Vector3 rvector)
{
	return Vector3(0.0f, -1.0f, 0.0f);
	Vector3 test= Vector3(1.0f, 0.0f,(rvector.x) /(rvector.z));
	test.Normalize();
	if (rvector.Dot(test) == 0)return test;
	test = Vector3(1.0f, - rvector.x / (rvector.y), 0.0f);
	test.Normalize();
	if (rvector.Dot(test) == 0)return test;
	test = Vector3(-rvector.y / rvector.x, 1.0f, 0.0f);
	test.Normalize();
	return test;
}

Matrix MyPerspective(const float rx, const float ry, const float front, const float back)
{
	Matrix P;
	P._11 = 1.0f / rx;
	P._22= 1.0f / ry;
	P._33 = -(back + front) / (back - front);
	P._43 = -(2.0f*front*back) / (back - front);
	P._34 = -1;
	return P;
}

MAT4 operator*(const MAT4 A, const MAT4 B)
{
	MAT4 M;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j) {
			M[i][j] = 0.0;
			for (int k = 0; k < 4; ++k)
				M[i][j] += A[i][k] * B[k][j];
		}
	return M;
}

VEC3 cross(VEC3 x, VEC3 y)
{
	return VEC3(x[1] * y[2] - y[1] * x[2],
		x[2] * y[0] - y[2] * x[0],
		x[0] * y[1] - y[0] * x[1]);
}

VEC3 normalize(VEC3 x)
{
	return x / sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
}

VEC4 normalize(VEC4 x)
{
	return x / sqrtf(x[0] * x[0] + x[1] * x[1] + x[2] * x[2] + x[3] * x[3]);
}

float dot(VEC3 x, VEC3 y)
{
	return  x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

float dot(VEC4 x, VEC4 y)
{
	return  x[0] * y[0] + x[1] * y[1] + x[2] * y[2] + x[3] * y[3];
}

////////////////////////////////////////////////////////////////////////
// Calculates (efficently) the inverse of a matrix by performing
// gaussian matrix reduction with partial pivoting followed by
// back/substitution with the loops manually unrolled.
//
// Taken from Mesa implementation of OpenGL:  http://mesa3d.sourceforge.net/
////////////////////////////////////////////////////////////////////////
#define MAT(m,r,c) ((*m)[r][c])
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }

bool invert(const MAT4* mat, MAT4* inv)
{
	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

	r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

	r0[0] = MAT(mat, 0, 0);
	r0[1] = MAT(mat, 0, 1);
	r0[2] = MAT(mat, 0, 2);
	r0[3] = MAT(mat, 0, 3);
	r0[4] = 1.0;
	r0[5] = r0[6] = r0[7] = 0.0;

	r1[0] = MAT(mat, 1, 0);
	r1[1] = MAT(mat, 1, 1);
	r1[2] = MAT(mat, 1, 2);
	r1[3] = MAT(mat, 1, 3);
	r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0;

	r2[0] = MAT(mat, 2, 0);
	r2[1] = MAT(mat, 2, 1);
	r2[2] = MAT(mat, 2, 2);
	r2[3] = MAT(mat, 2, 3);
	r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0;

	r3[0] = MAT(mat, 3, 0);
	r3[1] = MAT(mat, 3, 1);
	r3[2] = MAT(mat, 3, 2);
	r3[3] = MAT(mat, 3, 3);
	r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

	/* choose pivot - or die */
	if (fabs(r3[0]) > fabs(r2[0])) SWAP_ROWS(r3, r2);
	if (fabs(r2[0]) > fabs(r1[0])) SWAP_ROWS(r2, r1);
	if (fabs(r1[0]) > fabs(r0[0])) SWAP_ROWS(r1, r0);
	if (0.0 == r0[0])  return false;

	/* eliminate first variable     */
	m1 = r1[0] / r0[0]; m2 = r2[0] / r0[0]; m3 = r3[0] / r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[1]) > fabs(r2[1])) SWAP_ROWS(r3, r2);
	if (fabs(r2[1]) > fabs(r1[1])) SWAP_ROWS(r2, r1);
	if (0.0 == r1[1])  return false;

	/* eliminate second variable */
	m2 = r2[1] / r1[1]; m3 = r3[1] / r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	/* choose pivot - or die */
	if (fabs(r3[2]) > fabs(r2[2])) SWAP_ROWS(r3, r2);
	if (0.0 == r2[2])  return false;

	/* eliminate third variable */
	m3 = r3[2] / r2[2];
	r3[3] -= m3 * r2[3];
	r3[4] -= m3 * r2[4];
	r3[5] -= m3 * r2[5];
	r3[6] -= m3 * r2[6];
	r3[7] -= m3 * r2[7];

	/* last check */
	if (0.0 == r3[3]) return false;

	s = 1.0F / r3[3];             /* now back substitute row 3 */
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 /* now back substitute row 2 */
	s = 1.0F / r2[2];
	r2[4] = s * (r2[4] - r3[4] * m2);
	r2[5] = s * (r2[5] - r3[5] * m2);
	r2[6] = s * (r2[6] - r3[6] * m2);
	r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
	r1[4] -= r3[4] * m1;
	r1[5] -= r3[5] * m1;
	r1[6] -= r3[6] * m1;
	r1[7] -= r3[7] * m1;
	m0 = r0[3];
	r0[4] -= r3[4] * m0;
	r0[5] -= r3[5] * m0;
	r0[6] -= r3[6] * m0;
	r0[7] -= r3[7] * m0;

	m1 = r1[2];                 /* now back substitute row 1 */
	s = 1.0F / r1[1];
	r1[4] = s * (r1[4] - r2[4] * m1);
	r1[5] = s * (r1[5] - r2[5] * m1);
	r1[6] = s * (r1[6] - r2[6] * m1);
	r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
	r0[4] -= r2[4] * m0;
	r0[5] -= r2[5] * m0;
	r0[6] -= r2[6] * m0;
	r0[7] -= r2[7] * m0;

	m0 = r0[1];                 /* now back substitute row 0 */
	s = 1.0F / r0[0];
	r0[4] = s * (r0[4] - r1[4] * m0);
	r0[5] = s * (r0[5] - r1[5] * m0);
	r0[6] = s * (r0[6] - r1[6] * m0);
	r0[7] = s * (r0[7] - r1[7] * m0);

	MAT(inv, 0, 0) = r0[4];
	MAT(inv, 0, 1) = r0[5],
		MAT(inv, 0, 2) = r0[6];
	MAT(inv, 0, 3) = r0[7],
		MAT(inv, 1, 0) = r1[4];
	MAT(inv, 1, 1) = r1[5],
		MAT(inv, 1, 2) = r1[6];
	MAT(inv, 1, 3) = r1[7],
		MAT(inv, 2, 0) = r2[4];
	MAT(inv, 2, 1) = r2[5],
		MAT(inv, 2, 2) = r2[6];
	MAT(inv, 2, 3) = r2[7],
		MAT(inv, 3, 0) = r3[4];
	MAT(inv, 3, 1) = r3[5],
		MAT(inv, 3, 2) = r3[6];
	MAT(inv, 3, 3) = r3[7];

	return true;
}

MYQUATERNION::MYQUATERNION() {
	s = 1; x = 0; y = 0; z = 0;
}

MYQUATERNION::MYQUATERNION(float is, float ix, float iy, float iz)
	:s(is), x(ix), y(iy), z(iz) {}

MYQUATERNION::MYQUATERNION(float scalar, const VEC3 & v)
	: s(scalar), x(v[0]), y(v[1]), z(v[2]) {}

MYQUATERNION::MYQUATERNION(const VEC4 & v)
	: s(v[0]), x(v[1]), y(v[2]), z(v[3]) {}

MYQUATERNION::MYQUATERNION(const float * pArray) {
	s = *pArray;
	x = *(pArray + 1); y = *(pArray + 2); z = *(pArray + 3);
}
MYQUATERNION::MYQUATERNION(const MYQUATERNION & q) { s = q.s; x = q.x; y = q.y; z = q.z; }

MYQUATERNION& MYQUATERNION::operator=(const MYQUATERNION& q) { s = q.s; x = q.x; y = q.y; z = q.z; return *this; }

MYQUATERNION::MYQUATERNION(MYQUATERNION && q) { *this = q; }

MYQUATERNION& MYQUATERNION::operator=(MYQUATERNION&& q) { *this = q; return *this; }

MYQUATERNION::operator VEC4() const { return VEC4(s, x, y, z); }

// MYQUATERNION operations
bool MYQUATERNION::operator == (const MYQUATERNION& q) const
{
	return(q.s == s && q.x == x && q.y == y && q.z == z);
}
bool MYQUATERNION::operator != (const MYQUATERNION& q) const
{
	return !(q.s == s && q.x == x && q.y == y && q.z == z);
}
MYQUATERNION& MYQUATERNION::operator= (const VEC4& F) {
	s = F[0]; x = F[1]; y = F[2]; z = F[3]; return *this;
}
MYQUATERNION& MYQUATERNION::operator+= (const MYQUATERNION& q)
{
	s += q.s; x += q.x; y += q.y; z += q.z;
	return *this;
}
MYQUATERNION& MYQUATERNION::operator-= (const MYQUATERNION& q)
{
	s -= q.s; x -= q.x; y -= q.y; z -= q.z;
	return *this;
}
MYQUATERNION& MYQUATERNION::operator*= (const MYQUATERNION& q)
{
	VEC3 v1(x, y, z);
	VEC3 v2(q.x, q.y, q.z);
	s = s * q.s - dot(v1, v2);
	VEC3 v3 = v2 * s + v1 * q.s + cross(v1, v2);
	x = v3[0]; y = v3[1]; z = v3[2];
	return *this;
}
MYQUATERNION& MYQUATERNION::operator*= (float S)
{
	s *= S; x *= S; y *= S; z *= S;
	return *this;
}
MYQUATERNION& MYQUATERNION::operator/= (const MYQUATERNION& q)
{
	MYQUATERNION q2inverse;
	q.Inverse(q2inverse);
	*this = *this*q2inverse;
	return *this;
}
MYQUATERNION MYQUATERNION::operator+ () const { return *this; }
MYQUATERNION MYQUATERNION::operator- () const { return MYQUATERNION(-s, -x, -y, -z); }
float MYQUATERNION::Length() const
{
	return sqrtf(s*s + dot(VEC3(x, y, z), VEC3(x, y, z)));
}

float MYQUATERNION::LengthSquared() const
{
	return s * s + dot(VEC3(x, y, z), VEC3(x, y, z));
}

void MYQUATERNION::Normalize()
{
	float ss = 1.0f / sqrtf(s * s + x * x + y * y + z * z);
	x /= ss;
	y /= ss;
	z /= ss;
	s /= ss;
}

void MYQUATERNION::Normalize(MYQUATERNION & result) const
{
	result = normalize(VEC4(*this));
}

void MYQUATERNION::Rotate(VEC3 r)
{
	VEC3 Zaxis(0, 0, 1);
	Axis = cross(Zaxis, r);
	Angle = acosf(dot(r, Zaxis) / sqrtf(dot(r, r)));
	s = 0;
	VEC3 temp = r * cosf(Angle / 2) + Axis * (1.0f - cosf(Angle))*(dot(Axis, r)) + cross(Axis, r);
	x = temp[0];
	y = temp[1];
	z = temp[2];
}

void MYQUATERNION::ToMatrix(MAT4 & m)
{
	m[0][0] = 1.0f - 2.0f*(y*y + z * z); m[0][1] = 2.0f*(x*y - s * z); m[0][2] = 2.0f*(x*z + s * y);
	m[1][0] = 2.0f*(x*y + s * z); m[1][1] = 1.0f - 2.0f*(x*x + z * z); m[1][2] = 2.0f*(y*z - s * x);
	m[2][0] = 2.0f*(x*z - s * y); m[2][1] = 2.0f*(y*z + s * x); m[2][2] = 1.0f - 2.0f*(y*y + x * x);
}

MAT4 MYQUATERNION::ToMatrix()
{
	MAT4 m;
	m[0][0] = 1.0f - 2.0f*(y*y + z * z); m[0][1] = 2.0f*(x*y - s * z); m[0][2] = 2.0f*(x*z + s * y);
	m[1][0] = 2.0f*(x*y + s * z); m[1][1] = 1.0f - 2.0f*(x*x + z * z); m[1][2] = 2.0f*(y*z - s * x);
	m[2][0] = 2.0f*(x*z - s * y); m[2][1] = 2.0f*(y*z + s * x); m[2][2] = 1.0f - 2.0f*(y*y + x * x);
	return m;
}

void MYQUATERNION::Conjugate()
{
	x = -x; y = -y; z = -z;
}

void MYQUATERNION::Conjugate(MYQUATERNION & result) const
{
	result.s = s; result.x = -x; result.y = -y; result.z = -z;
}

void MYQUATERNION::Inverse(MYQUATERNION & result) const
{
	Conjugate(result);
	result*(1.0f / result.LengthSquared());
}

float MYQUATERNION::Dot(const MYQUATERNION & Q) const
{
	return x * Q.x + y * Q.y + z * Q.z + s * Q.s;
}

void MYQUATERNION::Slerp(float t, const MYQUATERNION & Q1, const MYQUATERNION & Q2, MYQUATERNION & result)
{
	MYQUATERNION q0 = Q1;
	MYQUATERNION q1 = Q2;
	float d = q0.Dot(q1);
	if (fabsf(d) > 0.9995f)
	{
		result = normalize(q0 + t * (q1 - q0));
	}
	if (d < 0)
	{
		q0 = -1 * q0; d = -d;
	}
	float a = acoshf(d);
	result = q0 * sinf((1 - t)*a) + q1 * sin(t*a) * (1.0f / sinf(a));
}

MYQUATERNION MYQUATERNION::Slerp(float t, const MYQUATERNION & Q1, const MYQUATERNION & Q2)
{
	MYQUATERNION q0 = Q1;
	MYQUATERNION q1 = Q2;
	float d = q0.Dot(q1);
	if (fabsf(d) > 0.9995f)
	{
		MYQUATERNION temp = q0 + t * (q1 - q0);
		temp.Normalize();
		return temp;
	}
	if (d < 0.0f)
	{
		q0 = -1 * q0; d = -d;
	}
	float a = acosf(d);
	return (q0 * sinf((1.0f - t)*a) + q1 * sin(t*a)) * (1.0f / sinf(a));
}
// Binary operators
MYQUATERNION operator+ (const MYQUATERNION& Q1, const MYQUATERNION& Q2)
{
	return MYQUATERNION(Q1.s + Q2.s, Q1.x + Q2.x, Q1.y + Q2.y, Q1.z + Q2.z);
}
MYQUATERNION operator- (const MYQUATERNION& Q1, const MYQUATERNION& Q2)
{
	return MYQUATERNION(Q1.s - Q2.s, Q1.x - Q2.x, Q1.y - Q2.y, Q1.z - Q2.z);
}
MYQUATERNION operator* (const MYQUATERNION& Q1, const MYQUATERNION& Q2)
{
	VEC3 v1(Q1.x, Q1.y, Q1.z);
	VEC3 v2(Q2.x, Q2.y, Q2.z);
	return MYQUATERNION(Q1.s*Q2.s - dot(v1, v2),
		v2*Q1.s + v1 * Q2.s + cross(v1, v2));
}
MYQUATERNION operator* (const MYQUATERNION& Q, float S)
{
	return MYQUATERNION(Q.s*S, Q.x*S, Q.y*S, Q.z*S);
}
MYQUATERNION operator/ (const MYQUATERNION& Q1, const MYQUATERNION& Q2)
{
	MYQUATERNION q2inverse;
	Q2.Inverse(q2inverse);
	return Q1 * q2inverse;
}
MYQUATERNION operator* (float S, const MYQUATERNION& Q)
{
	return MYQUATERNION(Q.s*S, Q.x*S, Q.y*S, Q.z*S);
}
