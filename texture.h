#pragma once
#include<d3d9.h>
#include<d3dx9.h>
void Texture_Init(void);  //�e�N�X�`���Ǘ����W���[���̏�����
int Texture_SetLoadFile(const char* pFileName, int width, int height);  //���[�h�������t�@�C����o�^���Ă��� ������(�t�@�C����,�e�N�X�`���t�@�C���̉摜�̕�,����) �߂�l�F�Ǘ��ԍ�
int Texture_Load(void);  //���܂œo�^���Ă������t�@�C�����������ɑS�ēǂݍ��܂��@�߂�l�F�ǂݍ��݂Ɏ��s������
void Texture_Release(int ids[],int count);
void Texture_Release(void);
LPDIRECT3DTEXTURE9 Texture_GetTexture(int id);
int Texture_GetWidth(int id);
int Texture_GetHeight(int id);