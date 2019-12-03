#include "scene.h"
#include "title.h"
#include "texture.h"
#include "gameover.h"
#include"game.h"
SCENE g_NextScene = SCENE_TITLE;  //ここをSCENE_GAMEにしたらGAMEから始まる
SCENE g_Scene = g_NextScene;

void Scene_Init()
{
	switch (g_NextScene)
	{
	case SCENE_TITLE:
		Title_Init();
		break;
	case SCENE_GAME:
		Game_Init();
		break;
	case SCENE_GAMEOVER:
		Gameover_Init();
		break;
	default:
		break;
	}

	if (Texture_Load() < 0)
	{
		//エラー処理
	}
}

void Scene_Uninit()
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Uninit();
		break;
	case SCENE_GAME:
		Game_Uninit();
		break;
	case SCENE_GAMEOVER:
		Gameover_Uninit();
		break;
	default:
		break;
	}
}

void Scene_Update()
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Update();
		break;
	case SCENE_GAME:
		Game_Update();
		break;
	case SCENE_GAMEOVER:
		Gameover_Update();
		break;
	default:
		break;
	}
}

void Scene_Draw()
{
	switch (g_Scene)
	{
	case SCENE_TITLE:
		Title_Draw();
		break;
	case SCENE_GAME:
		Game_Draw();
		break;
	case SCENE_GAMEOVER:
		Gameover_Draw();
		break;
	default:
		break;
	}
}

void Scene_SetNextScene(SCENE nextScene)
{
	g_NextScene = nextScene;  //次のシーンへの移動
}

bool Scene_Change(void)
{
	if (g_Scene != g_NextScene)
	{
		if (g_NextScene == SCENE_END)
		{
			return true;
		}
		Scene_Uninit();
		Scene_Init();
		g_Scene = g_NextScene;
	}
	return false;
}