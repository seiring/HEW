#include "Score.h"
#include "scene.h"
#include "input.h"

void Score_Init()
{

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