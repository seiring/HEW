#include "uzu.h"
#include"common.h"
#include "scene.h"
typedef struct Uzu_Maker_tag
{
	int frame;
	float x, z;
}UzuMake;

static const UzuMake UZU_MAKE_DATA[] = {
	{60 , 0.0f,1.0f},
	{120,-2.0f,1.0f},
	{150,-4.0f,1.0f},
	{180, 2.0f,1.0f},
	{220, 3.0f,1.0f},
	{280,-2.0f,1.0f},
	{360, 3.0f,1.0f},
	{420, 4.0f,1.0f},
	{500, 3.0f,1.0f},
	{620,-2.0f,1.0f},
	{720 ,0.0f,1.0f},
	{900,-2.0f,1.0f},
	{1280,-4.0f,1.0f},
	{1380, 2.0f,1.0f},
	{1720 ,0.0f,1.0f},
	{1900,-2.0f,1.0f},
	{2220, 3.0f,1.0f},
	{2280,-2.0f,1.0f},
	{2360, 3.0f,1.0f},
	{3280,-2.0f,1.0f},
	{3360, 3.0f,1.0f},
	{3420, 4.0f,1.0f},
	{3500, 3.0f,1.0f},
	{3620,-2.0f,1.0f},
	{3820, 4.0f,1.0f},
	{4000, 3.0f,1.0f},
	{4620,-2.0f,1.0f},
	{4880,-2.0f,1.0f},
	{4960, 3.0f,1.0f},
	{4990, 4.0f,1.0f},
	{5000, 3.0f,1.0f},
};

static int g_FrameCount;
static int g_Arrow;
static const int UZU_MAKE_MAX = sizeof(UZU_MAKE_DATA) / sizeof(UZU_MAKE_DATA[0]);

void UzuMaker_Init()
{
	g_FrameCount = 0;
	g_Arrow = 0;
}

void UzuMaker_Update()
{
	for (;g_Arrow < UZU_MAKE_MAX; g_Arrow++)
	{
		if (UZU_MAKE_DATA[g_Arrow].frame == g_FrameCount)
		{
			Uzu_Create(UZU_MAKE_DATA[g_Arrow].x, UZU_MAKE_DATA[g_Arrow].z);
		}
		else
		{
			break;
		}
	}
	g_FrameCount++;
	if (g_FrameCount >= 10000)
	{
		Scene_SetNextScene(SCENE_GAMEOVER);
	}
}