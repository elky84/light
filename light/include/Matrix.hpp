#pragma once

namespace light
{

const int MATRIX3_ELEMENT = 3;

class Matrix3 
{
public:
	Matrix3();

	Matrix3(const float rhs[MATRIX3_ELEMENT * MATRIX3_ELEMENT]);

	Matrix3(const Matrix3& rhs);

	Matrix3& operator=(const Matrix3& rhs);
	~Matrix3()  {}

	void Identity();
	void Transpose();

	Matrix3& operator+();

	Matrix3 operator-() const;

	Matrix3 operator+(const Matrix3& rhs) const;
	Matrix3& operator+=(const Matrix3& rhs);

	Matrix3 operator-(const Matrix3& rhs) const;
	Matrix3& operator-=(const Matrix3& rhs);
	Matrix3 operator*(const Matrix3& rhs) const;
	Matrix3& operator*=(const Matrix3& rhs);

	void Scalar(float k);

	float Det() const;
	Matrix3 Inverse() const;

protected:
	Matrix3 Adjoint() const;

public:
	union
	{
		float _element[MATRIX3_ELEMENT][MATRIX3_ELEMENT];
		float element[MATRIX3_ELEMENT * MATRIX3_ELEMENT];
	};

	//  우리가 사용하는 배열, element[9]의 행렬에서의 위치는 아래와 같다
	//  [  e0  e3  e6  ]
	//  [  e1  e4  e7  ]
	//  [  e2  e5  e8  ]
};

const int MATRIX4_ELEMENT = 4;

class Matrix4 
{
public:
	//  행렬의 생성자
	Matrix4();

	Matrix4(const float rhs[MATRIX4_ELEMENT * MATRIX4_ELEMENT]);

	Matrix4(float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33 );

	Matrix4(const Matrix4& rhs);

	Matrix4& operator=(const Matrix4& rhs);
	~Matrix4(){}

	void Identity();
	void Transpose();

	Matrix4& operator+();

	Matrix4 operator-() const;

	Matrix4 operator+(const Matrix4& rhs) const;
	Matrix4& operator+=(const Matrix4& rhs); 

	Matrix4 operator-(const Matrix4& rhs) const;
	Matrix4& operator-=(const Matrix4& rhs);
	Matrix4 operator*(const Matrix4& rhs) const;
	Matrix4& operator*=(const Matrix4& rhs);

	void Scalar(float f);

	float Det() const;

	Matrix4 Inverse() const;
public:
	//  우리가 사용하는 배열, element[16]의 행렬에서의 위치는 아래와 같다
	//  [  e0  e4  e8  e12 ]
	//  [  e1  e5  e9  e13 ]
	//  [  e2  e6  e10 e14 ]
	//  [  e3  e7  e11 e15 ]

	union
	{
		float _element[MATRIX4_ELEMENT][MATRIX4_ELEMENT];
		float element[MATRIX4_ELEMENT * MATRIX4_ELEMENT];
	};
};

} //namespace light
