#include"common.h"
#include"texture.h"
#include"sprite.h"
#include"input.h"
#include "scene.h"
#include "sound.h"

static int  g_tex_id = -1;

static float g_title_x = 0.0;
static float g_title_y = 0.0;
static bool push = false;

void Title_Init()
{
	Sprite_Init();
	g_tex_id = Texture_SetLoadFile("Asset/Texture/title.jpg", SCREEN_WIDTH, SCREEN_HEIGHT);
	PlaySound(SOUND_LABEL_BGM000);
}
void Title_Uninit()
{
	StopSound(SOUND_LABEL_BGM000);
}
void Title_Update()
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Scene_SetNextScene(SCENE_TYUTORIAL);
	}
}
void Title_Draw()
{
	Sprite_Draw(g_tex_id, g_title_x, g_title_y);
}