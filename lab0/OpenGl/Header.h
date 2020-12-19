#pragma once
// From: https://gist.github.com/namandixit/f5818c5c5f4457b3ba3b6e055bff44c5

/* This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.
In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
For more information, please refer to <http://unlicense.org/>
*/

#pragma once

#include <math.h>

/* Magic Numbers, weild with caution */

#define QUAT_MAG_EPSILON 0.00001f


/* Constants */

# define M_E		2.7182818284590452354	/* e */
# define M_LOG2E	1.4426950408889634074	/* log_2 e */
# define M_LOG10E	0.43429448190325182765	/* log_10 e */
# define M_LN2		0.69314718055994530942	/* log_e 2 */
# define M_LN10		2.30258509299404568402	/* log_e 10 */
# define M_PI		3.14159265358979323846	/* pi */
# define M_PI_2		1.57079632679489661923	/* pi/2 */
# define M_PI_4		0.78539816339744830962	/* pi/4 */
# define M_1_PI		0.31830988618379067154	/* 1/pi */
# define M_2_PI		0.63661977236758134308	/* 2/pi */
# define M_2_SQRTPI	1.12837916709551257390	/* 2/sqrt(pi) */
# define M_SQRT2	1.41421356237309504880	/* sqrt(2) */
# define M_SQRT1_2	0.70710678118654752440	/* 1/sqrt(2) */

/* Data Structures */

typedef struct {
	float x, y, z, _pad;
} Vector;

typedef struct {
	float r, i, j, k;
} Quaternion;

typedef union {
	float elem[16];
	struct {
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};
} Matrix;

/* Utility */

static inline float
radians(float degree)
{
	return degree * M_PI / 180.0;
}

static inline float
degrees(float radian)
{
	return radian * 180.0 / M_PI;
}

/* Vectors */

static inline Vector
vecNew(float x, float y, float z)
{
	return (Vector) { x, y, z, 1 };
}

static inline Vector
vecInvert(Vector v)
{
	return vecNew(-v.x, -v.y, -v.z);
}

static inline Vector
vecAdd(Vector v, Vector u)
{
	return vecNew(v.x + u.x,
		v.y + u.y,
		v.z + u.z);
}

static inline Vector
vecSub(Vector v, Vector u)
{
	return vecNew(v.x - u.x,
		v.y - u.y,
		v.z - u.z);
}

static inline Vector
vecMul(Vector v, Vector u)
{
	return vecNew(v.x * u.x,
		v.y * u.y,
		v.z * u.z);
}

static inline Vector
vecScale(Vector v, float r)
{
	return vecNew(v.x * r,
		v.y * r,
		v.z * r);
}

static inline float
vecDot(Vector v, Vector u)
{
	return ((v.x * u.x) +
		(v.y * u.y) +
		(v.z * u.z));
}

static inline Vector
vecCross(Vector v, Vector u)
{
	return vecNew((v.y * u.z) - (v.z * u.y),
		(v.z * u.x) - (v.x * u.z),
		(v.x * u.y) - (v.y * u.x));
}


static inline Vector
vecLerp(Vector v, Vector u, float factor)
{
	return vecNew(v.x + factor * (u.x - v.x),
		v.y + factor * (u.y - v.y),
		v.z + factor * (u.z - v.z));
}

static inline float
vecMag(Vector v)
{
	return sqrtf((v.x * v.x) +
		(v.y * v.y) +
		(v.z * v.z));
}

static inline float
vecMagSq(Vector v)
{
	return ((v.x * v.x) +
		(v.y * v.y) +
		(v.z * v.z));
}

static inline Vector
vecNorm(Vector v)
{
	float mag = vecMag(v);
	if (mag > 0) {
		return vecNew(v.x / mag, v.y / mag, v.z / mag);
	}
	else {
		return vecNew(0, 0, 0);
	}
}

/* Quaternion */

static inline Quaternion
quatNew(float a, float b, float c, float d)
{
	Quaternion q;

	q.r = a;
	q.i = b;
	q.j = c;
	q.k = d;

	return q;
}

