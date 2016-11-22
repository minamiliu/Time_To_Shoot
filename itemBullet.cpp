/*******************************************************************************
*
* �^�C�g��:		�A�C�e��(�e)�̏���
* �v���O������:	itemBullet.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "itemBullet.h"
#include "debugproc.h"
#include "sound.h"
#include "input.h"
#include "camera.h"
#include "meshwall.h"
#include "hitCheck.h"
#include "sound.h"
#include "player.h"
#include "playerBulletNum.h"
#include "shadow.h"



//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_BULLET		"data/MODEL/item_bullet.x"					// �ǂݍ��ރe�N�X�`���t�@�C����

#define VALUE_MOVE		(0.0f)									// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.01f) 						// ��]��

#define MODEL_POS_X	(0)
#define MODEL_POS_Y	(100)
#define MODEL_POS_Z	(0)



//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ITEM_BULLET g_ItemBullet[MAX_ITEM_BULLET];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitItemBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_ItemBullet[nCntItem].pos = D3DXVECTOR3( MODEL_POS_X, MODEL_POS_Y, MODEL_POS_Z);
		g_ItemBullet[nCntItem].rot = D3DXVECTOR3( D3DXToRadian(30.0f), 0.0f, 0.0f);
		g_ItemBullet[nCntItem].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
		g_ItemBullet[nCntItem].bUse = false;

		// ���f���Ɋւ���ϐ��̏�����							
		g_ItemBullet[nCntItem].pD3DTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
		g_ItemBullet[nCntItem].pD3DXMesh = NULL;		// ���b�V�����ւ̃|�C���^
		g_ItemBullet[nCntItem].pD3DXBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
		g_ItemBullet[nCntItem].nNumMat = 0;				// �}�e���A�����̐�

		// X�t�@�C���̓ǂݍ���
		if(FAILED(D3DXLoadMeshFromX(
			MODEL_BULLET,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,			// ���b�V���̍쐬�I�v�V�������w��
			pDevice,					// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,						// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&g_ItemBullet[nCntItem].pD3DXBuffMat,		// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,						// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&g_ItemBullet[nCntItem].nNumMat,			// D3DXMATERIAL�\���̂̐�
			&g_ItemBullet[nCntItem].pD3DXMesh			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
			)))
		{
			return E_FAIL;
		}

	}
													
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitItemBullet(void)
{
	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		// ���b�V���̊J��
		if(g_ItemBullet[nCntItem].pD3DXMesh != NULL)
		{
			g_ItemBullet[nCntItem].pD3DXMesh->Release();
			g_ItemBullet[nCntItem].pD3DXMesh = NULL;
		}
		// �}�e���A���̊J��
		if(g_ItemBullet[nCntItem].pD3DXBuffMat != NULL)
		{
			g_ItemBullet[nCntItem].pD3DXBuffMat->Release();
			g_ItemBullet[nCntItem].pD3DXBuffMat = NULL;
		}
		g_ItemBullet[nCntItem].bUse = false;
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItemBullet( void)
{
	CAMERA *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();

	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		if(g_ItemBullet[nCntItem].bUse)
		{
			//�A�C�e������]������
			g_ItemBullet[nCntItem].rot.y += VALUE_ROTATE;

			if( HitCheckBall( g_ItemBullet[nCntItem].pos, 10.0f, pCamera->posV, 30.0f))
			{
				pPlayer->nBullet = 3;
				ChangeNumBullet( 0, pPlayer->nBullet);
				g_ItemBullet[nCntItem].bUse = false;
				ReleaseShadow( g_ItemBullet[nCntItem].nIdxShadow);

				PlaySound(SOUND_LABEL_SE_RELOAD);
			}
		}
	}



}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItemBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		if(g_ItemBullet[nCntItem].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_ItemBullet[nCntItem].mtxWorld);
	
			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_ItemBullet[nCntItem].scl.x, g_ItemBullet[nCntItem].scl.y, g_ItemBullet[nCntItem].scl.z);
			D3DXMatrixMultiply( &g_ItemBullet[nCntItem].mtxWorld, &g_ItemBullet[nCntItem].mtxWorld, &mtxScl);
	
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_ItemBullet[nCntItem].rot.y, g_ItemBullet[nCntItem].rot.x, g_ItemBullet[nCntItem].rot.z);
			D3DXMatrixMultiply( &g_ItemBullet[nCntItem].mtxWorld, &g_ItemBullet[nCntItem].mtxWorld, &mtxRot);
	
			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTranslate, g_ItemBullet[nCntItem].pos.x, g_ItemBullet[nCntItem].pos.y, g_ItemBullet[nCntItem].pos.z);
			D3DXMatrixMultiply( &g_ItemBullet[nCntItem].mtxWorld, &g_ItemBullet[nCntItem].mtxWorld, &mtxTranslate);
	
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform( D3DTS_WORLD, &g_ItemBullet[nCntItem].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);
	
			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL *)g_ItemBullet[nCntItem].pD3DXBuffMat->GetBufferPointer();
			for(int nCntMat = 0; nCntMat < (int)g_ItemBullet[nCntItem].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture( 0, g_ItemBullet[nCntItem].pD3DTexture);
				// �`��
				g_ItemBullet[nCntItem].pD3DXMesh->DrawSubset( nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial( &matDef);		
		}
	}
}

//=============================================================================
// �A�C�e���̐ݒu
//=============================================================================
void SetItemBullet( D3DXVECTOR3 pos)
{
	for(int nCntItem = 0; nCntItem < MAX_ITEM_BULLET; nCntItem++)
	{
		if(!g_ItemBullet[nCntItem].bUse) 
		{
			//�ʒu��ݒ�
			pos.y = MODEL_POS_Y;
			g_ItemBullet[nCntItem].pos = pos;
			g_ItemBullet[nCntItem].bUse = true;
		
			//shadow
			g_ItemBullet[nCntItem].nIdxShadow = CreateShadow( g_ItemBullet[nCntItem].pos, 10.0f, 10.0f);
			break;
		}
	}	
}


