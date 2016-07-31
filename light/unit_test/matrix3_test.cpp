#include "stdafx.h"
#include "Matrix.hpp"

TEST(Matrix3, Test)
{

	//      [  2  1  3  ]       [  1  0 -2  ]
	//  A = [  1  2 -1  ]   B = [  3  1 -1  ]
	//      [  2 -1  3  ]       [  0  4 -2  ]
	float array1[] = { 2.0f, 1.0f, 2.0f, 1.0f, 2.0f, -1.0f, 3.0f, -1.0f, 3.0f };
	float array2[] = { 1.0f, 3.0f, 0.0f, 0.0f, 1.0f, 4.0f, -2.0f, -1.0f, -2.0f };
	light::Matrix3 matA(array1), matB(array2);
	light::Matrix3 result; 

	result = matA; //´ÜÀ§Çà·Ä, I
	result.Identity();

	result = matA;
	result.Transpose(); //ÀüÄ¡Çà·Ä, A^T

	result = matA + matB; //Çà·ÄÀÇ µ¡¼À, A + B

	result = matA - matB; //Çà·ÄÀÇ »¬¼À, A - B

	result = matA * matB; //Çà·ÄÀÇ °ö¼À, A * B

	result = matA;
	result.Scalar(3.0f); //Çà·Ä°ú ½ºÄ®¶óÀÇ °ö, 3 * A

	float det = matA.Det(); //Çà·Ä½Ä, det(A)
	UNREFERENCED_PARAMETER(det);

	result = matB.Inverse(); //¿ªÇà·ÄÀÇ °ö¼À, B^-1
}
