#define _CRT_SECURE_NO_WARNINGS
#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"mydirect3d.h"
#include<string.h>


#define FILENAME_MAX (128)
#define TEXTUREDATA_MAX (256)


typedef struct TextureData_tag
{
	char filename[FILENAME_MAX];
	int width;
	int height;
	LPDIRECT3DTEXTURE9 pTexture;
}TextureData;


static LPDIRECT3DDEVICE9 g_pDevice = NULL;
static TextureData g_TextureData[TEXTUREDATA_MAX];


//static TextureData g_TextureData[TEXTUREDATA_MAX] = {};
void Texture_Init(void)  //�e�N�X�`���Ǘ����W���[���̏�����
{
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		g_TextureData[i].filename[0] = 0;
		g_TextureData[i].pTexture = NULL;
		g_TextureData[i].width = 0;
		g_TextureData[i].height = 0;
	}
}

int Texture_SetLoadFile(const char* pFilename, int width, int height)  //���[�h�������t�@�C����o�^���Ă��� ������(�t�@�C����,�e�N�X�`���t�@�C���̉摜�̕�,����) �߂�l�F�Ǘ��ԍ�
{
	//���łɓo�^����Ă��Ȃ����H����
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (strcmp(g_TextureData[i].filename, pFilename) == 0) //0�Ŕ����A�����t�@�C������������
		{
			return i; //�Ǘ��ԍ�i��Ԃ�
		}
	}
	//�f�[�^�x�[�X�̎g�p����Ă��Ȃ��������������o�^����
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].filename[0] != 0)  //�o�^����Ă�����
		{
			continue; //�g�p�ς݂Ȃ̂Ŏ������� 
		}
	    //�o�^����
		strcpy(g_TextureData[i].filename, pFilename);
		g_TextureData[i].width = width;
		g_TextureData[i].height = height;
		return i;  //�Ǘ��ԍ���߂�
	}
	return -1;  //TEXTUREDATA_MAX���g���؂���(�o�^�ł���Ƃ��낪�Ȃ�����)
}

int Texture_Load(void)  //���܂œo�^���Ă������t�@�C�����������ɑS�ēǂݍ��܂��@�߂�l�F�ǂݍ��݂Ɏ��s������
{
	g_pDevice = MyDirect3D_GetDevice();

	int error_count = 0; //�G���[�̐��𐔂���ϐ�

	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].filename[0] == 0)  //�o�^����Ă��Ȃ�������
		{
			continue;
		}
		if (g_TextureData[i].pTexture != 0)
		{
			continue;
		}

		//�e�N�X�`���̓ǂݍ���
		HRESULT hr = D3DXCreateTextureFromFile(g_pDevice, g_TextureData[i].filename, &g_TextureData[i].pTexture);
		if (FAILED(hr))
		{
			error_count++;
		}
	}
	return error_count;
}

void Texture_Release(int ids[], int count)  //�e�N�X�`�����ꕔ�J������֐� ����(int�^�̔z��̐擪�A�h���X,�z��̐�) int a[]={1,3,6}
{
	for (int i = 0; i < count; i++)
	{
		if (g_TextureData[ids[i]].pTexture == NULL)
		{
			g_TextureData[ids[i]].pTexture->Release();
			g_TextureData[ids[i]].pTexture = NULL;
			g_TextureData[ids[i]].filename[0] = 0;
			g_TextureData[ids[i]].width = 0;
			g_TextureData[ids[i]].height = 0;
		}
	}
}

void Texture_Release(void) //�e�N�X�`����S�J��
{
	for (int i = 0; i < TEXTUREDATA_MAX; i++)
	{
		if (g_TextureData[i].pTexture != NULL)
		{
			g_TextureData[i].pTexture->Release();
			g_TextureData[i].pTexture = NULL;
			g_TextureData[i].filename[0] = 0;
			g_TextureData[i].width = 0;
			g_TextureData[i].height = 0;
		}
	}
}

LPDIRECT3DTEXTURE9 Texture_GetTexture(int id)  //�C���^�[�t�F�[�X�|�C���^���擾�ł���
{
	return g_TextureData[id].pTexture;
}

int Texture_GetWidth(int id) //�e�N�X�`���̕����擾�ł���
{
	return g_TextureData[id].width;
} 

int Texture_GetHeight(int id)  //�e�N�X�`���̍������擾�ł���
{
	return g_TextureData[id].height;
}