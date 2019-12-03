#include"cube.h"
#include"scene.h"
#include"input.h"
#include"debug_font.h"
#include"player.h"
#include"wind.h"
#include"uzu.h"
#include"uzumaker.h"
#include"judgement.h"
#include"start.h"
#include"Gorl.h"
#include"sound.h"

static int framecount;
static int framecount2;
void Game_Init()
{
	framecount = 0;
	framecount2 = 0;
	Cube_Init();
	PlayerInit();
	Init_Wind();
	UzuInit();
	UzuMaker_Init();
	InitStart();
	InitGorl();
	PlaySound(SOUND_LABEL_BGM001);
	PlaySound(SOUND_LABEL_BGM002);
}

void Game_Uninit()
{
	Cube_Uninit();
	UzuUninit();
	Judgement_Init();
	StopSound(SOUND_LABEL_BGM002);
	StopSound(SOUND_LABEL_BGM001);
}

void Game_Update()
{
	framecount++;
	framecount2++;
	Cube_Update();
	PlayerUpdate();
	Update_Wind();
	UzuMaker_Update();
	UzuUpdate();
	Judgement_Update();
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Scene_SetNextScene(SCENE_GAMEOVER);
	}
}

void Game_Draw()
{
	Cube_Draw(); //�g�̕`��
	Draw_Wind();//�������̕`��
	UzuDraw();  //�Q�̕`��
	PlayerDraw();//�v���C���[�̕`��
	DrawStart();
	DrawGorl();
}