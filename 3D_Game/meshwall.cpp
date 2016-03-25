/*******************************************************************************
*
* �^�C�g��:		���b�V���E�H�[������
* �v���O������:	meshwall.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "meshwall.h"
#include "input.h"
#include "debugproc.h"
#include "hitCheck.h"
#include "fountain.h"
#include "stageManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_MESH_WALL		(64)							// �ǂ̑���
#define	TEXTURE_FILENAME	"data/TEXTURE/wall000.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_WALL		(5.0f)						// �ړ����x
#define	VALUE_ROTATE_WALL	(D3DX_PI * 0.001f)			// ��]���x


//**************************************
// ���b�V���E�H�[���\����
//**************************************
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pD3DIdxBuff;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX mtxWorldField;				// ���[���h�}�g���b�N�X  
	D3DXVECTOR3 pos;						// �|���S���\���ʒu�̒��S���W
	D3DXVECTOR3 rot;						// �|���S���̉�]�p

	int nNumBlockX, nNumBlockY;				// �u���b�N��
	int nNumVertex;							// �����_��
	int nNumVertexIndex;					// ���C���f�b�N�X��
	int nNumPolygon;						// ���|���S����
	float fSizeBlockX, fSizeBlockY;			// �u���b�N�T�C�Y
} MESH_WALL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice, MESH_WALL *pMesh);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;	// �e�N�X�`���ւ̃|�C���^

MESH_WALL g_aMeshWall[MAX_MESH_WALL];		// ���b�V���ǃ��[�N
int g_nNumMeshWall = 0;						// ���b�V���ǂ̐�

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESH_WALL *pMesh;

	if(g_nNumMeshWall >= MAX_MESH_WALL)
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	if(!g_pTextureWall)
	{
		D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
									TEXTURE_FILENAME,	// �t�@�C���̖��O
									&g_pTextureWall);	// �ǂݍ��ރ������[
	}

	// ���b�V���Ǎ\���̂̃|�C���^�ɍ���g�p���郁�b�V���ǃ��[�N�̃A�h���X����
	pMesh = &g_aMeshWall[g_nNumMeshWall];

	// ���b�V���ǂ̐��𑝂₷
	g_nNumMeshWall++;

	// �ʒu�E�����̏����ݒ�
	pMesh->pos = pos;
	pMesh->rot = rot;

	// �u���b�N���̐ݒ�
	pMesh->nNumBlockX = nNumBlockX;
	pMesh->nNumBlockY = nNumBlockY;
	
	// ���_���̐ݒ�
	pMesh->nNumVertex = (nNumBlockX+1) * (nNumBlockY+1);
	
	// �C���f�b�N�X���̐ݒ�
	pMesh->nNumVertexIndex = (nNumBlockX + 1)*2*nNumBlockY + (nNumBlockY - 1)*2;
	
	// �|���S�����̐ݒ�
	pMesh->nNumPolygon = nNumBlockX*nNumBlockY*2 + (nNumBlockY - 1)*4;
	
	// �u���b�N�T�C�Y�̐ݒ�
	pMesh->fSizeBlockX = fSizeBlockX;
	pMesh->fSizeBlockY = fSizeBlockY;

	// ���_���̍쐬
	MakeVertexField(pDevice, pMesh);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshWall(void)
{
	MESH_WALL *pMesh;

	for(int nCntMeshWall = 0; nCntMeshWall < g_nNumMeshWall; nCntMeshWall++)
	{
		// ���b�V���Ǎ\���̂̃|�C���^�Ƀ��b�V���ǃ��[�N�̃A�h���X����
		pMesh = &g_aMeshWall[nCntMeshWall];

		if(pMesh->pD3DVtxBuff)
		{// ���_�o�b�t�@�̊J��
			pMesh->pD3DVtxBuff->Release();
			pMesh->pD3DVtxBuff = NULL;
		}

		if(pMesh->pD3DIdxBuff)
		{// �C���f�b�N�X�o�b�t�@�̊J��
			pMesh->pD3DIdxBuff->Release();
			pMesh->pD3DIdxBuff = NULL;
		}
	}

	if(g_pTextureWall)
	{// �e�N�X�`���̊J��
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	//���b�V���ǂ̐����Z�b�g
	g_nNumMeshWall = 0;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshWall(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	MESH_WALL *pMesh;
	D3DXMATRIX mtxRot, mtxTranslate;

	// ���b�V���Ǎ\���̂̃|�C���^�Ƀ��b�V���ǃ��[�N�̐擪�A�h���X����
	pMesh = g_aMeshWall;

	for(int nCntMeshWall = 0; nCntMeshWall < g_nNumMeshWall; nCntMeshWall++, pMesh++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity( &pMesh->mtxWorldField);
	
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll( &mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		D3DXMatrixMultiply( &pMesh->mtxWorldField, &pMesh->mtxWorldField, &mtxRot);
	
		// �ړ��𔽉f
		D3DXMatrixTranslation( &mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		D3DXMatrixMultiply( &pMesh->mtxWorldField, &pMesh->mtxWorldField, &mtxTranslate);
	
		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform( D3DTS_WORLD, &pMesh->mtxWorldField);

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
		pDevice->SetStreamSource(0, pMesh->pD3DVtxBuff, 0, sizeof(VERTEX_3D));

		// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
		pDevice->SetIndices(pMesh->pD3DIdxBuff);

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureWall);
	
		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			0,						//�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
			0,						//�ŏ��̒��_�C���f�b�N�X
			pMesh->nNumVertex,		//���_�̐�
			0,						//�ǂݎ����J�n����ʒu
			pMesh->nNumPolygon		//�|���S���̐�
		);

		//�����͍ŏ��̕����̒����������Ȃ��`�揇
		if(nCntMeshWall == 7 && GetStageMode() == STAGE0)
		{
			DrawFountain();
		}
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexField(LPDIRECT3DDEVICE9 pDevice, MESH_WALL *pMesh)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * pMesh->nNumVertex,	//���_�f�[�^�̃o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&pMesh->pD3DVtxBuff,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateIndexBuffer(
		sizeof(WORD) * pMesh->nNumVertexIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pMesh->pD3DIdxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	
	{//���_�o�b�t�@�̒��g�𖄂߂�

		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pMesh->pD3DVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

		for(int nCntY = 0; nCntY < pMesh->nNumBlockY+1; nCntY++)
		{
			for(int nCntX = 0; nCntX < pMesh->nNumBlockX+1; nCntX++)
			{
				pVtx[0].vtx = D3DXVECTOR3( -pMesh->fSizeBlockX *pMesh->nNumBlockX /2 + nCntX*pMesh->fSizeBlockX, pMesh->fSizeBlockY * pMesh->nNumBlockY/2 - nCntY*pMesh->fSizeBlockY, 0.0f);
				pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
				pVtx[0].nor = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);
				pVtx[0].tex = D3DXVECTOR2( 1.0f * nCntX, 1.0f * nCntY );
				pVtx++;
			}
		}


		// ���_�f�[�^���A�����b�N����
		pMesh->pD3DVtxBuff->Unlock();	
	}


	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�

		WORD *pIdx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		pMesh->pD3DIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

		for(int nCntY = 0; nCntY < pMesh->nNumBlockY; nCntY++)
		{
			if(nCntY > 0)
			{
				pIdx[0] = (pMesh->nNumBlockX + 1) * (nCntY + 1);
				pIdx++;
			}
			for(int nCntX = 0; nCntX < pMesh->nNumBlockX+1; nCntX++, pIdx+=2)
			{
				//���_���W�̐ݒ�
				pIdx[0] = (pMesh->nNumBlockX + 1) * (nCntY + 1) + nCntX; 
				pIdx[1] = (pMesh->nNumBlockX + 1) * (nCntY) + nCntX;
			}
			if(nCntY < pMesh->nNumBlockY - 1)
			{
				*pIdx = *(pIdx-1);
				pIdx++;
			}
		}

		// ���_�f�[�^���A�����b�N����
		pMesh->pD3DIdxBuff->Unlock();	
	}

	return S_OK;
}
bool HitCheckMeshWall( D3DXVECTOR3 tNowPos, D3DXVECTOR3 tNextPos, D3DXVECTOR3 *wall_nor, D3DXVECTOR3 *HitPoint)
{
	bool bHit = false;
	D3DXVECTOR3 tHitPosNear = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXVECTOR3 tPos[4];


	// ���b�V���Ǎ\���̂̃|�C���^�Ƀ��b�V���ǃ��[�N�̐擪�A�h���X����
	MESH_WALL *pMesh = g_aMeshWall;

	for(int nCntMeshWall = 0; nCntMeshWall < g_nNumMeshWall; nCntMeshWall++, pMesh++)
	{

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity( &pMesh->mtxWorldField);
	
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll( &mtxRot, pMesh->rot.y, pMesh->rot.x, pMesh->rot.z);
		D3DXMatrixMultiply( &pMesh->mtxWorldField, &pMesh->mtxWorldField, &mtxRot);
	
		// �ړ��𔽉f
		D3DXMatrixTranslation( &mtxTranslate, pMesh->pos.x, pMesh->pos.y, pMesh->pos.z);
		D3DXMatrixMultiply( &pMesh->mtxWorldField, &pMesh->mtxWorldField, &mtxTranslate);

		{//���_�o�b�t�@�̎擾

			VERTEX_3D *pVtx;

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
			pMesh->pD3DVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

			tPos[0] = pVtx[0].vtx;
			tPos[1] = pVtx[pMesh->nNumBlockX].vtx;
			tPos[2] = pVtx[(pMesh->nNumBlockX+1) * pMesh->nNumBlockY].vtx;
			tPos[3] = pVtx[(pMesh->nNumBlockX+1) * (pMesh->nNumBlockY+1) - 1 ].vtx;


			// ���_�f�[�^���A�����b�N����
			pMesh->pD3DVtxBuff->Unlock();	
		}

		//���_�o�b�t�@�Ƀ��[���h�}�g���b�N�X���|����A���_�o�b�t�@���X�V�����
		D3DXVec3TransformCoord( &tPos[0], &tPos[0], &pMesh->mtxWorldField);
		D3DXVec3TransformCoord( &tPos[1], &tPos[1], &pMesh->mtxWorldField);
		D3DXVec3TransformCoord( &tPos[2], &tPos[2], &pMesh->mtxWorldField);
		D3DXVec3TransformCoord( &tPos[3], &tPos[3], &pMesh->mtxWorldField);

		if( HitCheck( tPos[0], tPos[1], tPos[2], tNowPos, tNextPos) )
		{
			bHit = true;

			//��ԋ߂���_�����߂�
			D3DXVECTOR3 tHitPos = GetHitPos();
			D3DXVECTOR3 v1 = tHitPosNear - tNowPos;
			D3DXVECTOR3 v2 = tHitPos - tNowPos;
			if( D3DXVec3Length(&v1) > D3DXVec3Length(&v2))
			{
				tHitPosNear = tHitPos;

				//�ǂ̖@�����擾
				if(wall_nor != NULL) *wall_nor = CrossProduct( tPos[0] - tPos[1], tPos[2] - tPos[1]);
			}
			
		}
		else if( HitCheck( tPos[1], tPos[2], tPos[3], tNowPos, tNextPos) )
		{
			bHit = true;

			//��ԋ߂���_�����߂�
			D3DXVECTOR3 tHitPos = GetHitPos();
			D3DXVECTOR3 v1 = tHitPosNear - tNowPos;
			D3DXVECTOR3 v2 = tHitPos - tNowPos;
			if( D3DXVec3Length(&v1) > D3DXVec3Length(&v2))
			{
				tHitPosNear = tHitPos;

				//�ǂ̖@�����擾
				if(wall_nor != NULL) *wall_nor = CrossProduct( tPos[1] - tPos[2], tPos[3] - tPos[2]);
			}
		}
	}

	//��ԋ߂���_��n��
	if(HitPoint != NULL) *HitPoint = tHitPosNear;

	return bHit;
}