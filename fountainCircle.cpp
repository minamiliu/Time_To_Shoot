/*******************************************************************************
*
* �^�C�g��:		�����̃��f���̕\������
* �v���O������:	fountainCircle.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "fountainCircle.h"
#include "debugproc.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_PLANE		"data/MODEL/fountain.x"				// �ǂݍ���X�t�@�C����

#define VALUE_MOVE		(5.0f)								// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.1f) 					// ��]��

#define MODEL_POS_X	(-700)
#define MODEL_POS_Y	(5)
#define MODEL_POS_Z	(200)

#define MODEL_SPHERE (20)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
FOUNTAIN_CIRCLE g_FountainCircle;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitFountainCircle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_FountainCircle.pos = D3DXVECTOR3( MODEL_POS_X, MODEL_POS_Y, MODEL_POS_Z);
	g_FountainCircle.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_FountainCircle.scl = D3DXVECTOR3( 10.0f, 5.0f, 10.0f);
	g_FountainCircle.move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
	g_FountainCircle.rotAngle = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_FountainCircle.rotTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_FountainCircle.fRadius = 150.0f;
	g_FountainCircle.bUse = true;
	
	// ���f���Ɋւ���ϐ��̏�����
	g_FountainCircle.pD3DXMesh = NULL;			// ���b�V�����ւ̃|�C���^
	g_FountainCircle.pD3DXBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
	g_FountainCircle.nNumMat = 0;				// �}�e���A�����̐�

	// X�t�@�C���̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(
		MODEL_PLANE,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,			// ���b�V���̍쐬�I�v�V�������w��
		pDevice,					// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,						// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_FountainCircle.pD3DXBuffMat,		// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,						// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_FountainCircle.nNumMat,			// D3DXMATERIAL�\���̂̐�
		&g_FountainCircle.pD3DXMesh			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���ւ̃|�C���^�̏�����
	for(int nCntMat = 0; nCntMat < MAX_D3D_MAT; nCntMat++)
	{
		g_FountainCircle.pD3DTexture[nCntMat] = NULL;		// �e�N�X�`���ւ̃|�C���^
	}

	// �}�e���A�����ɑ΂���|�C���^���擾
	D3DXMATERIAL *pD3DXMat = (D3DXMATERIAL *)g_FountainCircle.pD3DXBuffMat->GetBufferPointer();
	if((int)g_FountainCircle.nNumMat <= MAX_D3D_MAT)
	{
		for(int nCntMat = 0; nCntMat < (int)g_FountainCircle.nNumMat; nCntMat++)
		{
			// �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile( pDevice, pD3DXMat[nCntMat].pTextureFilename, &g_FountainCircle.pD3DTexture[nCntMat]);
		}
	}


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFountainCircle(void)
{
	// �e�N�X�`���̊J��
	for(int nCntMat = 0; nCntMat < MAX_D3D_MAT; nCntMat++)
	{
		if(g_FountainCircle.pD3DTexture[nCntMat] != NULL)
		{
			g_FountainCircle.pD3DTexture[nCntMat]->Release();
			g_FountainCircle.pD3DTexture[nCntMat] = NULL;
		}
	}

	// ���b�V���̊J��
	if(g_FountainCircle.pD3DXMesh != NULL)
	{
		g_FountainCircle.pD3DXMesh->Release();
		g_FountainCircle.pD3DXMesh = NULL;
	}
	// �}�e���A���̊J��
	if(g_FountainCircle.pD3DXBuffMat != NULL)
	{
		g_FountainCircle.pD3DXBuffMat->Release();
		g_FountainCircle.pD3DXBuffMat = NULL;
	}

	g_FountainCircle.bUse = false;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFountainCircle(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFountainCircle(void)
{
	if(!g_FountainCircle.bUse) return ;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_FountainCircle.mtxWorld);
	
	// �X�P�[���𔽉f
	D3DXMatrixScaling( &mtxScl, g_FountainCircle.scl.x, g_FountainCircle.scl.y, g_FountainCircle.scl.z);
	D3DXMatrixMultiply( &g_FountainCircle.mtxWorld, &g_FountainCircle.mtxWorld, &mtxScl);
	
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_FountainCircle.rot.y, g_FountainCircle.rot.x, g_FountainCircle.rot.z);
	D3DXMatrixMultiply( &g_FountainCircle.mtxWorld, &g_FountainCircle.mtxWorld, &mtxRot);
	
	// �ړ��𔽉f
	D3DXMatrixTranslation( &mtxTranslate, g_FountainCircle.pos.x, g_FountainCircle.pos.y, g_FountainCircle.pos.z);
	D3DXMatrixMultiply( &g_FountainCircle.mtxWorld, &g_FountainCircle.mtxWorld, &mtxTranslate);
	
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform( D3DTS_WORLD, &g_FountainCircle.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL *)g_FountainCircle.pD3DXBuffMat->GetBufferPointer();
	
	for(int nCntMat = 0; nCntMat < (int)g_FountainCircle.nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture( 0, g_FountainCircle.pD3DTexture[nCntMat]);

		// �`��
		g_FountainCircle.pD3DXMesh->DrawSubset( nCntMat);

	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial( &matDef);


}

//=============================================================================
// ���f���̎擾
//=============================================================================
FOUNTAIN_CIRCLE *GetFountainCircle(void)
{
	return &g_FountainCircle;
}


