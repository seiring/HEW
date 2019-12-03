#include"collision.h"
#ifndef _UZU_H_
#define _UZU_H_
#define UZU_MAX (256)

void UzuInit(void);
void UzuUninit(void);
void UzuUpdate(void);
void UzuDraw(void);
void Uzu_Create(float x, float z);
const Circle* Uzu_GetCircleCollision(int i);
bool Uzu_IsUsed(int i);
void Uzu_Destroy(int i);
float GET_UzuX(int i);
float GET_UzuZ(int i);
float Get_count();
#endif  _UZU_H_
