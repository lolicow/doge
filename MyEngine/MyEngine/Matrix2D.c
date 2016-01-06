/*-------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

File Name:  Matrix2D.c
Purpose:  implementation file for 2d matrix class, build transformation matrix and implement matrix multiplication
Language: c
Platform: visual studio 2015,windows
Project: tuoming.li, CS529, CS529_CS529_Project1_Part1
Author:  Li Tuoming,tuoming.li,60001515
Creation date: 9/12/2015
---------------------------------------------------------*/

#include "Matrix2D.h"
#include "stdio.h"
#define PI 3.1415926535897932384626433832795


/*
This function print a matrix.
*/

void PrintMatrix(Matrix2D* matrix)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			printf("m[%d][%d]=%f  ", i, j, matrix->m[i][j]);
		}
	printf("\n");
	}
	printf("\n");
}


/*
This function sets the matrix Result to the identity matrix
*/
void Matrix2DIdentity(Matrix2D *pResult)
{
	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			if (i == j)
				pResult->m[i][j] = 1.0f;
			else
				pResult->m[i][j] = 0.0f;
		}
	}
}

// ---------------------------------------------------------------------------

/*
This functions calculated the transpose matrix of Mtx and saves it in Result
*/
void Matrix2DTranspose(Matrix2D *pResult, Matrix2D *pMtx)
{
	float tempforswapping = 0.0f;
	for (int i = 0;i < 3;i++)
	{
		for (int j = 0;j < 3;j++)
		{
			tempforswapping = pMtx->m[i][j];
			pResult->m[j][i] = tempforswapping;
			tempforswapping = pMtx->m[j][i];
			pResult->m[i][j] = tempforswapping;
		}
	}
}

// ---------------------------------------------------------------------------

/*
This function multiplies Mtx0 with Mtx1 and saves the result in Result
Result = Mtx0*Mtx1
*/
void Matrix2DConcat(Matrix2D *pResult, Matrix2D *pMtx0, Matrix2D *pMtx1)
{
	int i, j, k = 0;
	Matrix2D tempforreult;
	for (i = 0;i < 3;i++)
	{
		for (j = 0;j < 3;j++)
		{
			tempforreult.m[i][j] = 0;
			for (k = 0;k < 3;k++)
			{
				tempforreult.m[i][j] = tempforreult.m[i][j] + (pMtx0->m[i][k] * pMtx1->m[k][j]);
			}
		}
	}

	for (i = 0;i < 3;i++)
	{
		for (j = 0;j < 3;j++)
		{
			pResult->m[i][j] = tempforreult.m[i][j];
		}
	}
}

// ---------------------------------------------------------------------------

/*
This function creates a translation matrix from x *p y and saves it in Result
*/
void Matrix2DTranslate(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][2] = x;
	pResult->m[1][2] = y;
}

// ---------------------------------------------------------------------------

/*
This function creates a scaling matrix from x *p y and saves it in Result
*/
void Matrix2DScale(Matrix2D *pResult, float x, float y)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = x;
	pResult->m[1][1] = y;
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in degree.
Save the resultant matrix in Result
*/
void Matrix2DRotDeg(Matrix2D *pResult, float Angle)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = cos(Angle / 180 * PI);
	pResult->m[0][1] = -sin(Angle / 180 * PI);
	pResult->m[1][0] = sin(Angle / 180 * PI);
	pResult->m[1][1] = cos(Angle / 180 * PI);
}

// ---------------------------------------------------------------------------

/*
This matrix creates a rotation matrix from "Angle" whose value is in radian.
Save the resultant matrix in Result
*/
void Matrix2DRotRad(Matrix2D *pResult, float Angle)
{
	Matrix2DIdentity(pResult);
	pResult->m[0][0] = cos(Angle);
	pResult->m[0][1] = -sin(Angle);
	pResult->m[1][0] = sin(Angle);
	pResult->m[1][1] = cos(Angle);
}

// ---------------------------------------------------------------------------

/*
This function multiplies the matrix Mtx with the vector Vec and saves the result in Result
Result = Mtx * Vec
*/
void Matrix2DMultVec(Vector2D *pResult, Matrix2D *pMtx, Vector2D *pVec)
{
	float x, y, w = 0.0f;
	x = pMtx->m[0][0] * pVec->x + pMtx->m[0][1] * pVec->y + pMtx->m[0][2] * 1;
	y = pMtx->m[1][0] * pVec->x + pMtx->m[1][1] * pVec->y + pMtx->m[1][2] * 1;
	w = pMtx->m[2][0] * pVec->x + pMtx->m[2][1] * pVec->y + pMtx->m[2][2] * 1;

	if (w != 0)
	{
		pResult->x = x / w;
		pResult->y = y / w;
	}
	else
	{
		pResult->x = x;
		pResult->y = y;
	}
}

// ---------------------------------------------------------------------------
