#pragma once
enum SCENE
{
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_GAMEOVER,
	SCENE_MAX,  //enum‚Ì”‚ğ”‚¦‚é
	SCENE_END = SCENE_MAX,  //SCENE‚ÌI—¹
};
void Scene_Init();
void Scene_Uninit();
void Scene_Update();
void Scene_Draw();
void Scene_SetNextScene(SCENE nextScene);
bool Scene_Change(void);