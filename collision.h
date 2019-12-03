#pragma once
#include<d3dx9.h>
typedef struct Circle_tag
{
	D3DXVECTOR2 position;  //’†SÀ•W
	float radius;
}Circle;

bool IsHitCollision_Circle_Circle(const Circle* pA, const Circle* pB);