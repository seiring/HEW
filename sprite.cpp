#include<windows.h>
#include<d3d9.h>
#include<d3dx9.h>
#include"mydirect3d.h"
#include"texture.h"
#include<stdlib.h>

//���_�̌`��\�����钸�_�\���̂�錾����
typedef struct Vertex2d_tag
{
	D3DXVECTOR4 position;  //���W
	D3DCOLOR color;
	D3DXVECTOR2 uv; //texcoord;
}vertex2d;

//�f�o�C�X�ɒ��_�̌`��`���邽�߂�FVF��錾����
#define FVF_VERTEX2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

static D3DCOLOR g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
static LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;
static LPDIRECT3DINDEXBUFFER9 g_pIndexBuffer = NULL;

void Sprite_SetColor(D3DCOLOR color)
{
	g_Color = color;
}

void Sprite_Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	pDevice->CreateVertexBuffer(sizeof(vertex2d) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX2D, D3DPOOL_MANAGED, &g_pVertexBuffer, NULL);
	pDevice->CreateIndexBuffer(sizeof(WORD) * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_pIndexBuffer, NULL);
	WORD *pIndex;
	g_pIndexBuffer->Lock(0, 0, (void**)&pIndex, D3DLOCK_DISCARD);
	pIndex[0] = 0,pIndex[1] = 1,pIndex[2] = 2;
	pIndex[3] = 1,pIndex[4] = 3,pIndex[5] = 2;
	g_pIndexBuffer->Unlock();
}

void Sprite_Uninit(void)
{
	if (g_pVertexBuffer)
	{
		g_pVertexBuffer -> Release();
		g_pVertexBuffer = NULL;
	}
}

void Sprite_Draw(int textureId, float dx, float dy)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice) 
	{ 
		return;
	}  

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f    , dy - 0.5f    ,0.0f,1.0f), g_Color, D3DXVECTOR2(0.0f,0.0f)},
					 {D3DXVECTOR4(dx - 0.5f + w, dy - 0.5f    ,0.0f,1.0f), g_Color, D3DXVECTOR2(1.0f,0.0f)},
					 {D3DXVECTOR4(dx - 0.5f    , dy - 0.5f + h,0.0f,1.0f), g_Color, D3DXVECTOR2(0.0f,1.0f)},
					 {D3DXVECTOR4(dx - 0.5f + w, dy - 0.5f + h,0.0f,1.0f), g_Color, D3DXVECTOR2(1.0f,1.0f)},
	};

	vertex2d *pv;
	g_pVertexBuffer->Lock(0, 0 ,(void**)&pv,D3DLOCK_DISCARD); 
	//                      ���S��       �����z�A�h���X
	memcpy(pv, v, sizeof(v));
	//     ���恪������
	g_pVertexBuffer->Unlock(); //Unlock�������牼�z�A�h���X�͎g���Ȃ�

	pDevice->SetStreamSource(0, g_pVertexBuffer, 0, sizeof(vertex2d));

	pDevice->SetFVF(FVF_VERTEX2D);  //FVF�̐ݒ�

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	pDevice->SetIndices(g_pIndexBuffer);

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(�}�`�̌`,��,���_�f�[�^�̐擪�A�h���X,���_����̃T�C�Y)
}

void Sprite_Draw(int textureId, float dx, float dy,int cut_x,int cut_y,int cut_w,int cut_h)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	if (!pDevice)
	{
		return;
	}

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;


	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u0,v0)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u1,v0)},
					 {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u0,v1)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u1,v1)},
	};

	pDevice->SetFVF(FVF_VERTEX2D);  //FVF�̐ݒ�

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(�}�`�̌`,��,���_�f�[�^�̐擪�A�h���X,���_����̃T�C�Y)
}