static inline Quaternion
quatMul(Quaternion q, Quaternion p)
{
	return quatNew((q.r * p.r) - (q.i * p.i) - (q.j * p.j) - (q.k * p.k),
		(q.r * p.i) + (q.i * p.r) + (q.j * p.k) - (q.k * p.j),
		(q.r * p.j) + (q.j * p.r) + (q.k * p.i) - (q.i * p.k),
		(q.r * p.k) + (q.k * p.r) + (q.i * p.j) - (q.j * p.i));
}

static inline float
quatMagSq(Quaternion q)
{
	return ((q.r * q.r) +
		(q.i * q.i) +
		(q.j * q.j) +
		(q.k * q.k));
}

static inline float
quatMag(Quaternion q)
{
	return sqrtf(quatMagSq(q));
}

static inline Quaternion
quatNormalize(Quaternion q)
{
	float len_inv = 1.0f / quatMag(q);

	Quaternion p;

	if (len_inv <= QUAT_MAG_EPSILON) {
		p.r = 1;
		p.i = 0;
		p.j = 0;
		p.k = 0;

		return p;
	}

	p.r = q.r * len_inv;
	p.i = q.i * len_inv;
	p.j = q.j * len_inv;
	p.k = q.k * len_inv;

	return p;
}

/* Matrix */

/* WARNING: DO NOT modify this code to accept or return pointers
 * as arguements or return values, respectively. Since this code will
 * most probably be inlined, using pointers will stop optimizer's
 * data flow analysis.
 */

static inline Matrix
matNew(void)
{
	Matrix m;

	m.elem[0] = 1;
	m.elem[1] = 0;
	m.elem[2] = 0;
	m.elem[3] = 0;

	m.elem[4] = 0;
	m.elem[5] = 1;
	m.elem[6] = 0;
	m.elem[7] = 0;

	m.elem[8] = 0;
	m.elem[9] = 0;
	m.elem[10] = 1;
	m.elem[11] = 0;

	m.elem[12] = 0;
	m.elem[13] = 0;
	m.elem[14] = 0;
	m.elem[15] = 1;

	return m;
}

static inline Matrix
matAdd(Matrix m, Matrix n)
{
	Matrix o;

	for (int i = 0; i < 16; ++i) {
		o.elem[i] = m.elem[i] + n.elem[i];
	}

	return o;
}

static inline Matrix
matSub(Matrix m, Matrix n)
{
	Matrix o;

	for (int i = 0; i < 16; ++i) {
		o.elem[i] = m.elem[i] + n.elem[i];
	}

	return o;
}

static inline Matrix
matMul(Matrix m, Matrix n)
{
	Matrix o;

	for (int i = 0; i < 16; i = i + 4) {
		for (int j = 0; j < 16; j = j + 4) {
			int index = i / 4;
			o.elem[index + j] = ((m.elem[index] * n.elem[j]) +
				(m.elem[index + 4] * n.elem[j + 1]) +
				(m.elem[index + 8] * n.elem[j + 2]) +
				(m.elem[index + 12] * n.elem[j + 3]));
		}
	}

	return o;
}

static inline Matrix
matMulR(Matrix m, float r)
{
	Matrix n;

	for (int i = 0; i < 16; ++i) {
		n.elem[i] = m.elem[i] * r;
	}

	return n;
}

static inline float
matDeterminant(Matrix m)
{
	float a = (m.elem[6] * m.elem[11]) - (m.elem[10] * m.elem[7]);
	float b = (m.elem[10] * m.elem[15]) - (m.elem[14] * m.elem[11]);
	float c = (m.elem[6] * m.elem[15]) - (m.elem[14] * m.elem[7]);
	float d = (m.elem[2] * m.elem[11]) - (m.elem[10] * m.elem[3]);
	float e = (m.elem[2] * m.elem[15]) - (m.elem[14] * m.elem[3]);
	float f = (m.elem[2] * m.elem[7]) - (m.elem[6] * m.elem[3]);


	float aa = (m.elem[5] * b) - (m.elem[9] * c) + (m.elem[13] * a);
	float bb = (m.elem[1] * b) - (m.elem[9] * e) + (m.elem[13] * d);
	float cc = (m.elem[1] * c) - (m.elem[5] * e) + (m.elem[13] * f);
	float dd = (m.elem[1] * a) - (m.elem[5] * d) + (m.elem[9] * f);

	float det = ((m.elem[0] * aa) - (m.elem[4] * bb) +
		(m.elem[8] * cc) - (m.elem[12] * dd));

	return det;
}

