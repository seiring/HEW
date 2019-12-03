#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"texture.h"
#include"sprite.h"

static int g_FrameCount = 0;
static int g_bangou = -1;


void SpriteAnim_Init()
{
	g_bangou = Texture_SetLoadFile("Asset/Texture/exp.png", 128, 128);
}
void SpriteAnim_Update()
{
	g_FrameCount++;
	
}
void SpriteAnim_Draw(float dx, float dy)
{
	int pattern = g_FrameCount / 4 % 5 * 32 ;
	int pattern2 = g_FrameCount / 5 / 5 * 32;
	//Sprite_Draw(g_bangou, 0, 0);
	Sprite_Draw(g_bangou, dx, dy, pattern, pattern2 , 32, 32);
}
void SpriteAnim_Uninit()
{

}