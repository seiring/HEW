#include<d3dx9.h>
#include"mydirect3d.h"
#include"common.h"
#include"input.h"

#define N (0.1f)
#define F (100.0f)

static D3DXVECTOR3 g_vecFront(0.0f, 0.0f, 1.0f);  //�O���x�N�g��
static D3DXVECTOR3 g_vecRight(1.0f, 0.0f, 0.0f);  //�E���x�N�g��
static D3DXVECTOR3 g_vecUp(0.0f, 1.0f, 0.0f);     //����x�N�g��
static D3DXVECTOR3 g_position(0.0f, 0.0f, 0.0f);  //�J�����|�W�V����
static float g_Fov = 1.0f;                        //��p(��������60��)
static float g_moveSpeed = 0.0f;
static float g_rotationSpeed = 0.0f;
static float l = 5.0f;

void Camera_Init(void)
{
	g_vecFront = D3DXVECTOR3(0.0f, -0.8f, 1.0f);
	g_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&g_vecUp, &g_vecFront, &g_vecRight);  //�O�ς̊֐�
	D3DXVec3Normalize(&g_vecFront, &g_vecFront);        //�P�ʃx�N�g���ɕϊ�
	D3DXVec3Normalize(&g_vecUp, &g_vecUp);
	g_position = D3DXVECTOR3(0.0f, 3.0f, -5.0f);
	g_Fov = 1.0f;
	g_moveSpeed = 0.1f;
	g_rotationSpeed = 0.03;
}

void Camera_Update(void)
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();
	D3DXVECTOR3 vecDir(0.0f, 0.0f, 0.0f);

	if (Keyboard_IsPress(DIK_W) && Keyboard_IsPress(DIK_LSHIFT))
	{
		vecDir += g_vecFront;

	}

	if (Keyboard_IsPress(DIK_S) && Keyboard_IsPress(DIK_LSHIFT))
	{
		vecDir -= g_vecFront;
	}

	if (Keyboard_IsPress(DIK_D) && Keyboard_IsPress(DIK_LSHIFT))
	{
		vecDir += g_vecRight;
	}

	if (Keyboard_IsPress(DIK_A) && Keyboard_IsPress(DIK_LSHIFT))
	{
		vecDir -= g_vecRight;
	}

	if (Keyboard_IsPress(DIK_UP) && Keyboard_IsPress(DIK_LSHIFT))
	{
		vecDir += g_vecUp;
	}

	if (Keyboard_IsPress(DIK_DOWN) && Keyboard_IsPress(DIK_LSHIFT))
	{
		vecDir -= g_vecUp;
	}

	D3DXVec3Normalize(&vecDir, &vecDir);  //���K��
	g_position += vecDir * g_moveSpeed;  //�����~�ړ��X�s�[�h

	if (Keyboard_IsPress(DIK_RIGHT) && Keyboard_IsPress(DIK_LSHIFT))
	{
		D3DXMATRIX mtxRotation;
		D3DXMatrixRotationAxis(&mtxRotation, &g_vecUp, g_rotationSpeed);
		//���ǂ̃x�N�g�������ɉ�]���邩
		D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRotation); //�x�N�g���ϊ�
		D3DXVec3TransformNormal(&g_vecRight, &g_vecRight, &mtxRotation);
		D3DXVec3Normalize(&g_vecFront, &g_vecFront);
		D3DXVec3Normalize(&g_vecRight, &g_vecRight);
	}


	if (Keyboard_IsPress(DIK_LEFT) && Keyboard_IsPress(DIK_LSHIFT))
	{
		D3DXMATRIX mtxRotation;
		D3DXMatrixRotationAxis(&mtxRotation, &g_vecUp, -g_rotationSpeed);
		//���ǂ̃x�N�g�������ɉ�]���邩
		D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRotation); //�x�N�g���ϊ�
		D3DXVec3TransformNormal(&g_vecRight, &g_vecRight, &mtxRotation);
		D3DXVec3Normalize(&g_vecFront, &g_vecFront);
		D3DXVec3Normalize(&g_vecRight, &g_vecRight);
	}
	


	//�r���[�ϊ��s��
	D3DXMATRIX mtxView;
	D3DXVECTOR3 eye = g_position;
	D3DXVECTOR3 at = g_position + g_vecFront * l; //����ꏊ
	D3DXVECTOR3 up = g_vecUp;//����x�N�g��
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up); //�r���[�s������
	pDevice->SetTransform(D3DTS_VIEW, &mtxView); //�f�o�C�X�Ƀr���[�Ԋҍs���ݒ�

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE); //���C�g�g�por���g�p

	//�v���W�F�N�V����(�p�[�X�y�N�e�B�u)�s��
	D3DXMATRIX mtxProjection;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, N, F);
	//                                        ���W�A���p�ŉ�p�ݒ�     �@�@�@�A�X�y�N�g��             near  far
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

/*
��]�i�E�E��)
D3DMatrixRotationY
3�{���ׂč��W�ϊ�

���s�ړ�
D3DXVECTOR3 vecMoveFront =g_vecFront;
vecMoveFront.y=0.0f;
D3DXVec3Normalize(&vecMoveFront,&vecMoveFront);
g_position +=vecMoveFront * g_moveSpeed;
*/