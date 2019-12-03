#include<d3dx9.h>
#include"mydirect3d.h"
#include"common.h"
#include"camerar.h"

float x = 5.0f;
float y = 0.0f;
float z = -5.0f;
typedef struct GridVertex_tag
{
	D3DXVECTOR3 position;
	D3DCOLOR color;
}GridVertex;

#define FVF_GRID (D3DFVF_XYZ|D3DFVF_DIFFUSE)

static GridVertex g_grid_vertex[44];

void Grid_Init()
{
	for (int i = 0; i < 22; i++)
	{
		x = -5.0f;
		g_grid_vertex[i].position = D3DXVECTOR3(x, y, z);
		x = 5.0f;
		g_grid_vertex[i + 1].position = D3DXVECTOR3(x, y, z);
		z += 1.0f;
		i++;	
	}
	for (int i = 22; i < 44; i++)
	{
		z = -5.0f;
		g_grid_vertex[i].position = D3DXVECTOR3(x, y, z);
		z = 5.0f;
		g_grid_vertex[i + 1].position = D3DXVECTOR3(x, y, z);
		x -= 1.0f;
		i++;
	}
	for (int i = 0; i < 44;i++)
	{
		g_grid_vertex[i].color = D3DCOLOR_RGBA(125, 255, 125, 255);
	}
}

void Grid_Uninit()
{

}
void Grid_Update()
{
	
}

void Grid_Draw()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	//���[���h�ϊ��s��
	D3DXMATRIX mtxWorld, mtxRotation, mtxTranslation;
	//�������Ȃ��s��i���s�ړ��E��]�E�g��j
	D3DXMatrixIdentity(&mtxWorld);  //�P�ʍs��ɂ���
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld); //�f�o�C�X�Ƀ��[���h�ϊ��s���ݒ�
	pDevice->SetFVF(FVF_GRID);
	pDevice->SetTexture(0, NULL);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); //���C�g�g�por���g�p
	pDevice->DrawPrimitiveUP(D3DPT_LINELIST, 22, &g_grid_vertex, sizeof(GridVertex));
}