/*******************************************************************************
*
* �^�C�g��:		���b�V���t�B�[���h����
* �v���O������:	meshfield.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "meshfield.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define TEXTURE_FIELD	"data/TEXTURE/field002.jpg"						// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureField;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;	// ���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffField;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

D3DXMATRIX g_mtxWorldField;					// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posField;						// �ʒu
D3DXVECTOR3 g_rotField;						// ����

int g_nNumBlockX, g_nNumBlockZ;				// �u���b�N��
int g_nNumVertex;							// �����_��
int g_nNumVertexIndex;						// ���C���f�b�N�X��
int g_nNumPolygon;							// ���|���S����
float g_fSizeBlockX, g_fSizeBlockZ;			// �u���b�N�T�C�Y


//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E��]�E�X�P�[���̏����ݒ�

	g_posField = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_rotField = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

	g_nNumBlockX = nNumBlockX;
	g_nNumBlockZ = nNumBlockZ;
	g_fSizeBlockX = fSizeBlockX;
	g_fSizeBlockZ = fSizeBlockZ;

	g_nNumVertex = (nNumBlockX+1) * (nNumBlockZ+1);
	g_nNumVertexIndex = (nNumBlockX + 1)*2*nNumBlockZ + (nNumBlockZ - 1)*2;
	g_nNumPolygon = nNumBlockX*nNumBlockZ*2 + (nNumBlockZ - 1)*4;

	g_pD3DTextureField = NULL;	
	g_pD3DVtxBuffField = NULL;
	g_pD3DIdxBuffField = NULL;
	
	// ���_���̍쐬
	MakeVertexField(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureField);


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
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
void UpdateMeshField(void)
{

}

//=============================================================================
// �`�揉��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
		
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_mtxWorldField);
	
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxRot);
	
	// �ړ��𔽉f
	D3DXMatrixTranslation( &mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply( &g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);
	
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform( D3DTS_WORLD, &g_mtxWorldField);

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(g_pD3DIdxBuffField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureField);
	
	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,						//�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
		0,						//�ŏ��̒��_�C���f�b�N�X
		g_nNumVertex,			//���_�̐�
		0,						//�ǂݎ����J�n����ʒu
		g_nNumPolygon			//�|���S���̐�
	);



}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * g_nNumVertex,	//���_�f�[�^�̃o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffField,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * g_nNumVertexIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pD3DIdxBuffField,
		NULL)))
	{
		return E_FAIL;
	}

	
	{//���_�o�b�t�@�̒��g�𖄂߂�

		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0);

		for(int nCntZ = 0; nCntZ < g_nNumBlockZ+1; nCntZ++)
		{
			for(int nCntX = 0; nCntX < g_nNumBlockX+1; nCntX++)
			{
				pVtx[0].vtx = D3DXVECTOR3( -g_fSizeBlockX*g_nNumBlockX/2 + nCntX*g_fSizeBlockX, 0.0f, g_fSizeBlockZ*g_nNumBlockZ/2 - nCntZ*g_fSizeBlockZ);
				pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
				pVtx[0].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
				pVtx[0].tex = D3DXVECTOR2( 1.0f * nCntX, 1.0f * nCntZ );
				pVtx++;
			}
		}


		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffField->Unlock();	
	}


	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		WORD *pIdx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DIdxBuffField->Lock( 0, 0, (void**)&pIdx, 0);

		for(int nCntZ = 0; nCntZ < g_nNumBlockZ; nCntZ++)
		{
			if(nCntZ > 0)
			{
				pIdx[0] = (g_nNumBlockX + 1) * (nCntZ + 1);
				pIdx++;
			}
			for(int nCntX = 0; nCntX < g_nNumBlockX+1; nCntX++, pIdx+=2)
			{
				//���_���W�̐ݒ�
				pIdx[0] = (g_nNumBlockX + 1) * (nCntZ + 1) + nCntX; 
				pIdx[1] = (g_nNumBlockX + 1) * (nCntZ) + nCntX;
			}
			if(nCntZ < g_nNumBlockZ - 1)
			{
				*pIdx = *(pIdx-1);
				pIdx++;
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DIdxBuffField->Unlock();	
	}

	return S_OK;
}

