#include<d3dx9.h>
#include <stdio.h>
#include"mydirect3d.h"
#include"common.h"
#include"texture.h"
#include"sprite.h"
#include "player.h"
#include"input.h"
#include "wind.h"
#include"collision.h"
#include"debug_font.h"
#include "Gorl.h"
#include"uzu.h"
#include"scene.h"
#define PLAYER_HEIGHT	491
#define PLAYER_WIDTH	348

typedef struct PlayerVertex_tag
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;  //�@��
	D3DCOLOR color;
	D3DXVECTOR2 uv;
	Circle collision;
	bool used;
}PlayerVertex;

#define FVF_PLAYER (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

static float g_Angle = 1.0f;
static float g_angle_x = 0.0f;
static float g_angle_y = 0.0f;

static D3DXVECTOR3 g_playerpos;  //�v���C���[�̈ʒu
static D3DXVECTOR3 g_old_pos;

static bool g_player_flag;       //�v���C���[�̐�������
static Circle g_player_collision;//�v���C���[�����蔻��p
static int g_player_hp;          //�v���C���[��HP
static int g_playercount;        //�v���C���[���G����p

static int g_vec = 1;
static int frame = 0;

static int player_start = 3;
static float old_counter = 0.0f;  //��ʑJ�ڃJ�E���g
static float time = 0.0f;         //�J�E���g�p

//�e�N�X�`���Ǘ��p
static int g_texid = -1;
static int g_texidR = -1;
static int g_texidL = -1;

