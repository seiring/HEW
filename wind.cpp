//#include "common.h"
#include "wind.h"
#include "texture.h"
#include "Sprite.h"
#include <stdlib.h>

#define WIND_SIZE (300)

static int g_texid = -1;
static int g_texid2 = -1;
static int g_texid3 = -1;
static int g_texid4 = -1;

static D3DXVECTOR2 g_wind_position;
static D3DXVECTOR2 g_fuuryou_Vecter;
static int g_kazamuki = 0;
static int g_kakuritu = 60;
//static float g_angle = 0.0f;
static float g_fuuryou = 0.01f;

void Init_Wind(void)
{
	g_wind_position = D3DXVECTOR2(20, 20);

	g_texid = Texture_SetLoadFile("Asset/Texture/wind_up.jpg", WIND_SIZE, WIND_SIZE);
	g_texid2 =Texture_SetLoadFile("Asset/Texture/wind_down.jpg", WIND_SIZE, WIND_SIZE);
	g_texid3 = Texture_SetLoadFile("Asset/Texture/wind_right.jpg", WIND_SIZE, WIND_SIZE);
	g_texid4 = Texture_SetLoadFile("Asset/Texture/wind_left.jpg", WIND_SIZE, WIND_SIZE);
}

void Update_Wind(void)
{
	//g_angle+=0.1f;
	if (g_kakuritu == rand() % 60 + 1)
	{
		g_kazamuki = rand() % 4;
	}
}

void Uninit_Wind(void)
{

}

void Draw_Wind(void)
{
	int WindTexture = 0;
	//テクスチャーだけを変化させてる
	if (g_kazamuki == 0) //風向き上
	{
		//置く方向の風
		WindTexture = g_texid;
	}

	if (g_kazamuki == 1) //風向き下
	{
		WindTexture = g_texid2;
	}

	if (g_kazamuki == 2) //風向き右
	{
		WindTexture = g_texid3;
	}

	if (g_kazamuki == 3) //風向き左
	{
		WindTexture = g_texid4;
	}



	//共通で表示してる
	float wariai = WIND_SIZE / 100;
	//float wariai2 = WIND_SIZE / 10;
	//wariai = (wariai + wariai2) * 0.1;

	Sprinte_Draw(WindTexture, g_wind_position.x * wariai, g_wind_position.y*wariai, 30 * wariai, 30 * wariai,40 *wariai, 40 * wariai, 0, 1.0, 1.0, 1.0);
}

int Get_Kazamuki(void)
{
	return g_kazamuki;
}

float Get_Fuuryou(void)
{
	return g_fuuryou;
}