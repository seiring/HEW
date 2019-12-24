#include"common.h"
#include<d3d9.h>
#include"texture.h"
#include<d3dx9.h>
#include"mydirect3d.h"
#include "camerar.h"

static float g_angle = 0.05f;
static float g_rad = 0.0f;
static 	float g_max = 5.0f;
static int kannri = -1;
static int g_vertex_counter = 0;
static int g_primitive_counter = 0;
static int m = 0;
static float scrool = 0.0f;
int distance[1700] = {};
static int g_kazu_x = 0;
static int g_kazu_y = 0;
#define FVF_MESH (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1) //�RD�ɂȂ���HW�͂���Ȃ��Ȃ��� �\���̂̉��ɏ���

static LPDIRECT3DINDEXBUFFER9 g_index;
static LPDIRECT3DVERTEXBUFFER9 g_Vertex;
static D3DXVECTOR3 source(0, 0, 20);

typedef struct MeshVertex_tag
{
	D3DXVECTOR3 position;
	D3DCOLOR	color;
	D3DXVECTOR2 uv;
}MeshVertex;

MeshVertex *pv;

#define FVF_GRID (D3DFVF_XYZ|D3DFVF_DIFFUSE) //�RD�ɂȂ���HW�͂���Ȃ��Ȃ��� �\���̂̉��ɏ���

void MeshInit(int x, int y)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = MyDirect3D_GetDevice();

	g_kazu_x = x;
	g_kazu_y = y;
	int t = 0;
	//���_�̐��J�E���g
	g_vertex_counter = (x + 1)*(y + 1);

	//�g�p�|���S���̐����J�E���g
	g_primitive_counter = ((x * y) * 2) + (4 * (y - 1));//�k�ނȂ�=X*y*2

	MeshVertex* g_mesh_Vertex;//�|�C���^�^��[]����Ȃ�

		//�_�u�����b�v(UV�̃��b�v)���b�N�̊O
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);//�A�h���X��U
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);//�A�h���X��V

	//���ꂪ�Ȃ���g_vertex��NUll�ɂȂ�
	pDevice->CreateVertexBuffer(sizeof(MeshVertex) * g_vertex_counter, D3DUSAGE_WRITEONLY, FVF_MESH, D3DPOOL_MANAGED, &g_Vertex, NULL);//���_
	//Lock = �ق��̃t�@�C����������Ȃ��悤�ɂ���

	//���b�N����g_Vertex
	g_Vertex->Lock(0, 0, (void**)&pv, D3DLOCK_DISCARD);
	// g_mesh_Vertex = new MeshVertex[(x+1)*(y+1)];//���_�����z������

	float yoko = 0;
	float tate = 0;
	float sea = 0.1;
	yoko = 1 / (float)x; // ��̑傫����
	tate = 1 / (float)y;//��̑傫����
	int  takasa = 0;

	int m = 0;
	//���_�C���f�b�N�X����肽��
	for (int i = 0; i < y + 1; i++)
	{
		for (int t = 0; t < x + 1; t++)
		{

			//distance = D3DXVec3LengthSq(&(source - pv[m].position));//��_�Ԃ̋��������
			//�����W�@���@�ő�̍������@������ �U�� �����ԁi�p�x�j

			pv[m].position = D3DXVECTOR3(-x / 2 + t, 0.0, -y / 2 + i); //�����@index0�@�@
			pv[m].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pv[m].uv = D3DXVECTOR2(t * yoko, (y - i) * tate);//�_�u�����b�v
			distance[m] = D3DXVec3LengthSq(&(source - pv[m].position));//��_�Ԃ̋��������

			m++;
		}
	}

	//�A�����b�N���Ă�
	g_Vertex->Unlock();//�o�b�t�@���� GPU�ɂ��邩�炢��Ȃ��ƕԂ�

	//�C���f�b�N�X�o�b�t�@
	pDevice->CreateIndexBuffer(sizeof(WORD) * ((((x + 1) * 2)*y) + (y - 1) * 2), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &g_index, NULL);
	WORD *pindex;
	g_index->Lock(0, 0, (void**)&pindex, D3DLOCK_DISCARD);
	t = 0;
	//�C���f�b�N�X�����(�k�ފ܂�)
	int index = 0;
	index = (((x + 1) * 2)*y) + (y - 1) * 2;

	//pindex�̒��g�����邽�߂�a�𓯂����錾
	int a[(((40 + 1) * 2)*40) + ( 40- 1) * 2];

	for (int i = 0; i < index; i += 2)
	{
		pindex[i] = t;//����
		a[i] = t; // pindex�̒��g�����邽��a�ɓ���Ă�
		pindex[i + 1] = x + 1 + t;//����

		a[i + 1] = x + 1 + t;//����
		//�k�ރ|���S��
		if (t % (x + 1) == x)
		{
			pindex[i + 2] = x + 1 + t;//���̏I���
			a[i + 3] = x + 1 + t;//���̏I���

			pindex[i + 3] = t + 1;//���̍s�̍ŏ�
			a[i + 4] = t + 1;//���̍s�̍ŏ�
			i += 2;

		}
		//t
		t++;
	}

	//pv�̓|�C���^������\���̂̐錾�͂���Ȃ�
	//position�̓���
	//pindex[index�̐�]�@=�@�u���_�̐��v
	//memcpy(pv, g_mesh_Vertex, sizeof(g_mesh_Vertex));
	g_index->Unlock();

	kannri = Texture_SetLoadFile("Asset/Texture/background.png", 842, 545);
	//kannri = Texture_SetLoadFile("", 842, 545);
	Texture_Load();
}

