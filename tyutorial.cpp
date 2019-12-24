#include "tyutorial.h"
#include "scene.h"
#include "input.h"

void tyutorial_Init()
{

}

void tyutorial_Uninit()
{

}

void tyutorial_Update()
{
	if (Keyboard_IsTrigger(DIK_SPACE))
	{
		Scene_SetNextScene(SCENE_GAME);
	}
}

void tyutorial_Draw()
{

}