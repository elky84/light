#include "light.h"
#include "Matrix.hpp"

#include <cmath>
#include <memory.h>

namespace light
{

Matrix3::Matrix3()
{
	// 두 구현과 동일한 의미를 가진다
	::memset(element, '\0', sizeof(element));
}

Matrix3::Matrix3(const float rhs[MATRIX3_ELEMENT * MATRIX3_ELEMENT])
{
	// 두 구현과 동일한 의미를 가진다
	::memcpy(element, rhs, sizeof(element));
}

Matrix3::Matrix3(const Matrix3& rhs)
{
	// 두 구현과 동일한 의미를 가진다
	::memcpy(element, rhs.element, sizeof(element));
}

Matrix3& Matrix3::operator=(const Matrix3& rhs)
{
	if (this == &rhs) return *this;
	::memcpy(element, rhs.element, sizeof(element));
	return *this;
}

void Matrix3::Identity()
{
	//  단위행렬
	element[0] = 1.0;
	element[1] = 0.0;
	element[2] = 0.0;

	element[3] = 0.0;
	element[4] = 1.0;
	element[5] = 0.0;

	element[6] = 0.0;
	element[7] = 0.0;
	element[8] = 1.0;
}

void Matrix3::Transpose()
{
	//  전치행렬
	Matrix3 mat(*this);
	for (int i = 0; i < MATRIX3_ELEMENT; ++i)
	{
		for (int j = 0; j < MATRIX3_ELEMENT; ++j)
		{
			element[j*MATRIX3_ELEMENT + i] = mat.element[i*MATRIX3_ELEMENT + j];
		}
	}
}

Matrix3& Matrix3::operator+()
{ 
	return *this;
}

Matrix3 Matrix3::operator-() const
{
	Matrix3 mat(*this);
	mat.Scalar(-1.0);
	return mat;
}

Matrix3 Matrix3::operator+(const Matrix3& rhs) const
{
	// 행렬의 덧셈
	Matrix3 mat(*this);
	for (int i = 0; i < MATRIX3_ELEMENT * MATRIX3_ELEMENT; ++i)
	{
		mat.element[i] += rhs.element[i];
	}
	return mat;
}

Matrix3& Matrix3::operator+=(const Matrix3& rhs)
{
	// 행렬의 덧셈
	for (int i = 0; i < MATRIX3_ELEMENT * MATRIX3_ELEMENT; ++i)
	{
		element[i] += rhs.element[i];
	}
	return *this;
}

Matrix3 Matrix3::operator-(const Matrix3& rhs) const
{
	//  행렬의 뺄셈
	Matrix3 mat(*this);
	for (int i = 0; i < MATRIX3_ELEMENT * MATRIX3_ELEMENT; ++i)
	{
		mat.element[i] -= rhs.element[i];
	}
	return mat;
}

Matrix3& Matrix3::operator-=(const Matrix3& rhs)
{
	//  행렬의 뺄셈
	for (int i = 0; i < MATRIX3_ELEMENT * MATRIX3_ELEMENT; ++i)
	{
		element[i] -= rhs.element[i];
	}
	return *this;
}

Matrix3 Matrix3::operator*(const Matrix3& rhs) const
{
	//  행렬의 곱셈
	Matrix3 mat;
	for (int i = 0; i < MATRIX3_ELEMENT; ++i)
	{
		for (int j = 0; j < MATRIX3_ELEMENT; ++j)
		{
			for (int k = 0; k < MATRIX3_ELEMENT; ++k)
			{
				mat.element[j*MATRIX3_ELEMENT + i] += element[k*MATRIX3_ELEMENT + i] * rhs.element[j*MATRIX3_ELEMENT + k];
			}
		}
	}
	return mat;
}

Matrix3& Matrix3::operator*=(const Matrix3& rhs)
{
	return (*this = *this * rhs); 	//  행렬의 곱셈
}

void Matrix3::Scalar(float k)
{
	// 행렬과 스칼라의 곱
	for (int i = 0; i < MATRIX3_ELEMENT * MATRIX3_ELEMENT; ++i)
	{
		element[i] *= k;
	}
}

float Matrix3::Det() const
{
	// 3차 행렬식
	float det = 0.0;
	det = element[0] * element[4] * element[8] + element[3] * element[7] * element[2] + element[6] * element[1] * element[5];
	det -= element[0] * element[7] * element[5] + element[3] * element[1] * element[8] + element[6] * element[4] * element[2];
	return det;
}

Matrix3 Matrix3::Inverse() const
{
	// 역행렬
	float det = Det();

	//  행렬식이 0이면 역행렬은 존재하진 않는다(특이행렬인 경우에 해당)
	if (fabs(det) < 1.0e-5)
		return Matrix3();

	Matrix3 adjoint(Adjoint());
	adjoint.Scalar(1.0f / det);
	return adjoint;
}

Matrix3 Matrix3::Adjoint() const
{
	// 수반행렬
	Matrix3 adjoint;
	adjoint.element[0] = element[4] * element[8] - element[7] * element[5];
	adjoint.element[1] = -element[1] * element[8] + element[7] * element[2];
	adjoint.element[2] = element[1] * element[5] - element[4] * element[2];

	adjoint.element[3] = -element[3] * element[8] + element[6] * element[5];
	adjoint.element[4] = element[0] * element[8] - element[6] * element[2];
	adjoint.element[5] = -element[0] * element[5] + element[3] * element[2];

	adjoint.element[6] = element[3] * element[7] - element[6] * element[4];
	adjoint.element[7] = -element[0] * element[7] + element[6] * element[1];
	adjoint.element[8] = element[0] * element[4] - element[3] * element[1];
	return adjoint;
}

Matrix4::Matrix4()
{
	::memset(element, 0, sizeof(element));
}

Matrix4::Matrix4(const float rhs[MATRIX4_ELEMENT * MATRIX4_ELEMENT])
{
	::memcpy(element, rhs, sizeof(element));
}

Matrix4::Matrix4( float e00, float e01, float e02, float e03,
					float e10, float e11, float e12, float e13,
					float e20, float e21, float e22, float e23,
					float e30, float e31, float e32, float e33 )
{
	_element[0][0]=e00; _element[0][1]=e01; _element[0][2]=e02; _element[0][3]=e03;
	_element[1][0]=e10; _element[1][1]=e11; _element[1][2]=e12; _element[1][3]=e13;
	_element[2][0]=e20; _element[2][1]=e21; _element[2][2]=e22; _element[2][3]=e23;
	_element[3][0]=e30; _element[3][1]=e31; _element[3][2]=e32; _element[3][3]=e33;
}


Matrix4::Matrix4(const Matrix4& rhs)
{
	// 두 구현과 동일한 의미를 가진다
	::memcpy(element, rhs.element, sizeof(element));
}

Matrix4& Matrix4::operator=(const Matrix4& rhs)
{
	if (this == &rhs) return *this;
	::memcpy(element, rhs.element, sizeof(element));
	return *this;
}

void Matrix4::Identity()
{
	// 단위행렬
	element[0] = 1.0f;
	element[1] = 0.0f;
	element[2] = 0.0f;
	element[3] = 0.0f;

	element[4] = 0.0f;
	element[5] = 1.0f;
	element[6] = 0.0f;
	element[7] = 0.0f;

	element[8] = 0.0f;
	element[9] = 0.0f;
	element[10] = 1.0f;
	element[11] = 0.0f;

	element[12] = 0.0f;
	element[13] = 0.0f;
	element[14] = 0.0f;
	element[15] = 1.0f;
}

void Matrix4::Transpose()
{
	//  전치행렬
	Matrix4 mat(*this);
	for (int i = 0; i < MATRIX4_ELEMENT; ++i)
	{
		for (int j = 0; j < MATRIX4_ELEMENT; ++j)
		{
			element[j*MATRIX4_ELEMENT + i] = mat.element[i*MATRIX4_ELEMENT + j];
		}
	}
}

Matrix4& Matrix4::operator+()
{ 
	return *this;  
}

Matrix4 Matrix4::operator-() const
{
	Matrix4 mat(*this);
	mat.Scalar(-1.0);
	return mat;
}

Matrix4 Matrix4::operator+(const Matrix4& rhs) const
{
	// 행렬의 덧셈
	Matrix4 mat(*this);
	for (int i = 0; i < MATRIX4_ELEMENT * MATRIX4_ELEMENT; ++i)
	{
		mat.element[i] += rhs.element[i];
	}
	return mat;
}

Matrix4& Matrix4::operator+=(const Matrix4& rhs)
{
	// 행렬의 덧셈
	for (int i = 0; i < MATRIX4_ELEMENT * MATRIX4_ELEMENT; ++i)
	{
		element[i] += rhs.element[i];
	}
	return *this;
}

Matrix4 Matrix4::operator-(const Matrix4& rhs) const
{
	//  행렬의 뺄셈
	Matrix4 mat(*this);
	for (int i = 0; i < MATRIX4_ELEMENT * MATRIX4_ELEMENT; ++i)
	{
		mat.element[i] -= rhs.element[i];
	}
	return mat;
}

Matrix4& Matrix4::operator-=(const Matrix4& rhs)
{
	//  행렬의 뺄셈
	for (int i = 0; i < MATRIX4_ELEMENT * MATRIX4_ELEMENT; ++i)
	{
		element[i] -= rhs.element[i];
	}
	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& rhs) const
{
	//  행렬의 곱셈
	Matrix4 mat;
	for (int i = 0; i < MATRIX4_ELEMENT; ++i)
	{
		for (int j = 0; j < MATRIX4_ELEMENT; ++j)
		{
			for (int k = 0; k < MATRIX4_ELEMENT; ++k)
			{
				mat.element[j*MATRIX4_ELEMENT + i] += element[k*MATRIX4_ELEMENT + i] * rhs.element[j*MATRIX4_ELEMENT + k];
			}
		}
	}
	return mat;
}

Matrix4& Matrix4::operator*=(const Matrix4& rhs)
{
	return (*this = *this * rhs); 	//  행렬의 곱셈
}

void Matrix4::Scalar(float f)
{
	// 행렬과 스칼라의 곱
	for (int i = 0; i < MATRIX4_ELEMENT * MATRIX4_ELEMENT; ++i)
	{
		element[i] *= f;
	}
}

float Matrix4::Det() const
{
	return (element[ 0]*element[ 5] - element[ 1]*element[ 4]) * 
		(element[10]*element[15] - element[11]*element[14]) - 
		(element[ 0]*element[ 6] - element[ 2]*element[ 4]) * 
		(element[ 9]*element[15] - element[11]*element[13]) + 
		(element[ 0]*element[ 7] - element[ 3]*element[ 4]) *
		(element[ 9]*element[14] - element[10]*element[13]) + 
		(element[ 1]*element[ 6] - element[ 2]*element[ 5]) *
		(element[ 8]*element[15] - element[11]*element[12]) -
		(element[ 1]*element[ 7] - element[ 3]*element[ 5]) *
		(element[ 8]*element[14] - element[10]*element[12]) +
		(element[ 2]*element[ 7] - element[ 3]*element[ 6]) *
		(element[ 8]*element[13] - element[ 9]*element[12]);
}

Matrix4 Matrix4::Inverse () const
{
	float fDet = Det();
	if (abs(fDet) <= 0.0)
		return Matrix4();

	float fA0 = element[ 0]*element[ 5] - element[ 1]*element[ 4];
	float fA1 = element[ 0]*element[ 6] - element[ 2]*element[ 4];
	float fA2 = element[ 0]*element[ 7] - element[ 3]*element[ 4];
	float fA3 = element[ 1]*element[ 6] - element[ 2]*element[ 5];
	float fA4 = element[ 1]*element[ 7] - element[ 3]*element[ 5];
	float fA5 = element[ 2]*element[ 7] - element[ 3]*element[ 6];
	float fB0 = element[ 8]*element[13] - element[ 9]*element[12];
	float fB1 = element[ 8]*element[14] - element[10]*element[12];
	float fB2 = element[ 8]*element[15] - element[11]*element[12];
	float fB3 = element[ 9]*element[14] - element[10]*element[13];
	float fB4 = element[ 9]*element[15] - element[11]*element[13];
	float fB5 = element[10]*element[15] - element[11]*element[14];

	Matrix4 kInv;
	kInv.element[ 0] =
		+ element[ 5]*fB5 - element[ 6]*fB4 + element[ 7]*fB3;
	kInv.element[ 4] =
		- element[ 4]*fB5 + element[ 6]*fB2 - element[ 7]*fB1;
	kInv.element[ 8] =
		+ element[ 4]*fB4 - element[ 5]*fB2 + element[ 7]*fB0;
	kInv.element[12] =
		- element[ 4]*fB3 + element[ 5]*fB1 - element[ 6]*fB0;
	kInv.element[ 1] =
		- element[ 1]*fB5 + element[ 2]*fB4 - element[ 3]*fB3;
	kInv.element[ 5] =
		+ element[ 0]*fB5 - element[ 2]*fB2 + element[ 3]*fB1;
	kInv.element[ 9] =
		- element[ 0]*fB4 + element[ 1]*fB2 - element[ 3]*fB0;
	kInv.element[13] =
		+ element[ 0]*fB3 - element[ 1]*fB1 + element[ 2]*fB0;
	kInv.element[ 2] =
		+ element[13]*fA5 - element[14]*fA4 + element[15]*fA3;
	kInv.element[ 6] =
		- element[12]*fA5 + element[14]*fA2 - element[15]*fA1;
	kInv.element[10] =
		+ element[12]*fA4 - element[13]*fA2 + element[15]*fA0;
	kInv.element[14] =
		- element[12]*fA3 + element[13]*fA1 - element[14]*fA0;
	kInv.element[ 3] =
		- element[ 9]*fA5 + element[10]*fA4 - element[11]*fA3;
	kInv.element[ 7] =
		+ element[ 8]*fA5 - element[10]*fA2 + element[11]*fA1;
	kInv.element[11] =
		- element[ 8]*fA4 + element[ 9]*fA2 - element[11]*fA0;
	kInv.element[15] =
		+ element[ 8]*fA3 - element[ 9]*fA1 + element[10]*fA0;

	float fInvDet = 1.0f / fDet;
	kInv.element[ 0] *= fInvDet;
	kInv.element[ 1] *= fInvDet;
	kInv.element[ 2] *= fInvDet;
	kInv.element[ 3] *= fInvDet;
	kInv.element[ 4] *= fInvDet;
	kInv.element[ 5] *= fInvDet;
	kInv.element[ 6] *= fInvDet;
	kInv.element[ 7] *= fInvDet;
	kInv.element[ 8] *= fInvDet;
	kInv.element[ 9] *= fInvDet;
	kInv.element[10] *= fInvDet;
	kInv.element[11] *= fInvDet;
	kInv.element[12] *= fInvDet;
	kInv.element[13] *= fInvDet;
	kInv.element[14] *= fInvDet;
	kInv.element[15] *= fInvDet;
	return kInv;
}

} //namespace light
