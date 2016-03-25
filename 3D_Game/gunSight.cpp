/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`���e�̏Ə��폈��
* �v���O������:	gunSight.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "gunSight.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_POLYGON				"data/TEXTURE/sight2.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(SCREEN_WIDTH/2)				// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(SCREEN_HEIGHT/2)				// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(50)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(50)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
//#define	TIME_CHANGE_PATTERN			(5)								// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define MAX_GUNSIGHT				(1)							// �I�u�W�F�N�g�̐�


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexGunSight(LPDIRECT3DDEVICE9 pDevice);
void SetVertexGunSight(int nCntExplo);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferGunSight = NULL;
LPDIRECT3DTEXTURE9 g_pTextureGunSight = NULL;

GUNSIGHT g_gunSight[MAX_GUNSIGHT]; //�v���C���[�̃��[�N


//=============================================================================
// ����������
//=============================================================================
HRESULT InitGunSight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntExplo = 0; nCntExplo < MAX_GUNSIGHT; nCntExplo++)
	{
		//bullet�̏�����
		g_gunSight[nCntExplo].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_gunSight[nCntExplo].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		//g_gunSight[nCntExplo].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_gunSight[nCntExplo].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_gunSight[nCntExplo].bUse = true;
	}



	//���_���̍쐬
	if(FAILED(MakeVertexGunSight(pDevice)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,		// �t�@�C���̖��O
								&g_pTextureGunSight);	// �ǂݍ��ރ������[

	return S_OK;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawGunSight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferGunSight, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGunSight);

	for(int nCntExplo = 0; nCntExplo < MAX_GUNSIGHT; nCntExplo++)
	{
		if(g_gunSight[nCntExplo].bUse)
		{

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntExplo*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);
		}	
	}



}


//=============================================================================
// �I������
//=============================================================================
void UninitGunSight(void)
{
	if(g_pTextureGunSight != NULL)
	{
		g_pTextureGunSight->Release();
		g_pTextureGunSight = NULL;
	}
	if(g_pVtxBufferGunSight != NULL)
	{
		g_pVtxBufferGunSight->Release();
		g_pVtxBufferGunSight = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexGunSight(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexGunSight(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_GUNSIGHT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferGunSight,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}



	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferGunSight->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntExplo = 0; nCntExplo < MAX_GUNSIGHT; nCntExplo++, pVtx += NUM_VERTEX)
	{
		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
		pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	
	}



	g_pVtxBufferGunSight->Unlock();

	return S_OK;
}



//=============================================================================
// �X�V����
//=============================================================================
void UpdateGunSight(void)
{

}

/*******************************************************************************
�֐���:	void SetVertexGunSight(int nCntExplo)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���_���W�̐ݒ�
*******************************************************************************/
void SetVertexGunSight(int nCntExplo)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferGunSight->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntExplo * NUM_VERTEX; 
		
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x - (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_gunSight[nCntExplo].pos.x + (POLYGON_SIZE_X/2), g_gunSight[nCntExplo].pos.y + (POLYGON_SIZE_Y/2), 0.0f);	

		

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferGunSight->Unlock();
	}
}


