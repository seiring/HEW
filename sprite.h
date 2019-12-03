#pragma once
void Sprite_Init(void);
void Sprite_Uninit(void);
void Sprite_Draw(int textureId, float dx, float dy);
void Sprite_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h);
void Sprite_Draw(int textureId, float dx, float dy, float center_x, float center_y, float angle);
void Sprite_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float center_x, float center_y, float angle);
void Sprite_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float center_x, float center_y, float angle,float sx,float sy, float sz);
void Sprite_SetColor(D3DCOLOR color);
void Sprinte_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float sx, float sy, float sz);