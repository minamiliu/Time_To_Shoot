/*******************************************************************************
* �^�C�g��:		�t�B�[���h����
* �v���O������:	field.cpp
* �쐬��:		TH�������@���
* �쐬��:		2015/10/20
********************************************************************************
* �X�V����:		- 2015/10/20	 ���
*					- V1.00		Initial Version
*******************************************************************************/
/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "field.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_FIELD	(9)
#define NUM_VERTEX	(4)
#define NUM_POLYGON (2)

#define TEXTURE_FIELD	"data/TEXTURE/field000.jpg"						// �ǂݍ��ރe�N�X�`���t�@�C����


#define VALUE_MOVE		(5.0f)											// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 								// ��]��

#define SIZE_X			(100.0f)										// �n�ʂ̃T�C�Y(X����)
#define SIZE_Z			(100.0f)										// �n�ʂ̃T�C�Y(Z����)
																		
																		

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureField;								// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;								// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX g_mtxWorldField;									// ���[���h�}�g���b�N�X

FIELD g_field[MAX_FIELD];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for(int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_field[nCntField].posField = D3DXVECTOR3( 0.0f + nCntField%3*SIZE_X, 0.0f, 0.0f + nCntField/3*SIZE_X);
		g_field[nCntField].rotField = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_field[nCntField].sclField = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);	
	}

	g_pD3DTextureField = NULL;	
	g_pD3DVtxBuffField = NULL;
	
	// ���_���̍쐬
	MakeVertexField(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureField);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitField(void)
{
	if(g_pD3DTextureField != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}

	if(g_pD3DVtxBuffField != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateField(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
		
	for(int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity( &g_mtxWorldField);

		// �X�P�[���𔽉f
		D3DXMatrixScaling( &mtxScl, g_field[nCntField].sclField.x, g_field[nCntField].sclField.y, g_field[nCntField].sclField.z);
		D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxScl);
	
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll( &mtxRot, g_field[nCntField].rotField.y, g_field[nCntField].rotField.x, g_field[nCntField].rotField.z);
		D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	
		// �ړ��𔽉f
		D3DXMatrixTranslation( &mtxTranslate, g_field[nCntField].posField.x, g_field[nCntField].posField.y, g_field[nCntField].posField.z);
		D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform( D3DTS_WORLD, &g_mtxWorldField);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureField);

	}
		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,						//���[�h����ŏ��̒��_�C���f�b�N�X
			NUM_POLYGON*MAX_FIELD	//�|���S���̐�
		);	


}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_VERTEX*MAX_FIELD,	//���_�f�[�^�̃o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffField,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntField = 0; nCntField < MAX_FIELD; nCntField++, pVtx+=4)
	{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_field[nCntField].posField.x - (SIZE_X/2), 0.0f, g_field[nCntField].posField.z + (SIZE_Z/2));
		pVtx[1].vtx = D3DXVECTOR3(g_field[nCntField].posField.x + (SIZE_X/2), 0.0f, g_field[nCntField].posField.z + (SIZE_Z/2));
		pVtx[2].vtx = D3DXVECTOR3(g_field[nCntField].posField.x - (SIZE_X/2), 0.0f, g_field[nCntField].posField.z - (SIZE_Z/2));
		pVtx[3].vtx = D3DXVECTOR3(g_field[nCntField].posField.x + (SIZE_X/2), 0.0f, g_field[nCntField].posField.z - (SIZE_Z/2));

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

		// ���ˌ��̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffField->Unlock();

	return S_OK;
}




