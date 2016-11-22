/*******************************************************************************
*
* �^�C�g��:		���C�e�B���O����
* �v���O������:	light.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "light.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_LIGHT		(3)												// ���C�g�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DLIGHT9 g_aLight[NUM_LIGHT];											// ���C�g���

//=============================================================================
// ���C�g�̏���������
//=============================================================================
HRESULT InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	D3DXVECTOR3 vecDir;

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory( &g_aLight[0], sizeof(D3DLIGHT9));
	
	// ���C�g0�̃^�C�v�̐ݒ�
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;
	
	// ���C�g0�̊g�U���̐ݒ�
	g_aLight[0].Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);
	g_aLight[0].Ambient = D3DXCOLOR( 0.2f, 0.2f, 0.2f, 0.2f);
	
	// ���C�g0�̕����̐ݒ�
	vecDir = D3DXVECTOR3( 0.2f, -0.6f, 0.8f);
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[0].Direction, &vecDir);
	
	// ���C�g0�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight( 0, &g_aLight[0]);

	// ���C�g0���g�p�g�p��Ԃ�
	pDevice->LightEnable( 0, TRUE);

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory( &g_aLight[1], sizeof(D3DLIGHT9));

	// ���C�g1�̃^�C�v�̐ݒ�
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// ���C�g1�̊g�U���̐ݒ�
	g_aLight[1].Diffuse = D3DXCOLOR( 0.75f, 0.75f, 0.75f, 1.0f);
	g_aLight[1].Ambient = D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.0f);

    // ���C�g1�̕����̐ݒ�
	vecDir = D3DXVECTOR3( -0.2f, -0.3f, -0.5f);
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[1].Direction, &vecDir);

	// ���C�g1�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight( 1, &g_aLight[1]);

	// ���C�g1�̂��g�p�g�p��Ԃ�
	pDevice->LightEnable( 1, TRUE);

	// D3DLIGHT9�\���̂�0�ŃN���A����
	ZeroMemory( &g_aLight[2], sizeof(D3DLIGHT9));
	
	// ���C�g2�̃^�C�v�̐ݒ�
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;
	
	// ���C�g2�̊g�U���̐ݒ�
	g_aLight[2].Diffuse = D3DXCOLOR( 0.15f, 0.15f, 0.15f, 1.0f);
	g_aLight[2].Ambient = D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.0f);
	
	// ���C�g2�̕����̐ݒ�
	vecDir = D3DXVECTOR3( 0.8f, 0.1f, 0.5f);
	D3DXVec3Normalize( (D3DXVECTOR3*)&g_aLight[2].Direction, &vecDir);
	
	// ���C�g2�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetLight( 2, &g_aLight[2]);

	// ���C�g2���g�p�g�p��Ԃ�
	pDevice->LightEnable( 2, TRUE);

	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	return S_OK;
}

//=============================================================================
// ���C�g�̏I������
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ���C�g�̍X�V����
//=============================================================================
void UpdateLight(void)
{
}

