#include<d3dx9.h>
#include"mydirect3d.h"
void Set_Light()
{
	LPDIRECT3DDEVICE9 pDevice = MyDirect3D_GetDevice();  //Device�̎擾

	D3DLIGHT9 light = {};
	light.Type = D3DLIGHT_DIRECTIONAL;
	D3DXVECTOR3 vecDirLight(0.0f,0.0f,1.0f);
	D3DXVec3Normalize(&vecDirLight, &vecDirLight);
	light.Direction = vecDirLight;

	//���C�g�̐F
	light.Diffuse.r = 0.0f;  
	light.Diffuse.g = 0.0f;
	light.Diffuse.b = 0.0f;
	light.Diffuse.a = 1.0f;


	pDevice->SetLight(0, &light);  //0�ԃX���b�g�Ƀ��C�g���Z�b�g
	pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,D3DMCS_COLOR1);  //�}�e���A���͒��_�J���[
	pDevice->LightEnable(0, TRUE); //0�ԃX���b�g��ON

	//�A���r�G���g���C�g                   D3DXCOLOR(0.1f,0.0f,0.1f)
	pDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_RGBA(255,255,255,255));
	pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	//�f�B���N�V���i���}�e���A�����C�g + �A���r�G���g�}�e���A�� * ���C�g = 1
}