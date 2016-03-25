/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`���̎��v�\������
* �v���O������:	clock.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
********************************************************************************

*******************************************************************************/
/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "clock.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define	TEXTURE_CLOCK				"data/TEXTURE/UI/Clock.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_HAND				"data/TEXTURE/UI/ClockHand.png"		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(SCREEN_WIDTH/2 + 100)				// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(70)				// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(100)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(100)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
//#define	TIME_CHANGE_PATTERN			(5)								// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)
#define MAX_OBJECT					(2)							// �I�u�W�F�N�g�̐�

#define	VALUE_ROTATE			(D3DX_PI / 30.0f)								// �J�����̉�]��

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexClock(LPDIRECT3DDEVICE9 pDevice);
void SetVertexClockRot(int nCntClock, float angle);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferClock = NULL;
LPDIRECT3DTEXTURE9 g_pTextureClock = NULL;
LPDIRECT3DTEXTURE9 g_pTextureHand = NULL;

CLOCK g_clock[MAX_OBJECT]; //�v���C���[�̃��[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitClock(D3DXVECTOR3 initPos)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntClock = 0; nCntClock < MAX_OBJECT; nCntClock++)
	{
		//bullet�̏�����
		g_clock[nCntClock].pos = initPos;
		g_clock[nCntClock].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_clock[nCntClock].bUse = true;
	}



	//���_���̍쐬
	if(FAILED(MakeVertexClock(pDevice)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_CLOCK,		// �t�@�C���̖��O
								&g_pTextureClock);	// �ǂݍ��ރ������[
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_HAND,		// �t�@�C���̖��O
								&g_pTextureHand);	// �ǂݍ��ރ������[

	return S_OK;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawClock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferClock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntClock = 0; nCntClock < MAX_OBJECT; nCntClock++)
	{
		if(g_clock[nCntClock].bUse)
		{
			switch( nCntClock)
			{
			case 0:
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureClock);
				break;
			case 1:
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureHand);
				break;

			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntClock*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);
		}	
	}


}


//=============================================================================
// �I������
//=============================================================================
void UninitClock(void)
{
	if(g_pTextureClock != NULL)
	{
		g_pTextureClock->Release();
		g_pTextureClock = NULL;
	}
	if(g_pTextureHand != NULL)
	{
		g_pTextureHand->Release();
		g_pTextureHand = NULL;
	}
	if(g_pVtxBufferClock != NULL)
	{
		g_pVtxBufferClock->Release();
		g_pVtxBufferClock = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexClock(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexClock(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferClock,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}



	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferClock->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntClock = 0; nCntClock < MAX_OBJECT; nCntClock++, pVtx += NUM_VERTEX)
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
		pVtx[0].vtx = D3DXVECTOR3(g_clock[nCntClock].pos.x - (POLYGON_SIZE_X/2), g_clock[nCntClock].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_clock[nCntClock].pos.x + (POLYGON_SIZE_X/2), g_clock[nCntClock].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_clock[nCntClock].pos.x - (POLYGON_SIZE_X/2), g_clock[nCntClock].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_clock[nCntClock].pos.x + (POLYGON_SIZE_X/2), g_clock[nCntClock].pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));	

	}



	g_pVtxBufferClock->Unlock();

	return S_OK;
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateClock(float fTimeSpeed)
{
	SetVertexClockRot( 1, VALUE_ROTATE * fTimeSpeed);
}

/*******************************************************************************
�֐���:	void SetVertexClockRot(int nCntClock, float angle)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	���_���W�̐ݒ�
*******************************************************************************/
void SetVertexClockRot(int nCntClock, float angle)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferClock->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += nCntClock * NUM_VERTEX; 
		
		// ���_���W�̐ݒ�
		float xc = g_clock[nCntClock].pos.x;
		float yc = g_clock[nCntClock].pos.y;
		float nx,ny;

		nx = (pVtx[0].vtx.x - xc) * cosf(angle) - (pVtx[0].vtx.y - yc) * sinf(angle) + xc;
		ny = (pVtx[0].vtx.x - xc) * sinf(angle) + (pVtx[0].vtx.y - yc) * cosf(angle) + yc;
		pVtx[0].vtx.x = nx;
		pVtx[0].vtx.y = ny;

		nx = (pVtx[1].vtx.x - xc) * cosf(angle) - (pVtx[1].vtx.y - yc) * sinf(angle) + xc;
		ny = (pVtx[1].vtx.x - xc) * sinf(angle) + (pVtx[1].vtx.y - yc) * cosf(angle) + yc;
		pVtx[1].vtx.x = nx;
		pVtx[1].vtx.y = ny;

		nx = (pVtx[2].vtx.x - xc) * cosf(angle) - (pVtx[2].vtx.y - yc) * sinf(angle) + xc;
		ny = (pVtx[2].vtx.x - xc) * sinf(angle) + (pVtx[2].vtx.y - yc) * cosf(angle) + yc;
		pVtx[2].vtx.x = nx;
		pVtx[2].vtx.y = ny;

		nx = (pVtx[3].vtx.x - xc) * cosf(angle) - (pVtx[3].vtx.y - yc) * sinf(angle) + xc;
		ny = (pVtx[3].vtx.x - xc) * sinf(angle) + (pVtx[3].vtx.y - yc) * cosf(angle) + yc;
		pVtx[3].vtx.x = nx;
		pVtx[3].vtx.y = ny;

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferClock->Unlock();
	}
}