static PlayerVertex g_cube_vertex[] = {

	//1��(�O)
	{D3DXVECTOR3(-0.8f, 1.6f,0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,0.0f)},
	{D3DXVECTOR3( 0.8f, 1.6f,0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,0.0f)},
	{D3DXVECTOR3(-0.8f,-0.0f,-0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(0.0f,1.0f)},
	{D3DXVECTOR3( 0.8f,-0.0f,-0.0f),D3DXVECTOR3(0.0f,0.0f,-1.0f),D3DCOLOR_RGBA(255,255,255,255),D3DXVECTOR2(1.0f,1.0f)},
};




void PlayerInit()
{
	g_texid = Texture_SetLoadFile("Asset/Texture/player_C.png", PLAYER_WIDTH, PLAYER_HEIGHT);
	g_texidR = Texture_SetLoadFile("Asset/Texture/player_R.png", PLAYER_WIDTH, PLAYER_HEIGHT);
	g_texidL = Texture_SetLoadFile("Asset/Texture/player_L.png", PLAYER_WIDTH, PLAYER_HEIGHT);
	Texture_Load();

	g_playerpos = D3DXVECTOR3(0.0f,0.0f, -3.0f);  //�v���C���[�����ʒu�ݒ�
	g_player_flag = true;                         //�v���C���[����
	g_player_collision.radius = 0.2f;            //�����蔻��̉~�̑傫��
	g_player_hp = 3;
	g_playercount = 0;                           //�v���C���[���G���莞��

	D3DXVECTOR3 up(0.0f, 0.0f, 0.0f);
	//���̕����Ɨ�
	float g_kazamuki = Get_Kazamuki();
	float g_fuuryou = Get_Fuuryou();

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	pDevice->CreateVertexBuffer(sizeof(PlayerVertex) * 4, D3DUSAGE_WRITEONLY, FVF_PLAYER, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
	WORD *pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
	pIndex[0] = 0, pIndex[1] = 1, pIndex[2] = 2;
	pIndex[3] = 1, pIndex[4] = 3, pIndex[5] = 2;

	g_pIndexBuffer->Unlock();

	PlayerVertex *pv;
	g_pVertexBuffer->Lock(0, 0, (void**)&pv, D3DLOCK_DISCARD);
	//                      ���S��       �����z�A�h���X
	memcpy(pv, g_cube_vertex, sizeof(g_cube_vertex));

	g_pVertexBuffer->Unlock(); //Unlock�������牼�z�A�h���X�͎g���Ȃ�
}

void PlayerUninit()
{
	float g_kazamuki = Get_Kazamuki();
	float g_fuuryou = Get_Fuuryou();

	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
	if (g_pIndexBuffer)
	{
		g_pIndexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}

void PlayerUpdate()
{
	g_vec = 0;
	int g_kazamuki = Get_Kazamuki();
	float g_fuuryou = Get_Fuuryou();
	float playermove = 0.05f;
	float counter = Get_count();
	float end = End();
	
	//�v���C���[�̉�ʊO����
	if (counter >= 60 * player_start&&end == false)
	{
		//�v���C���[�̓��͈ړ�

		if (Keyboard_IsPress(DIK_W))
		{

			g_vec = 1;
			g_playerpos.z += playermove;
			//g_angle_y += 0.01f;
		}


		if (Keyboard_IsPress(DIK_S))
		{
			g_vec = 1;
			g_playerpos.z -= playermove;
			//g_angle_y -= 0.01f;
		}


		if (Keyboard_IsPress(DIK_D))
		{
			g_vec = 2;
			g_playerpos.x += playermove;
			//g_angle_x += 0.01f;
		}


		if (Keyboard_IsPress(DIK_A))
		{
			g_vec = 3;
			g_playerpos.x -= playermove;
			//g_angle_x -= 0.01f;
		}
		frame++;

		//���ɉe������鏈��

		switch (g_kazamuki)
		{
		case 0:
			g_playerpos.z += g_fuuryou;
			break;

		case 1:
			g_playerpos.z -= g_fuuryou;
			break;

		case 2:
			g_playerpos.x += g_fuuryou;
			break;

		case 3:
			g_playerpos.x -= g_fuuryou;
			break;

		default:
			break;
		}
	}
	

	// �G���h��\�����Ă���J�E���g���ā@�V�[���J��
	if (end == true)
	{
		//�ς�����u�Ԃ̃J�E���g����ɓ����
		if (old_counter <= 0.0)
		{
			old_counter = counter;
		}
		//�؂�ւ���Ă���̎��Ԃ�}��
		time = counter - old_counter;

		if (time >= 60 * 5)
		{
			Scene_SetNextScene(SCENE_GAMEOVER);
		}
	}

	g_old_pos = g_playerpos;

	if (g_playerpos.z <= -3 || g_playerpos.z >= 5)
	{
		g_playerpos.z = (int)g_old_pos.z;
	}
	if (g_playerpos.x <= -3 || g_playerpos.x >= 3)
	{
		g_playerpos.x = (int)g_old_pos.x;

	}

	//�v���C���[�̓����蔻��̈ʒu
	g_player_collision.position.x = g_playerpos.x;  //�v���C���[��x���W�𓖂��蔻���x���W�ɐݒ�
	g_player_collision.position.y = g_playerpos.z;  //�v���C���[��z���W�𓖂��蔻���y���W�ɐݒ�

	//�v���C���[���G����
	if (!g_player_flag)
	{
		g_playercount++;
		if (g_playercount >= 120)
		{
			g_player_flag = true;
		}
	}

}

void PlayerDraw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();  //Device�̎擾
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); //���C�g���g�p

	DebugFont_Draw(0, 210, "�v���C���[HP:%d",g_player_hp);

	//���[���h�ϊ��s��
	D3DXMATRIX mtxWorld, mtxRotation,mtxTranslation, mtxScaling;

	//�������Ȃ��s��i���s�ړ��E��]�E�g��j
	D3DXMatrixIdentity(&mtxWorld);  //�P�ʍs��ɂ���

	pDevice->SetFVF(FVF_PLAYER);

	//��]
	D3DXMatrixTranslation(&mtxTranslation, g_playerpos.x, g_playerpos.y, g_playerpos.z);

	//�s��̌v�Z
	mtxWorld =  mtxTranslation;
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); //�f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�

	//�p�^�[���ɂ���ĉE��������
	switch (g_vec)
	{
	case 1:
		pDevice->SetTexture(0, Texture_GetTexture(g_texid));
		pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(PlayerVertex));
		break;
	case 2:
		pDevice->SetTexture(0, Texture_GetTexture(g_texidL));
		pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(PlayerVertex));
		break;
	case 3:
		pDevice->SetTexture(0, Texture_GetTexture(g_texidR));
		pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(PlayerVertex));
		break;

	default:
		pDevice->SetTexture(0, Texture_GetTexture(g_texid));
		pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(PlayerVertex));
		break;
	}

 	pDevice->SetIndices(g_pIndexBuffer);
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

const Circle* Player_GetCircleCollision()
{
	return &g_player_collision;
}

void Player_Damage()
{
	if (g_player_flag)
	{
		g_playercount = 0;
		g_player_hp -= 1;
		g_player_flag = false;
	}
}

bool Get_PlayerFlag()
{
	return g_player_flag;
}