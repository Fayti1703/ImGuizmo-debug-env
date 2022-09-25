/*
 * Code taken from `https://github.com/CedricGuillemet/ImGuizmo`
 *
 * -- Original License --
 *
 * The MIT License(MIT)
 *
 * Copyright(c) 2021 Cedric Guillemet
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files(the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions :
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * -- Change Notes --
 * · Removed parts that were unused for this project.
 * · Made the matrix composition functions `extern "C"`
 *
 * Changes licensed under The MIT License, as reproduced above.
 * Copyright 2022 Fayti1703
 *
 */


#include "MatrixOps.h"

#include <cfloat>
#include <cstring>
#include <cmath>

static const float ZPI = 3.14159265358979323846f;
static const float RAD2DEG = (180.f / ZPI);
static const float DEG2RAD = (ZPI / 180.f);

void FPU_MatrixF_x_MatrixF(const float* a, const float* b, float* r)
{
	r[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
	r[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
	r[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
	r[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

	r[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
	r[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
	r[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
	r[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

	r[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
	r[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
	r[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
	r[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

	r[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
	r[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
	r[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
	r[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
}


struct matrix_t;

struct vec_t {
public:
	float x, y, z, w;

	void Set(float _x, float _y, float _z = 0.f, float _w = 0.f) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	vec_t& operator-=(const vec_t& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
		return *this;
	}

	vec_t& operator+=(const vec_t& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	vec_t& operator*=(const vec_t& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
		return *this;
	}

	vec_t& operator*=(float v) {
		x *= v;
		y *= v;
		z *= v;
		w *= v;
		return *this;
	}

	vec_t operator*(float f) const;
	vec_t operator-() const;
	vec_t operator-(const vec_t& v) const;
	vec_t operator+(const vec_t& v) const;
	vec_t operator*(const vec_t& v) const;

	const vec_t& operator+() const { return (*this); }

	float Length() const { return sqrtf(x * x + y * y + z * z); };

	float LengthSq() const { return (x * x + y * y + z * z); };

	vec_t Normalize() {
		(*this) *= (1.f / (Length() > FLT_EPSILON ? Length() : FLT_EPSILON));
		return (*this);
	}

	float& operator[](size_t index) { return ((float*) &x)[index]; }

	const float& operator[](size_t index) const { return ((float*) &x)[index]; }

	bool operator!=(const vec_t& other) const { return memcmp(this, &other, sizeof(vec_t)) != 0; }
};

vec_t makeVect(float _x, float _y, float _z = 0.f, float _w = 0.f) { vec_t res; res.x = _x; res.y = _y; res.z = _z; res.w = _w; return res; }
vec_t vec_t::operator * (float f) const { return makeVect(x * f, y * f, z * f, w * f); }
vec_t vec_t::operator - () const { return makeVect(-x, -y, -z, -w); }
vec_t vec_t::operator - (const vec_t& v) const { return makeVect(x - v.x, y - v.y, z - v.z, w - v.w); }
vec_t vec_t::operator + (const vec_t& v) const { return makeVect(x + v.x, y + v.y, z + v.z, w + v.w); }
vec_t vec_t::operator * (const vec_t& v) const { return makeVect(x * v.x, y * v.y, z * v.z, w * v.w); }

static const vec_t directionUnary[3] = { makeVect(1.f, 0.f, 0.f), makeVect(0.f, 1.f, 0.f), makeVect(0.f, 0.f, 1.f) };

struct matrix_t {
public:

	union {
		float m[4][4];
		float m16[16];
		struct {
			vec_t right, up, dir, position;
		} v;
		vec_t component[4];
	};

	operator float*() { return m16; }

	operator const float*() const { return m16; }

	matrix_t& operator*=(const matrix_t& mat) {
		matrix_t tmpMat;
		tmpMat = *this;
		tmpMat.Multiply(mat);
		*this = tmpMat;
		return *this;
	}

	matrix_t operator*(const matrix_t& mat) const {
		matrix_t matT;
		matT.Multiply(*this, mat);
		return matT;
	}

	void Multiply(const matrix_t& matrix) {
		matrix_t tmp;
		tmp = *this;

		FPU_MatrixF_x_MatrixF((float*) &tmp, (float*) &matrix, (float*) this);
	}

	void Multiply(const matrix_t& m1, const matrix_t& m2) {
		FPU_MatrixF_x_MatrixF((float*) &m1, (float*) &m2, (float*) this);
	}

	void SetToIdentity() {
		v.right.Set(1.f, 0.f, 0.f, 0.f);
		v.up.Set(0.f, 1.f, 0.f, 0.f);
		v.dir.Set(0.f, 0.f, 1.f, 0.f);
		v.position.Set(0.f, 0.f, 0.f, 1.f);
	}

	void RotationAxis(const vec_t& axis, float angle);

	void OrthoNormalize() {
		v.right.Normalize();
		v.up.Normalize();
		v.dir.Normalize();
	}
};

void matrix_t::RotationAxis(const vec_t& axis, float angle)
{
	float length2 = axis.LengthSq();
	if (length2 < FLT_EPSILON)
	{
		SetToIdentity();
		return;
	}

	vec_t n = axis * (1.f / sqrtf(length2));
	float s = sinf(angle);
	float c = cosf(angle);
	float k = 1.f - c;

	float xx = n.x * n.x * k + c;
	float yy = n.y * n.y * k + c;
	float zz = n.z * n.z * k + c;
	float xy = n.x * n.y * k;
	float yz = n.y * n.z * k;
	float zx = n.z * n.x * k;
	float xs = n.x * s;
	float ys = n.y * s;
	float zs = n.z * s;

	m[0][0] = xx;
	m[0][1] = xy + zs;
	m[0][2] = zx - ys;
	m[0][3] = 0.f;
	m[1][0] = xy - zs;
	m[1][1] = yy;
	m[1][2] = yz + xs;
	m[1][3] = 0.f;
	m[2][0] = zx + ys;
	m[2][1] = yz - xs;
	m[2][2] = zz;
	m[2][3] = 0.f;
	m[3][0] = 0.f;
	m[3][1] = 0.f;
	m[3][2] = 0.f;
	m[3][3] = 1.f;
}

extern "C" {

	void DecomposeMatrixToComponents(const float* matrix, float* translation, float* rotation, float* scale) {
		matrix_t mat = *(matrix_t*) matrix;

		scale[0] = mat.v.right.Length();
		scale[1] = mat.v.up.Length();
		scale[2] = mat.v.dir.Length();

		mat.OrthoNormalize();

		rotation[0] = RAD2DEG * atan2f(mat.m[1][2], mat.m[2][2]);
		rotation[1] = RAD2DEG * atan2f(-mat.m[0][2], sqrtf(mat.m[1][2] * mat.m[1][2] + mat.m[2][2] * mat.m[2][2]));
		rotation[2] = RAD2DEG * atan2f(mat.m[0][1], mat.m[0][0]);

		translation[0] = mat.v.position.x;
		translation[1] = mat.v.position.y;
		translation[2] = mat.v.position.z;
	}


	void RecomposeMatrixFromComponents(const float* translation, const float* rotation, const float* scale, float* matrix) {
		matrix_t& mat = *(matrix_t*) matrix;

		matrix_t rot[3];
		for(int i = 0; i < 3; i++) {
			rot[i].RotationAxis(directionUnary[i], rotation[i] * DEG2RAD);
		}

		mat = rot[0] * rot[1] * rot[2];

		float validScale[3];
		for(int i = 0; i < 3; i++) {
			if(fabsf(scale[i]) < FLT_EPSILON) {
				validScale[i] = 0.001f;
			} else {
				validScale[i] = scale[i];
			}
		}
		mat.v.right *= validScale[0];
		mat.v.up *= validScale[1];
		mat.v.dir *= validScale[2];
		mat.v.position.Set(translation[0], translation[1], translation[2], 1.f);
	}
}