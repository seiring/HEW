#include<d3d9.h>
#include<d3dx9.h>
#include "collision.h"
#include "common.h"
#include "texture.h"

bool IsHitCollision_Circle_Circle(const Circle* pA, const Circle* pB)
{
	return D3DXVec2LengthSq(&(pB->position - pA->position)) < (pA->radius + pB->radius)*(pA->radius + pB->radius);
}