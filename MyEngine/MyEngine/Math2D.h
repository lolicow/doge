/*-------------------------------------------------------
Copyright (C) 2015 DigiPen Institute of Technology. Reproduction or disclosure of this file or its contents without the prior written consent of DigiPen Institute of Technology is prohibited.

File Name:  Math2D.h
Purpose:  header file for Math2D class
Language: c
Platform: visual studio 2015,windows
Project: tuoming.li, CS529, CS529_CS529_Project1_Part1
Author:  Li Tuoming,tuoming.li,60001515
Creation date: 9/12/2015
---------------------------------------------------------*/
#ifdef __cplusplus
extern "C" {  //only visible by C++ compiler
#endif


#ifndef MATH2D_H
#define MATH2D_H


#include "Vector2D.h"

/*
This function checks if the point P is colliding with the circle whose
center is "Center" and radius is "Radius"
*/
int StaticPointToStaticCircle(Vector2D *pP, Vector2D *pCenter, float Radius);


/*
This function checks if the point Pos is colliding with the rectangle
whose center is Rect, width is "Width" and height is Height
*/
int StaticPointToStaticRect(Vector2D *pPos, Vector2D *pRect, float Width, float Height);

/*
This function checks for collision between 2 circles.
Circle0: Center is Center0, radius is "Radius0"
Circle1: Center is Center1, radius is "Radius1"
*/
int StaticCircleToStaticCircle(Vector2D *pCenter0, float Radius0, Vector2D *pCenter1, float Radius1);

/*
This functions checks if 2 rectangles are colliding
Rectangle0: Center is pRect0, width is "Width0" and height is "Height0"
Rectangle1: Center is pRect1, width is "Width1" and height is "Height1"
*/
int StaticRectToStaticRect(Vector2D *pRect0, float Width0, float Height0, Vector2D *pRect1, float Width1, float Height1);

int StaticRectToStaticCircle(Vector2D *pRect0, float Width0, float Height0, Vector2D *pCenter1, float Radius1);

#endif

#ifdef __cplusplus
} //only visible by C++ compiler
#endif