void Sprite_Draw(int textureId, float dx, float dy, float center_x ,float center_y ,float angle)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXMATRIX mtxR;
	D3DXMATRIX mtxT;
	D3DXMATRIX mtxIT;

	if (!pDevice) 
	{
		return;
	}

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);
	
	//��]�s��
	D3DXMatrixRotationZ(&mtxR, angle);
	//���s�ړ��s��
	D3DXMatrixTranslation(&mtxT, -center_x, -center_y, 0.0f);
	                                                   //z
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0.0f);

	//�s��̍���(�����̏��Ԃɂ͈Ӗ�������)
	D3DXMATRIX mtxW = mtxT * mtxR * mtxIT;
	                 //���s����]�����s�ړ�

	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f     , dy - 0.5f     ,0.0f,1.0f), g_Color, D3DXVECTOR2(0.0f,0.0f)},
					 {D3DXVECTOR4(dx - 0.5f + w , dy - 0.5f     ,0.0f,1.0f), g_Color, D3DXVECTOR2(1.0f,0.0f)},
					 {D3DXVECTOR4(dx - 0.5f     , dy - 0.5f + h ,0.0f,1.0f), g_Color, D3DXVECTOR2(0.0f,1.0f)},
					 {D3DXVECTOR4(dx - 0.5f + w , dy - 0.5f + h ,0.0f,1.0f), g_Color, D3DXVECTOR2(1.0f,1.0f)},
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	pDevice->SetFVF(FVF_VERTEX2D);  //FVF�̐ݒ�

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(�}�`�̌`,��,���_�f�[�^�̐擪�A�h���X,���_����̃T�C�Y)
}

void Sprite_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float center_x, float center_y, float angle)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxR;
	D3DXMATRIX mtxT;
	D3DXMATRIX mtxIT;

	if (!pDevice)
	{
		return;
	}

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;

	//��]�s��
	D3DXMatrixRotationZ(&mtxR, angle);
	//���s�ړ��s��
	D3DXMatrixTranslation(&mtxT, -center_x, -center_y, 0.0f);
	//z
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0.0f);

	//�s��̍���(�����̏��Ԃɂ͈Ӗ�������)
	D3DXMATRIX mtxW = mtxT * mtxR * mtxIT;
	//���s����]�����s�ړ�

	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u0,v0)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u1,v0)},
					 {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u0,v1)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u1,v1)},
	};

	WORD index[] = {
		0,1,2,
		1,3,2
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	pDevice->SetFVF(FVF_VERTEX2D);  //FVF�̐ݒ�

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(�}�`�̌`,��,���_�f�[�^�̐擪�A�h���X,���_����̃T�C�Y)

	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0 , 4 , 2 , index , D3DFMT_INDEX16 , v , sizeof(vertex2d));
}
void Sprite_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float center_x, float center_y, float angle,float sx,float sy ,float sz)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	D3DXMATRIX mtxR;
	D3DXMATRIX mtxT;
	D3DXMATRIX mtxIT;
	D3DXMATRIX mtxS;

	if (!pDevice) {
		return;
	}

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;

	//��]�s��
	D3DXMatrixRotationZ(&mtxR, angle);
	//���s�ړ��s��
	D3DXMatrixTranslation(&mtxT, -center_x, -center_y, 0.0f);
	//z
	D3DXMatrixTranslation(&mtxIT, center_x + dx, center_y + dy, 0.0f);
	//�g��s��
	D3DXMatrixScaling(&mtxS, sx, sy, sz);

	//�s��̍���(�����̏��Ԃɂ͈Ӗ�������)
	D3DXMATRIX mtxW = mtxT * mtxS * mtxR * mtxIT;
	//���s����]�����s�ړ�

	vertex2d v[] = { {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u0,v0)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f ,0.0f   ,1.0f), g_Color, D3DXVECTOR2(u1,v0)},
					 {D3DXVECTOR4(dx - 0.5f        , dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u0,v1)},
					 {D3DXVECTOR4(dx - 0.5f + cut_w, dy - 0.5f + cut_h ,0.0f,1.0f), g_Color, D3DXVECTOR2(u1,v1)},
	};

	WORD index[] = {
		0,1,2,
		1,3,2
	};

	D3DXVec4Transform(&v[0].position, &v[0].position, &mtxW);
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW);
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW);
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW);

	pDevice->SetFVF(FVF_VERTEX2D);  //FVF�̐ݒ�

	pDevice->SetTexture(0, Texture_GetTexture(textureId));

	//pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(vertex2d));  //(�}�`�̌`,��,���_�f�[�^�̐擪�A�h���X,���_����̃T�C�Y)

	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(vertex2d));
}

