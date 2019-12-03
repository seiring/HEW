#include "collision.h"
#include "player.h"
#include "uzu.h"
#include "sound.h"

static int g_score;
static int g_count;

void Judgement_Init()
{

}

void Judgement_Update()
{
	for (int i = 0; i <= UZU_MAX; i++)
	{
		//プレイヤーとエネミーの当たり判定
		if (IsHitCollision_Circle_Circle(Player_GetCircleCollision(), Uzu_GetCircleCollision(i)))
		{
			if (Uzu_IsUsed(i) && Get_PlayerFlag())
			{
				Player_Damage();
				//Uzu_Destroy(i);
			}
		}
	}
}