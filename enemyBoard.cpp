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
#include "enemyBoard.h"
#include "input.h"
#include "camera.h"
#include "hitCheck.h"
#include "debugproc.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX	(4)
#define NUM_POLYGON (2)

#define TEXTURE_FIELD	"data/TEXTURE/xxx.png"				// �ǂݍ��ރe�N�X�`���t�@�C����


#define VALUE_MOVE		(5.0f)								// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 					// ��]��

#define SIZE_X			(30.0f)								// �n�ʂ̃T�C�Y(X����)
#define SIZE_Y			(30.0f)								// �n�ʂ̃T�C�Y(Z����)
																		
																		

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexEnemyBoard(LPDIRECT3DDEVICE9 pDevice);
void ResetVertexEnemyBoard(int nIdxBoard);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMYBOARD g_aEnemyBoard[MAX_ENEMYBOARD];
LPDIRECT3DTEXTURE9		g_pD3DTextureEnemyBoard;								// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffEnemyBoard;								// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemyBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for(int nCntBoard = 0; nCntBoard < MAX_ENEMYBOARD; nCntBoard++)
	{
		g_aEnemyBoard[nCntBoard].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aEnemyBoard[nCntBoard].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aEnemyBoard[nCntBoard].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
		g_aEnemyBoard[nCntBoard].bUse = false;
	}

	g_pD3DVtxBuffEnemyBoard = NULL;

	
	// ���_���̍쐬
	MakeVertexEnemyBoard(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureEnemyBoard);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemyBoard(void)
{
	if(g_pD3DTextureEnemyBoard != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureEnemyBoard->Release();
		g_pD3DTextureEnemyBoard = NULL;
	}


	if(g_pD3DVtxBuffEnemyBoard != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffEnemyBoard->Release();
		g_pD3DVtxBuffEnemyBoard = NULL;
	}	


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemyBoard(void)
{
	CAMERA *pCamera = GetCamera();
	ENEMY *pEnemy = GetEnemy();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoard = 0; nCntBoard < MAX_ENEMYBOARD; nCntBoard++, pVtx+=4)
	{
		if(!g_aEnemyBoard[nCntBoard].bUse) continue;

		ResetVertexEnemyBoard(nCntBoard);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity( &g_aEnemyBoard[nCntBoard].mtxWorld);

		//���[���h�}�g���b�N�X�ɃJ�����̍s���]�u�s��Ƃ��Đݒ肷��
		g_aEnemyBoard[nCntBoard].mtxWorld._11 = pCamera->mtxView._11;
		g_aEnemyBoard[nCntBoard].mtxWorld._22 = pCamera->mtxView._22;//��������
		g_aEnemyBoard[nCntBoard].mtxWorld._33 = pCamera->mtxView._33;

		g_aEnemyBoard[nCntBoard].mtxWorld._12 = pCamera->mtxView._21;
		g_aEnemyBoard[nCntBoard].mtxWorld._13 = pCamera->mtxView._31;
		g_aEnemyBoard[nCntBoard].mtxWorld._23 = pCamera->mtxView._32;//��������

		g_aEnemyBoard[nCntBoard].mtxWorld._21 = pCamera->mtxView._12;//��������
		g_aEnemyBoard[nCntBoard].mtxWorld._31 = pCamera->mtxView._13;
		g_aEnemyBoard[nCntBoard].mtxWorld._32 = pCamera->mtxView._23;

		// �X�P�[���𔽉f
		D3DXMatrixScaling( &mtxScl, g_aEnemyBoard[nCntBoard].scl.x, g_aEnemyBoard[nCntBoard].scl.y, g_aEnemyBoard[nCntBoard].scl.z);
		D3DXMatrixMultiply( &g_aEnemyBoard[nCntBoard].mtxWorld, &g_aEnemyBoard[nCntBoard].mtxWorld, &mtxScl);
	
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aEnemyBoard[nCntBoard].rot.y, g_aEnemyBoard[nCntBoard].rot.x, g_aEnemyBoard[nCntBoard].rot.z);
		D3DXMatrixMultiply( &g_aEnemyBoard[nCntBoard].mtxWorld, &g_aEnemyBoard[nCntBoard].mtxWorld, &mtxRot);
	
		// �ړ��𔽉f
		D3DXMatrixTranslation( &mtxTranslate, g_aEnemyBoard[nCntBoard].pos.x, g_aEnemyBoard[nCntBoard].pos.y, g_aEnemyBoard[nCntBoard].pos.z);
		D3DXMatrixMultiply( &g_aEnemyBoard[nCntBoard].mtxWorld, &g_aEnemyBoard[nCntBoard].mtxWorld, &mtxTranslate);

		//���_�o�b�t�@�Ƀ��[���h�}�g���b�N�X���|����A���_�o�b�t�@���X�V�����
		D3DXVec3TransformCoord( &pVtx[0].vtx, &pVtx[0].vtx, &g_aEnemyBoard[nCntBoard].mtxWorld);
		D3DXVec3TransformCoord( &pVtx[1].vtx, &pVtx[1].vtx, &g_aEnemyBoard[nCntBoard].mtxWorld);
		D3DXVec3TransformCoord( &pVtx[2].vtx, &pVtx[2].vtx, &g_aEnemyBoard[nCntBoard].mtxWorld);
		D3DXVec3TransformCoord( &pVtx[3].vtx, &pVtx[3].vtx, &g_aEnemyBoard[nCntBoard].mtxWorld);
	}

	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffEnemyBoard->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemyBoard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	CAMERA *pCamera = GetCamera();

	// ���C�e�B���O���[�h��OFF
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE);
		
	for(int nCntBoard = 0; nCntBoard < MAX_ENEMYBOARD; nCntBoard++)
	{

		if(!g_aEnemyBoard[nCntBoard].bUse) continue;

		//// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity( &g_aEnemyBoard[nCntBoard].mtxWorld);
		
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform( D3DTS_WORLD, &g_aEnemyBoard[nCntBoard].mtxWorld);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, g_pD3DVtxBuffEnemyBoard, 0, sizeof(VERTEX_3D));
	
		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureEnemyBoard);

		//���e�X�g
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntBoard*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
			NUM_POLYGON				//�|���S���̐�
		);	
	}



	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexEnemyBoard(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_VERTEX*MAX_ENEMYBOARD,	//���_�f�[�^�̃o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffEnemyBoard,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoard = 0; nCntBoard < MAX_ENEMYBOARD; nCntBoard++, pVtx+=4)
	{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3( 0.0f - (SIZE_X/2), 0.0f + (SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3( 0.0f + (SIZE_X/2), 0.0f + (SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3( 0.0f - (SIZE_X/2), 0.0f - (SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3( 0.0f + (SIZE_X/2), 0.0f - (SIZE_Y/2), 0.0f);

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
	g_pD3DVtxBuffEnemyBoard->Unlock();

	return S_OK;
}

bool HitCheckEnemyBoard( D3DXVECTOR3 tNowPos, D3DXVECTOR3 tNextPos, int nIdxBoard)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

	// pVtx���C���f�b�N�X���Y����
	pVtx += nIdxBoard * 4;

	if( HitCheck( pVtx[0].vtx, pVtx[1].vtx, pVtx[2].vtx, tNowPos, tNextPos) )
	{
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEnemyBoard->Unlock();
		return true;
	}
	if( HitCheck( pVtx[1].vtx, pVtx[2].vtx, pVtx[3].vtx, tNowPos, tNextPos) )
	{
		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEnemyBoard->Unlock();
		return true;
	}


	// ���_�f�[�^���A�����b�N����
	g_pD3DVtxBuffEnemyBoard->Unlock();


	return false;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void ResetVertexEnemyBoard(int nIdxBoard)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtx���C���f�b�N�X���Y����
		pVtx += nIdxBoard * 4;

		// ���_���W�̐ݒ�i������p���Đݒ�j
		pVtx[0].vtx = D3DXVECTOR3(0.0f - (SIZE_X/2), 0.0f + (SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(0.0f + (SIZE_X/2), 0.0f + (SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f - (SIZE_X/2), 0.0f - (SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(0.0f + (SIZE_X/2), 0.0f - (SIZE_Y/2), 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEnemyBoard->Unlock();
	}
}

void SetVertexEnemyBoard(int nIdxBoard, float fSizeX, float fSizeZ)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffEnemyBoard->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtx���C���f�b�N�X���Y����
		pVtx += nIdxBoard * 4;

		// ���_���W�̐ݒ�i������p���Đݒ�j
		pVtx[0].vtx = D3DXVECTOR3(0.0f - (fSizeX/2), 0.0f + (fSizeZ/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(0.0f + (fSizeX/2), 0.0f + (fSizeZ/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(0.0f - (fSizeX/2), 0.0f - (fSizeZ/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(0.0f + (fSizeX/2), 0.0f - (fSizeZ/2), 0.0f);

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffEnemyBoard->Unlock();
	}
}

int CreateEnemyBoard(D3DXVECTOR3 pos, float fSizeX, float fSizeZ)
{
	int nIdxBoard = -1;

	for(int nCntBoard = 0; nCntBoard < MAX_ENEMY; nCntBoard++)
	{
		if(!g_aEnemyBoard[nCntBoard].bUse)
		{
			// �e�̈ʒu�A��]�p��ݒ�
			D3DXVECTOR3 tPos = pos;
			tPos.y += 15.0f;
			g_aEnemyBoard[nCntBoard].pos = tPos;

			// �e���[�N���g�p��ԂɕύX
			g_aEnemyBoard[nCntBoard].bUse = true;

			// ���_���W�̐ݒ�֐����Ăяo��
			SetVertexEnemyBoard( nCntBoard, fSizeX, fSizeZ);

			// �e�̔ԍ���nIdxShadow�ɐݒ�
			nIdxBoard = nCntBoard;
			break;
		}
	}

	// nIdxShadow��Ԃ�
	return nIdxBoard;
}

void ReleaseEnemyBoard(int nIdxBoard)
{
	//�w�肳�ꂽ�e���e���[�N�̌��Ȃ����ǂ���
	if(nIdxBoard < MAX_ENEMY)
	{
		// �e���[�N�𖢎g�p��Ԃɐݒ�
		g_aEnemyBoard[nIdxBoard].bUse = false;
	}
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void SetPositionEnemyBoard(int nIdxBoard, D3DXVECTOR3 pos)
{
	// �w�肳��ʒu�ɐݒ�
	D3DXVECTOR3 tPos = pos;
	g_aEnemyBoard[nIdxBoard].pos.x = pos.x;
	g_aEnemyBoard[nIdxBoard].pos.z = pos.z;
}
