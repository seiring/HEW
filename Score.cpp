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

//�ǉ��[�[�[�[�[�[�[�[2020/1/1
//�Ă΂ꂽ�Ƃ��ɃJ�E���g���v���X����@time�Ɏg��
int Score_Count(int counter)
{
	g_score_count = counter;
	return g_score_count;
}