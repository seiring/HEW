#ifndef _Player_H_
#define _Player_H_
#include<d3dx9.h>
#include"collision.h"

typedef struct Player_tag
{
	D3DXVECTOR2 position;
	float radius;
}Player;

void PlayerInit();
void PlayerUninit();
void PlayerUpdate();
void PlayerDraw();
const Circle* Player_GetCircleCollision();
void Player_Damage();
bool Get_PlayerFlag();

#endif  _CUBE_H_
