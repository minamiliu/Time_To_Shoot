/*******************************************************************************
*
* �^�C�g��:		�ۉe����
* �v���O������:	shadow.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SHADOW		"data/TEXTURE/shadow000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	SHADOW_SIZE_X		(50.0f)							// �e�̕�
#define	SHADOW_SIZE_Z		(50.0f)							// �e�̍���

#define	MAX_SHADOW			(256)							// �e�ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXMATRIX mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXVECTOR3 pos;			// �ʒu
	D3DXVECTOR3	rot;			// ��]
	bool bUse;					// �g�p���Ă��邩�ǂ���
} SHADOW;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;	// ���_�o�b�t�@�ւ̃|�C���^

SHADOW g_aShadow[MAX_SHADOW];							// �e���[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �e���[�N�̏�����
	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].bUse = false;
		g_aShadow[nCntShadow].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, TEXTURE_SHADOW, &g_pTextureShadow);

	// ���_���̍쐬
	MakeVertexShadow(pDevice);
	
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitShadow(void)
{
	if(g_pTextureShadow != NULL)
	{// �e�N�X�`���̊J��
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	if(g_pVtxBuffShadow != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].bUse = false;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateShadow(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	SHADOW *pShadow;

	// ���Z����
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);	// ���� = �]����(DEST) - �]����(SRC)
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �e�\���̂̃|�C���^�ɉe���[�N�̐擪�A�h���X����
	pShadow = &g_aShadow[0];
	
	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pShadow++)
	{
		if(pShadow->bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&pShadow->mtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll( &mtxRot, pShadow->rot.y, pShadow->rot.x, pShadow->rot.z);
			D3DXMatrixMultiply( &pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxRot, pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);
			D3DXMatrixMultiply( &pShadow->mtxWorld, &pShadow->mtxWorld, &mtxRot);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform( D3DTS_WORLD, &pShadow->mtxWorld);

			// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			// �|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntShadow * 4,			//���[�h����ŏ��̒��_�C���f�b�N�X
				2						//�|���S���̐�
			);	
		}
	}

	// �ʏ�u�����h 
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);	// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
																				
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
HRESULT MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * MAX_SHADOW,	//���_�f�[�^�̃o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffShadow->Lock( 0, 0, (void**)&pVtx, 0);

		for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++, pVtx += 4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(0.0f - (SHADOW_SIZE_X/2), 0.0f, 0.0f + (SHADOW_SIZE_Z/2));
			pVtx[1].vtx = D3DXVECTOR3(0.0f + (SHADOW_SIZE_X/2), 0.0f, 0.0f + (SHADOW_SIZE_Z/2));
			pVtx[2].vtx = D3DXVECTOR3(0.0f - (SHADOW_SIZE_X/2), 0.0f, 0.0f - (SHADOW_SIZE_Z/2));
			pVtx[3].vtx = D3DXVECTOR3(0.0f + (SHADOW_SIZE_X/2), 0.0f, 0.0f - (SHADOW_SIZE_Z/2));

			// �@���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
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
		g_pVtxBuffShadow->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexShadow(int nIdxShadow, float fSizeX, float fSizeZ)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffShadow->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtx���C���f�b�N�X���Y����
		pVtx += nIdxShadow * 4;

		// ���_���W�̐ݒ�i������p���Đݒ�j
		pVtx[0].vtx = D3DXVECTOR3(0.0f - (fSizeX/2), 0.0f, 0.0f + (fSizeZ/2));
		pVtx[1].vtx = D3DXVECTOR3(0.0f + (fSizeX/2), 0.0f, 0.0f + (fSizeZ/2));
		pVtx[2].vtx = D3DXVECTOR3(0.0f - (fSizeX/2), 0.0f, 0.0f - (fSizeZ/2));
		pVtx[3].vtx = D3DXVECTOR3(0.0f + (fSizeX/2), 0.0f, 0.0f - (fSizeZ/2));

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorShadow(int nIdxShadow, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBuffShadow->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtx���C���f�b�N�X���Y����
		pVtx += nIdxShadow * 4;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// ���_�f�[�^���A�����b�N����
		g_pVtxBuffShadow->Unlock();
	}
}

//=============================================================================
// �e�̍쐬
//=============================================================================
int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	int nIdxShadow = -1;

	for(int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if(!g_aShadow[nCntShadow].bUse)
		{
			// �e�̈ʒu�A��]�p��ݒ�
			D3DXVECTOR3 tPos = pos;
			tPos.y = 0.1f;
			g_aShadow[nCntShadow].pos = tPos;

			// �e���[�N���g�p��ԂɕύX
			g_aShadow[nCntShadow].bUse = true;

			// ���_���W�̐ݒ�֐����Ăяo��
			SetVertexShadow( nCntShadow, fSizeX, fSizeZ);

			// �e�̔ԍ���nIdxShadow�ɐݒ�
			nIdxShadow = nCntShadow;
			break;
		}
	}

	// nIdxShadow��Ԃ�
	return nIdxShadow;
}

//=============================================================================
// �e�̔j��
//=============================================================================
void ReleaseShadow(int nIdxShadow)
{
	//�w�肳�ꂽ�e���e���[�N�̌��Ȃ����ǂ���
	if(nIdxShadow < MAX_SHADOW)
	{
		// �e���[�N�𖢎g�p��Ԃɐݒ�
		g_aShadow[nIdxShadow].bUse = false;
	}
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	// �w�肳�ꂽ�e���[�N�Ɉʒu��ݒ�
	D3DXVECTOR3 tPos = pos;
	tPos.y = 0.1f;
	g_aShadow[nIdxShadow].pos = tPos;
}

