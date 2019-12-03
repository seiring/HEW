#include<d3dx9.h>
#include"mydirect3d.h"
#include"common.h"
#include"input.h"

#define N (0.1f)
#define F (100.0f)

static D3DXVECTOR3 g_vecFront(0.0f, 0.0f, 1.0f);  //前方ベクトル
static D3DXVECTOR3 g_vecRight(1.0f, 0.0f, 0.0f);  //右方ベクトル
static D3DXVECTOR3 g_vecUp(0.0f, 1.0f, 0.0f);     //上方ベクトル
static D3DXVECTOR3 g_position(0.0f, 0.0f, 0.0f);  //カメラポジション
static float g_Fov = 1.0f;                        //画角(だいたい60°)
static float g_moveSpeed = 0.0f;
static float g_rotationSpeed = 0.0f;
static float l = 5.0f;

void Camera_Init(void)
{
	g_vecFront = D3DXVECTOR3(0.0f, -0.8f, 1.0f);
	g_vecRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3Cross(&g_vecUp, &g_vecFront, &g_vecRight);  //外積の関数
	D3DXVec3Normalize(&g_vecFront, &g_vecFront);        //単位ベクトルに変換
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

	D3DXVec3Normalize(&vecDir, &vecDir);  //正規化
	g_position += vecDir * g_moveSpeed;  //方向×移動スピード

	if (Keyboard_IsPress(DIK_RIGHT) && Keyboard_IsPress(DIK_LSHIFT))
	{
		D3DXMATRIX mtxRotation;
		D3DXMatrixRotationAxis(&mtxRotation, &g_vecUp, g_rotationSpeed);
		//↑どのベクトルを軸に回転するか
		D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRotation); //ベクトル変換
		D3DXVec3TransformNormal(&g_vecRight, &g_vecRight, &mtxRotation);
		D3DXVec3Normalize(&g_vecFront, &g_vecFront);
		D3DXVec3Normalize(&g_vecRight, &g_vecRight);
	}


	if (Keyboard_IsPress(DIK_LEFT) && Keyboard_IsPress(DIK_LSHIFT))
	{
		D3DXMATRIX mtxRotation;
		D3DXMatrixRotationAxis(&mtxRotation, &g_vecUp, -g_rotationSpeed);
		//↑どのベクトルを軸に回転するか
		D3DXVec3TransformNormal(&g_vecFront, &g_vecFront, &mtxRotation); //ベクトル変換
		D3DXVec3TransformNormal(&g_vecRight, &g_vecRight, &mtxRotation);
		D3DXVec3Normalize(&g_vecFront, &g_vecFront);
		D3DXVec3Normalize(&g_vecRight, &g_vecRight);
	}
	


	//ビュー変換行列
	D3DXMATRIX mtxView;
	D3DXVECTOR3 eye = g_position;
	D3DXVECTOR3 at = g_position + g_vecFront * l; //見る場所
	D3DXVECTOR3 up = g_vecUp;//上方ベクトル
	D3DXMatrixLookAtLH(&mtxView, &eye, &at, &up); //ビュー行列を作る
	pDevice->SetTransform(D3DTS_VIEW, &mtxView); //デバイスにビュー返還行列を設定

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE); //ライト使用or未使用

	//プロジェクション(パースペクティブ)行列
	D3DXMATRIX mtxProjection;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(60), (float)SCREEN_WIDTH / SCREEN_HEIGHT, N, F);
	//                                        ラジアン角で画角設定     　　　アスペクト比             near  far
	pDevice->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}

/*
回転（右・左)
D3DMatrixRotationY
3本すべて座標変換

平行移動
D3DXVECTOR3 vecMoveFront =g_vecFront;
vecMoveFront.y=0.0f;
D3DXVec3Normalize(&vecMoveFront,&vecMoveFront);
g_position +=vecMoveFront * g_moveSpeed;
*/