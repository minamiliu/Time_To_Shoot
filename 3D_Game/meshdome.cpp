/*******************************************************************************
*
* �^�C�g��:		���b�V���h�[������
* �v���O������:	meshdome.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "meshdome.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_MESHDOME	"data/TEXTURE/dome.jpg"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	VALUE_MOVE_DOME		(4.0f)						// �ړ����x
#define	VALUE_ROTATE_DOME	(D3DX_PI * 0.01f)			// ��]���x

#define	DOME_HEIGHT_RATE	(1.0f)		// ��h�[���̍����W��
#define	TEX_COUNT_LOOP		(8)			// �e�N�X�`���̌J��Ԃ���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9 pTextureDome;			// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffDome;		// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuffDome;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffDomeTop;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIdxBuffDomeTop;		// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXMATRIX mtxWorldDome;					// ���[���h�}�g���b�N�X
	D3DXVECTOR3 posDome;						// �|���S���\���ʒu�̒��S���W
	D3DXVECTOR3 rotDome;						// �|���S���̉�]�p

	int nNumBlockHDome;						// �u���b�N��
	int nNumBlockVDome;						// �u���b�N��
	int nNumVertexDome;						// �����_��	
	int nNumVertexIndexDome;				// ���C���f�b�N�X��
	int nNumPolygonDome;					// ���|���S����
	float fRadius;							// ���a
} MESH_DOME;

MESH_DOME g_aMeshDome;

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshDome(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nNumBlockH, int nNumBlockV)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �ʒu�E�����E���a�i�O���[�o���ϐ��j�̏����ݒ�
	g_aMeshDome.posDome = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_aMeshDome.rotDome = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_aMeshDome.fRadius = fRadius;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, TEXTURE_MESHDOME, &g_aMeshDome.pTextureDome);

	// �u���b�N��H/V�i�O���[�o���ϐ��j�̐ݒ�
	g_aMeshDome.nNumBlockHDome = nNumBlockH;
	g_aMeshDome.nNumBlockVDome = nNumBlockV;

	// ���_���i�O���[�o���ϐ��j�̐ݒ�
	g_aMeshDome.nNumVertexDome = (g_aMeshDome.nNumBlockHDome + 1) * (g_aMeshDome.nNumBlockVDome + 1);

	// �|���S�����i�O���[�o���ϐ��j�̐ݒ�
	g_aMeshDome.nNumPolygonDome = ( g_aMeshDome.nNumBlockHDome * (g_aMeshDome.nNumBlockVDome * 2) + (g_aMeshDome.nNumBlockVDome - 1) * 4);

	// �C���f�b�N�X���i�O���[�o���ϐ��j�̐ݒ�
	g_aMeshDome.nNumVertexIndexDome = (g_aMeshDome.nNumBlockHDome * (g_aMeshDome.nNumBlockVDome * 2) + g_aMeshDome.nNumBlockVDome * 2 + (g_aMeshDome.nNumBlockVDome - 1) * 2);

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aMeshDome.nNumVertexDome,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,							// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,								// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,							// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_aMeshDome.pVtxBuffDome,					// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))										// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_aMeshDome.nNumBlockHDome + 1),		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,						// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,							// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,						// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_aMeshDome.pVtxBuffDomeTop,						// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))									// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_aMeshDome.nNumVertexIndexDome,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_aMeshDome.pIdxBuffDome,					// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * (g_aMeshDome.nNumBlockHDome + 2),			// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_aMeshDome.pIdxBuffDomeTop,					// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
		const float angleH = (D3DX_PI * 2.0f) / g_aMeshDome.nNumBlockHDome;
		const float angleV = (D3DX_PI / 2.0f) / (g_aMeshDome.nNumBlockVDome + 1);
		const float width = 1.0f / g_aMeshDome.nNumBlockHDome;
		const float height = 1.0f / g_aMeshDome.nNumBlockVDome;
		float lengthXZ;
		D3DXVECTOR3 nor;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_aMeshDome.pVtxBuffDome->Lock( 0, 0, (void**)&pVtx, 0);
		for(int nCntV = 0; nCntV < (g_aMeshDome.nNumBlockVDome + 1); nCntV++)
		{
			lengthXZ = cosf(angleV * nCntV) * g_aMeshDome.fRadius;

			for(int nCntH = 0; nCntH < (g_aMeshDome.nNumBlockHDome + 1); nCntH++, pVtx++)
			{
				// ���_���W�̐ݒ�
				pVtx->vtx.x = sinf(angleH * nCntH) * lengthXZ;
				pVtx->vtx.y = sinf(angleV * nCntV) * g_aMeshDome.fRadius * DOME_HEIGHT_RATE;
				pVtx->vtx.z = -cosf(angleH * nCntH) * lengthXZ;

				// �@���̐ݒ�
				nor = pVtx->vtx;
				D3DXVec3Normalize(&nor, &nor);
				pVtx->nor = nor;

				// ���_�J���[�̐ݒ�
				pVtx->col = D3DCOLOR_RGBA(255,255,255,255);

				// �e�N�X�`�����W�̐ݒ�
				pVtx->tex.x = nCntH * width * TEX_COUNT_LOOP;

				if(nCntV == g_aMeshDome.nNumBlockVDome)
				{
					pVtx->tex.y = 0.01f;
				}
				else
				{
					pVtx->tex.y = 1.0f - nCntV * height;
				}
			}
		}
		// ���_�f�[�^���A�����b�N����
		g_aMeshDome.pVtxBuffDome->Unlock();

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_aMeshDome.pVtxBuffDomeTop->Lock( 0, 0, (void**)&pVtx, 0);
		// ���_���W�̐ݒ�
		pVtx->vtx.x = 0.0f;
		pVtx->vtx.y = sinf(angleV * (g_aMeshDome.nNumBlockVDome + 1)) * g_aMeshDome.fRadius * DOME_HEIGHT_RATE;
		pVtx->vtx.z = 0.0f;

		// �@���̐ݒ�
		nor = pVtx->vtx;
		D3DXVec3Normalize(&nor, &nor);
		pVtx->nor = nor;

		// ���_�J���[�̐ݒ�
		pVtx->col = D3DCOLOR_RGBA(255,255,255,255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx->tex.x = 0.5f;
		pVtx->tex.y = 0.0f;

		// ���_�o�b�t�@�̃|�C���^���C���N�������g
		pVtx++;

		lengthXZ = cosf(angleV * g_aMeshDome.nNumBlockVDome) * g_aMeshDome.fRadius;
		for(int nCntH = 0; nCntH < g_aMeshDome.nNumBlockHDome; nCntH++, pVtx++)
		{
			// ���_���W�̐ݒ�
			pVtx->vtx.x = -sinf(angleH * nCntH) * lengthXZ;
			pVtx->vtx.y = sinf(angleV * g_aMeshDome.nNumBlockVDome) * g_aMeshDome.fRadius * DOME_HEIGHT_RATE;
			pVtx->vtx.z = -cosf(angleH * nCntH) * lengthXZ;

			// �@���̐ݒ�
			nor = pVtx->vtx;
			D3DXVec3Normalize(&nor, &nor);
			pVtx->nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx->col = D3DCOLOR_RGBA(255,255,255,255);

			// �e�N�X�`�����W�̐ݒ�
			pVtx->tex.x = 0.5f;
			pVtx->tex.y = 1.0f;
		}

		// ���_�f�[�^���A�����b�N����
		g_aMeshDome.pVtxBuffDomeTop->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_aMeshDome.pIdxBuffDome->Lock( 0, 0, (void**)&pIdx, 0);
		for(int nCntV = 0; nCntV < g_aMeshDome.nNumBlockVDome; nCntV++)
		{
			if(nCntV != 0)
			{
				*pIdx = nCntV * (g_aMeshDome.nNumBlockHDome + 1);
				pIdx++;
			}

			for(int nCntH = 0; nCntH < g_aMeshDome.nNumBlockHDome + 1; nCntH++)
			{
				*pIdx = nCntV * (g_aMeshDome.nNumBlockHDome + 1) + nCntH;
				pIdx++;

				*pIdx = (nCntV + 1) * (g_aMeshDome.nNumBlockHDome + 1) + nCntH;
				pIdx++;
			}

			if(nCntV != (g_aMeshDome.nNumBlockVDome - 1))
			{
				*pIdx = (nCntV + 1) * (g_aMeshDome.nNumBlockHDome + 1);
				pIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_aMeshDome.pIdxBuffDome->Unlock();

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_aMeshDome.pIdxBuffDomeTop->Lock( 0, 0, (void**)&pIdx, 0);
		for(int nCntH = 0; nCntH < g_aMeshDome.nNumBlockHDome + 1; nCntH++, pIdx++)
		{
			*pIdx = nCntH;
		}
		*pIdx = 1;

		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_aMeshDome.pIdxBuffDomeTop->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshDome(void)
{
	if(g_aMeshDome.pVtxBuffDome != NULL)
	{// ���_�o�b�t�@�̊J��
		g_aMeshDome.pVtxBuffDome->Release();
		g_aMeshDome.pVtxBuffDome = NULL;
	}

	if(g_aMeshDome.pIdxBuffDome != NULL)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		g_aMeshDome.pIdxBuffDome->Release();
		g_aMeshDome.pIdxBuffDome = NULL;
	}

	if(g_aMeshDome.pVtxBuffDomeTop != NULL)
	{// ���_�o�b�t�@�̊J��
		g_aMeshDome.pVtxBuffDomeTop->Release();
		g_aMeshDome.pVtxBuffDomeTop = NULL;
	}

	if(g_aMeshDome.pIdxBuffDomeTop != NULL)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		g_aMeshDome.pIdxBuffDomeTop->Release();
		g_aMeshDome.pIdxBuffDomeTop = NULL;
	}

	if(g_aMeshDome.pTextureDome != NULL)
	{// �e�N�X�`���̊J��
		g_aMeshDome.pTextureDome->Release();
		g_aMeshDome.pTextureDome = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshDome(void)
{

	//PrintDebugProc("*** ������� ***\n");
	//PrintDebugProc("�ʒu           : (%f : %f : %f)\n", g_posDome.x, g_posDome.y, g_posDome.z);
	//PrintDebugProc("����           : (%f : %f : %f)\n", g_rotDome.x, g_rotDome.y, g_rotDome.z);
	//PrintDebugProc("�u���b�N��     : (%d x %d)�u���b�N\n", g_nNumBlockHDome, g_nNumBlockVDome);
	//PrintDebugProc("���_��         : %d���_ + %d���_\n", ���_��, g_nNumBlockHDome + 1);
	//PrintDebugProc("�C���f�b�N�X�� : %d�� + %d��\n", g_nNumVertexIndexDome, g_nNumBlockHDome + 2);
	//PrintDebugProc("�|���S����     : %d�|���S�� + %d�|���S��\n", g_nNumPolygonDome, g_nNumBlockHDome);
	//PrintDebugProc("\n");

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshDome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);	// ���ʂ��J�����O

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_aMeshDome.mtxWorldDome);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aMeshDome.rotDome.y, g_aMeshDome.rotDome.x, g_aMeshDome.rotDome.z);
	D3DXMatrixMultiply( &g_aMeshDome.mtxWorldDome, &g_aMeshDome.mtxWorldDome, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation( &mtxTranslate, g_aMeshDome.rotDome.x, g_aMeshDome.rotDome.y, g_aMeshDome.rotDome.z);
	D3DXMatrixMultiply( &g_aMeshDome.mtxWorldDome, &g_aMeshDome.mtxWorldDome, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform( D3DTS_WORLD, &g_aMeshDome.mtxWorldDome);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, g_aMeshDome.pVtxBuffDome, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(g_aMeshDome.pIdxBuffDome);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_aMeshDome.pTextureDome);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,								//�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
		0,								//�ŏ��̒��_�C���f�b�N�X
		g_aMeshDome.nNumVertexDome,		//���_�̐�
		0,								//�ǂݎ����J�n����ʒu
		g_aMeshDome.nNumPolygonDome		//�|���S���̐�
	);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, g_aMeshDome.pVtxBuffDomeTop, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(g_aMeshDome.pIdxBuffDomeTop);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLEFAN,				//�v���~�e�B�u�̎��
		0,								//�ŏ��̒��_�C���f�b�N�X�܂ł̃I�t�Z�b�g
		0,								//�ŏ��̒��_�C���f�b�N�X
		(g_aMeshDome.nNumBlockHDome + 1),		//���_�̐�
		0,								//�ǂݎ����J�n����ʒu
		g_aMeshDome.nNumBlockHDome		//�|���S���̐�
	);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);	// ���ʂ��J�����O
}