static inline Matrix
matInverse(Matrix m)
{
	Matrix n;

	n.elem[0] = ((m.elem[5] * m.elem[10] * m.elem[15]) -
		(m.elem[5] * m.elem[11] * m.elem[14]) -
		(m.elem[9] * m.elem[6] * m.elem[15]) +
		(m.elem[9] * m.elem[7] * m.elem[14]) +
		(m.elem[13] * m.elem[6] * m.elem[11]) -
		(m.elem[13] * m.elem[7] * m.elem[10]));

	n.elem[1] = ((-m.elem[4] * m.elem[10] * m.elem[15]) +
		(m.elem[4] * m.elem[11] * m.elem[14]) +
		(m.elem[8] * m.elem[6] * m.elem[15]) -
		(m.elem[8] * m.elem[7] * m.elem[14]) -
		(m.elem[12] * m.elem[6] * m.elem[11]) +
		(m.elem[12] * m.elem[7] * m.elem[10]));

	n.elem[2] = ((m.elem[4] * m.elem[9] * m.elem[15]) -
		(m.elem[4] * m.elem[11] * m.elem[13]) -
		(m.elem[8] * m.elem[5] * m.elem[15]) +
		(m.elem[8] * m.elem[7] * m.elem[13]) +
		(m.elem[12] * m.elem[5] * m.elem[11]) -
		(m.elem[12] * m.elem[7] * m.elem[9]));

	n.elem[3] = ((-m.elem[4] * m.elem[9] * m.elem[14]) +
		(m.elem[4] * m.elem[10] * m.elem[13]) +
		(m.elem[8] * m.elem[5] * m.elem[14]) -
		(m.elem[8] * m.elem[6] * m.elem[13]) -
		(m.elem[12] * m.elem[5] * m.elem[10]) +
		(m.elem[12] * m.elem[6] * m.elem[9]));

	n.elem[4] = ((-m.elem[1] * m.elem[10] * m.elem[15]) +
		(m.elem[1] * m.elem[11] * m.elem[14]) +
		(m.elem[9] * m.elem[2] * m.elem[15]) -
		(m.elem[9] * m.elem[3] * m.elem[14]) -
		(m.elem[13] * m.elem[2] * m.elem[11]) +
		(m.elem[13] * m.elem[3] * m.elem[10]));

	n.elem[5] = ((m.elem[0] * m.elem[10] * m.elem[15]) -
		(m.elem[0] * m.elem[11] * m.elem[14]) -
		(m.elem[8] * m.elem[2] * m.elem[15]) +
		(m.elem[8] * m.elem[3] * m.elem[14]) +
		(m.elem[12] * m.elem[2] * m.elem[11]) -
		(m.elem[12] * m.elem[3] * m.elem[10]));

	n.elem[6] = ((-m.elem[0] * m.elem[9] * m.elem[15]) +
		(m.elem[0] * m.elem[11] * m.elem[13]) +
		(m.elem[8] * m.elem[1] * m.elem[15]) -
		(m.elem[8] * m.elem[3] * m.elem[13]) -
		(m.elem[12] * m.elem[1] * m.elem[11]) +
		(m.elem[12] * m.elem[3] * m.elem[9]));

	n.elem[7] = ((m.elem[0] * m.elem[9] * m.elem[14]) -
		(m.elem[0] * m.elem[10] * m.elem[13]) -
		(m.elem[8] * m.elem[1] * m.elem[14]) +
		(m.elem[8] * m.elem[2] * m.elem[13]) +
		(m.elem[12] * m.elem[1] * m.elem[10]) -
		(m.elem[12] * m.elem[2] * m.elem[9]));

	n.elem[8] = ((m.elem[1] * m.elem[6] * m.elem[15]) -
		(m.elem[1] * m.elem[7] * m.elem[14]) -
		(m.elem[5] * m.elem[2] * m.elem[15]) +
		(m.elem[5] * m.elem[3] * m.elem[14]) +
		(m.elem[13] * m.elem[2] * m.elem[7]) -
		(m.elem[13] * m.elem[3] * m.elem[6]));

	n.elem[9] = ((-m.elem[0] * m.elem[6] * m.elem[15]) +
		(m.elem[0] * m.elem[7] * m.elem[14]) +
		(m.elem[4] * m.elem[2] * m.elem[15]) -
		(m.elem[4] * m.elem[3] * m.elem[14]) -
		(m.elem[12] * m.elem[2] * m.elem[7]) +
		(m.elem[12] * m.elem[3] * m.elem[6]));

	n.elem[10] = ((m.elem[0] * m.elem[5] * m.elem[15]) -
		(m.elem[0] * m.elem[7] * m.elem[13]) -
		(m.elem[4] * m.elem[1] * m.elem[15]) +
		(m.elem[4] * m.elem[3] * m.elem[13]) +
		(m.elem[12] * m.elem[1] * m.elem[7]) -
		(m.elem[12] * m.elem[3] * m.elem[5]));

	n.elem[11] = ((-m.elem[0] * m.elem[5] * m.elem[14]) +
		(m.elem[0] * m.elem[6] * m.elem[13]) +
		(m.elem[4] * m.elem[1] * m.elem[14]) -
		(m.elem[4] * m.elem[2] * m.elem[13]) -
		(m.elem[12] * m.elem[1] * m.elem[6]) +
		(m.elem[12] * m.elem[2] * m.elem[5]));

	n.elem[12] = ((-m.elem[1] * m.elem[6] * m.elem[11]) +
		(m.elem[1] * m.elem[7] * m.elem[10]) +
		(m.elem[5] * m.elem[2] * m.elem[11]) -
		(m.elem[5] * m.elem[3] * m.elem[10]) -
		(m.elem[9] * m.elem[2] * m.elem[7]) +
		(m.elem[9] * m.elem[3] * m.elem[6]));

	n.elem[13] = ((m.elem[0] * m.elem[6] * m.elem[11]) -
		(m.elem[0] * m.elem[7] * m.elem[10]) -
		(m.elem[4] * m.elem[2] * m.elem[11]) +
		(m.elem[4] * m.elem[3] * m.elem[10]) +
		(m.elem[8] * m.elem[2] * m.elem[7]) -
		(m.elem[8] * m.elem[3] * m.elem[6]));

	n.elem[14] = ((-m.elem[0] * m.elem[5] * m.elem[11]) +
		(m.elem[0] * m.elem[7] * m.elem[9]) +
		(m.elem[4] * m.elem[1] * m.elem[11]) -
		(m.elem[4] * m.elem[3] * m.elem[9]) -
		(m.elem[8] * m.elem[1] * m.elem[7]) +
		(m.elem[8] * m.elem[3] * m.elem[5]));

	n.elem[15] = ((m.elem[0] * m.elem[5] * m.elem[10]) -
		(m.elem[0] * m.elem[6] * m.elem[9]) -
		(m.elem[4] * m.elem[1] * m.elem[10]) +
		(m.elem[4] * m.elem[2] * m.elem[9]) +
		(m.elem[8] * m.elem[1] * m.elem[6]) -
		(m.elem[8] * m.elem[2] * m.elem[5]));

	float det = (m.elem[0] * n.elem[0] +
		m.elem[1] * n.elem[1] +
		m.elem[2] * n.elem[2] +
		m.elem[3] * n.elem[3]);

	det = 1.0 / det;

	return matMulR(n, det);
}

