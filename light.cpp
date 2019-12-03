#include<d3dx9.h>
#include"mydirect3d.h"
void Set_Light()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();  //Deviceの取得

	D3DLIGHT9 light = {};
	light.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vecDirLight(0.0f,0.0f,1.0f);
	D3DXVec3Normalize(&vecDirLight, &vecDirLight);
	light.Direction = vecDirLight;

	//ライトの色
	light.Diffuse.r = 0.0f;  
	light.Diffuse.g = 0.0f;
	light.Diffuse.b = 0.0f;
	light.Diffuse.a = 1.0f;


	pDevice->SetLight(0, &light);  //0番スロットにライトをセット
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);  //マテリアルは頂点カラー
	pDevice->LightEnable(0, TRUE); //0番スロットをON

	//アンビエントライト                   D3DXCOLOR(0.1f,0.0f,0.1f)
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(255,255,255,255));
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	//ディレクショナルマテリアルライト + アンビエントマテリアル * ライト = 1
}