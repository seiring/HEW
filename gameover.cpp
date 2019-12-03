#include"common.h"
#include"texture.h"
#include"sprite.h"
#include"input.h"
#include "scene.h"
#include"sound.h"
static int  g_tex_id = -1;

static float g_gameover_x = 0.0;
static float g_gameover_y = 0.0;
static bool push = false;

void Gameover_Init()
{
	Sprite_Init();
	g_tex_id = Texture_SetLoadFile("Asset/Texture/gameover.png", SCREEN_WIDTH, SCREEN_HEIGHT);
	PlaySound(SOUND_LABEL_BGM002);
}
void Gameover_Uninit()
{
	StopSound(SOUND_LABEL_BGM002);
}
void Gameover_Update()
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Scene_SetNextScene(SCENE_TITLE);
	}
}
void Gameover_Draw()
{
	Sprite_Draw(g_tex_id, g_gameover_x, g_gameover_y);
}