static inline Matrix
matTranslate(Vector v)
{
	Matrix m;

	m.elem[0] = 1;
	m.elem[1] = 0;
	m.elem[2] = 0;
	m.elem[3] = 0;

	m.elem[4] = 0;
	m.elem[5] = 1;
	m.elem[6] = 0;
	m.elem[7] = 0;

	m.elem[8] = 0;
	m.elem[9] = 0;
	m.elem[10] = 1;
	m.elem[11] = 0;

	m.elem[12] = v.x;
	m.elem[13] = v.y;
	m.elem[14] = v.z;
	m.elem[15] = 1;

	return m;
}

static inline Matrix
matRotate(Quaternion q)
{
	Matrix m;

	m.elem[0] = 1 - (2 * q.j * q.j) - (2 * q.k * q.k);
	m.elem[1] = (2 * q.i * q.j) + (2 * q.k * q.r);
	m.elem[2] = (2 * q.i * q.k) - (2 * q.j * q.r);
	m.elem[3] = 0;

	m.elem[4] = (2 * q.i * q.j) - (2 * q.k * q.r);
	m.elem[5] = 1 - (2 * q.i * q.i) - (2 * q.k * q.k);
	m.elem[6] = (2 * q.j * q.k) + (2 * q.i * q.r);
	m.elem[7] = 0;

	m.elem[8] = (2 * q.i * q.k) + (2 * q.j * q.r);
	m.elem[9] = (2 * q.j * q.k) - (2 * q.i * q.r);
	m.elem[10] = 1 - (2 * q.i * q.i) - (2 * q.j * q.j);
	m.elem[11] = 0;

	m.elem[12] = 0;
	m.elem[13] = 0;
	m.elem[14] = 0;
	m.elem[15] = 1;

	return m;
}