void MeshUninit()
{

}

void MeshUpdate()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = MyDirect3D_GetDevice();

	g_rad += 0.1f;
	g_angle = sin(g_rad);

	int m = 0;
	float yoko = 0;
	float tate = 0;
	yoko = 1 / (float)g_kazu_x;
	tate = 1 / (float)g_kazu_y;
	float takasa = 0;

	scrool = g_rad * 0.03;

	//���ꂪ�Ȃ���g_vertex��NUll�ɂȂ�
	pDevice->CreateVertexBuffer(sizeof(MeshVertex) * g_vertex_counter, D3DUSAGE_WRITEONLY, FVF_MESH, D3DPOOL_MANAGED, &g_Vertex, NULL);//���_
	//Lock = �ق��̃t�@�C����������Ȃ��悤�ɂ���

	//���b�N����g_Vertex
	g_Vertex->Lock(0, 0, (void**)&pv, D3DLOCK_DISCARD);

	for (int i = 0; i < g_kazu_y + 1; i++)
	{
		for (int t = 0; t < g_kazu_x + 1; t++)
		{
			//���_������Ă���

			//�����W�@���@�ő�̍������@������ �U�� �����ԁi�p�x�j
			//takasa = 0.01 * distance[m] * 3 * g_angle;
			takasa = /*(0.01 * distance[m] * 3 * g_angle)*/ +sinf((distance[m] * 0.1) - g_rad)*0.1;//�g�̑������傫��

			pv[m].position = D3DXVECTOR3(-g_kazu_x / 2 + t, 0.0 + takasa, -g_kazu_y / 2 + i); //�����@index0�@�@
			pv[m].color = D3DCOLOR_RGBA(255, 255, 255, 255);
			pv[m].uv = D3DXVECTOR2(t * yoko, (g_kazu_y - i) * tate - scrool);//�_�u�����b�v

			m++;
		}
	}
	g_Vertex->Unlock();

}

void MeshDraw()
{

	LPDIRECT3DDEVICE9 pDevice;
	pDevice = MyDirect3D_GetDevice();
	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//----------------------------------------
	//���[���h�ϊ�
	//----------------------------------------
	D3DXMATRIX mtxWorld, mtxRotation, mtxTranslation;

	//�������Ȃ��s��
	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixScaling(&mtxWorld, 0.5f, 1.0f, 0.5f);

	Camera_Update();

	pDevice->SetFVF(FVF_MESH);
	pDevice->SetTexture(0, Texture_GetTexture(kannri));

	D3DXMatrixTranslation(&mtxTranslation, 0.0f, 0.5f, 0.0f);
	//mtxWorld = mtxTranslation;


	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);// �f�o�C�X��worl�ϊ��s���ݒ�@�������������ꂽ��ɏ���
	pDevice->SetRenderState(D3DRS_LIGHTING, false); //���C�g�g�p

//--------------------------------------------------
// Scene��end�̂Ƃ�
//--------------------------------------------------
	pDevice->SetStreamSource(0, g_Vertex, 0, sizeof(MeshVertex));        // ���_�o�b�t�@�̎w��
	pDevice->SetIndices(g_index);    // ���_�C���f�b�N�X�o�b�t�@�̎w��

	pDevice->SetFVF(FVF_MESH);
	pDevice->SetTexture(0, Texture_GetTexture(kannri));

	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_vertex_counter, 0, g_primitive_counter);//���_�̐��@�A�@�O�A�O�p�̐�
}


