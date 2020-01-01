#include "Score.h"
#include "scene.h"
#include "input.h"

static int g_score_count = 0;

void Score_Init()
{
	g_score_count = 0;
}

void Score_Uninit()
{

}

void Score_Update()
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Scene_SetNextScene(SCENE_GAMEOVER);
	}
}

void Score_Draw()
{

}

//追加ーーーーーーーー2020/1/1
//呼ばれたときにカウントをプラスする　timeに使う
int Score_Count(int counter)
{
	g_score_count = counter;
	return g_score_count;
}