static inline Matrix
matScale(Vector v)
{
	Matrix m;

	m.elem[0] = v.x;
	m.elem[1] = 0;
	m.elem[2] = 0;
	m.elem[3] = 0;

	m.elem[4] = 0;
	m.elem[5] = v.y;
	m.elem[6] = 0;
	m.elem[7] = 0;

	m.elem[8] = 0;
	m.elem[9] = 0;
	m.elem[10] = v.z;
	m.elem[11] = 0;

	m.elem[12] = 0;
	m.elem[13] = 0;
	m.elem[14] = 0;
	m.elem[15] = 1;

	return m;
}

static inline Matrix
matLookAt(Vector eye, Vector lookat, Vector world_up)
{
	Vector direction = vecSub(lookat, eye);
	direction = vecNorm(direction);

	Vector right = vecCross(direction, world_up);
	right = vecNorm(right);

	Vector up = vecCross(right, direction);

	Matrix m;

	m.elem[0] = right.x;
	m.elem[1] = up.x;
	m.elem[2] = -direction.x;
	m.elem[3] = 0.0;

	m.elem[4] = right.y;
	m.elem[5] = up.y;
	m.elem[6] = -direction.y;
	m.elem[7] = 0.0;

	m.elem[8] = right.z;
	m.elem[9] = up.z;
	m.elem[10] = -direction.z;
	m.elem[11] = 0.0;

	m.elem[12] = 0.0;
	m.elem[13] = 0.0;
	m.elem[14] = 0.0;
	m.elem[15] = 1.0;

	Matrix n = matTranslate(vecInvert(eye));

	return matMul(m, n);
}

static inline Matrix
matPerspective(float y_fov, float aspect, float near_plane, float far_plane)
{
	float a = 1.0 / tan(y_fov / 2.0);

	float plane_add = far_plane + near_plane;
	float plane_sub = far_plane - near_plane;

	Matrix m;

	m.elem[0] = a / aspect;
	m.elem[1] = 0.0;
	m.elem[2] = 0.0;
	m.elem[3] = 0.0;

	m.elem[4] = 0.0;
	m.elem[5] = a;
	m.elem[6] = 0.0;
	m.elem[7] = 0.0;

	m.elem[8] = 0.0;
	m.elem[9] = 0.0;
	m.elem[10] = -plane_add / plane_sub;
	m.elem[11] = -1.0;

	m.elem[12] = 0.0;
	m.elem[13] = 0.0;
	m.elem[14] = -((2.0 * far_plane * near_plane) / plane_sub);
	m.elem[15] = 0.0;

	return m;
}