void Sprinte_Draw(int textureId, float dx, float dy, int cut_x, int cut_y, int cut_w, int cut_h, float angle, float sx, float sy, float sz)
{
	D3DCOLOR g_Color = D3DCOLOR_RGBA(255, 255, 255, 255);
	typedef struct Vertex2d_tag
	{
		D3DXVECTOR4 position; //���W
		D3DCOLOR color;
		D3DXVECTOR2 uv; //texcoord;
	}Vertex2d;

	int w = Texture_GetWidth(textureId);
	int h = Texture_GetHeight(textureId);

	float u0 = cut_x / (float)w;
	float v0 = cut_y / (float)h;
	float u1 = (cut_x + cut_w) / (float)w;
	float v1 = (cut_y + cut_h) / (float)h;

	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();

	Vertex2d v[] =
	{
	{ D3DXVECTOR4(dx - 0.5f , dy - 0.5f,0.0f,1.0f),g_Color,D3DXVECTOR2(u0,v0) },//0.5���炷�̂̓��j�A�t�B���^�����O�����ꂢ�ɂ��邽��
	{ D3DXVECTOR4(dx + cut_w - 0.5f , dy - 0.5f,0.0f,1.0f),g_Color,D3DXVECTOR2(u1,v0) },
	{ D3DXVECTOR4(dx - 0.5f , dy + cut_h - 0.5f,0.0f,1.0f),g_Color,D3DXVECTOR2(u0,v1) },
	{ D3DXVECTOR4(dx + cut_w - 0.5f , dy + cut_h - 0.5f,0.0f,1.0f),g_Color,D3DXVECTOR2(u1,v1) },
	};

	//�C���f�b�N�Xver�P�Ubit
	WORD index[] =
	{
		0,1,2,
		1,3,2,
	};

	//--------------------------------------
	//���_�ɂǐ^�񒆂��Z�b�g���邽�߂ɂ��炷
	//--------------------------------------
	D3DXMATRIX mtxT;
	D3DXMatrixTranslation(&mtxT, -cut_w / 2 - dx - 0.5f, -cut_h / 2 - dy - 0.5f, 0.0f);
	//--------------------------------------
	//�g��
	//--------------------------------------
	D3DXMATRIX mtxS;
	D3DXMatrixScaling(&mtxS, sx, sy, sz);//1.0f���P�O�O���ɂȂ��Ă�B�������O�D�T��
	//-------------------------------------
	//��]����
	//-------------------------------------
	D3DXMATRIX mtxR;											//�ϐ��錾�@float���\�Z��
	D3DXMatrixRotationZ(&mtxR, angle);							//�o�͂Ɖ�]�p�i���W�A���j
	//-------------------------------------
	//���ɖ߂�
	//-------------------------------------
	D3DXMATRIX mtxIT;
	D3DXMatrixTranslation(&mtxIT, cut_w / 2 + dx - 0.5f, cut_h / 2 + dy - 0.5f, 0.0f);
	//--------------------------------------
	//��������
	//--------------------------------------
	D3DXMATRIX mtxW = mtxT * mtxS* mtxR * mtxIT;// �ŏI�I��mtxW�ō��W�ϊ��@�@���s�ړ�����]�����s�ړ�

	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW); //�o��,����,�s��
	D3DXVec4Transform(&v[1].position, &v[1].position, &mtxW); //�o��,����,�s��
	D3DXVec4Transform(&v[2].position, &v[2].position, &mtxW); //�o��,����,�s��
	D3DXVec4Transform(&v[3].position, &v[3].position, &mtxW); //�o��,����,�s��

	if (!pDevice)
	{
		return;
	}

	pDevice->SetFVF(FVF_VERTEX2D);
	pDevice->SetTexture(0, Texture_GetTexture(textureId));
	pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, index, D3DFMT_INDEX16, v, sizeof(Vertex2d));//�C���f�b�N�X�p
}