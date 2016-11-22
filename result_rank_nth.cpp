/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`�� ���U���g�̃����L���O�̏��ʐ����\������
* �v���O������:	result_BG.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "result_rank_nth.h"
#include "result_rank.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(150)							// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(150)							// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(50)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(100)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(12)							// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define NUM_DIGIT					(2)
#define MAX_NUM_BOUND				(10)
#define NUM_OBJECT					(NUM_DIGIT*MAX_RANK)			// �I�u�W�F�N�g�̐�

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexResultRankNth(LPDIRECT3DDEVICE9 pDevice);
void SetTextureResultRankNth( int nRank, int nIdx, int number);
void SetColorResultRankNth( int nRank, int nIdx, D3DCOLOR col);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRankNth = NULL;
LPDIRECT3DTEXTURE9 g_pTextureRankNth = NULL;

RESULT_RANK_NTH	g_numRankNth[MAX_RANK]; //score�̃��[�N




//=============================================================================
// ����������
//=============================================================================
HRESULT InitResultRankNth()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//�l�����̂݁A������
	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{		
		g_numRankNth[nCntRank].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
		g_numRankNth[nCntRank].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_numRankNth[nCntRank].score = 0;
	}
	for(int nCntRank = 0; nCntRank < MAX_RANK/2; nCntRank++)
	{
		g_numRankNth[nCntRank].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y + nCntRank*110, 0.0f);
		g_numRankNth[nCntRank + MAX_RANK/2].pos = D3DXVECTOR3( POLYGON_POS_X + 550, POLYGON_POS_Y + nCntRank*110, 0.0f);
	}

	g_numRankNth[MAX_RANK-1].pos = D3DXVECTOR3( -1000, -1000 , 0.0f);


	//���_���̍쐬
	if(FAILED(MakeVertexResultRankNth(pDevice)))
	{
		return E_FAIL;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,	// �t�@�C���̖��O
								&g_pTextureRankNth);	// �ǂݍ��ރ������[

	return S_OK;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultRankNth(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferRankNth, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRankNth);

	//�|���S���̕`��
	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++)
	{
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			(nCntPlace*NUM_VERTEX),						//���[�h����ŏ��̒��_�C���f�b�N�X
			NUM_POLYGON				//�|���S���̐�
		);
	}


}


//=============================================================================
// �I������
//=============================================================================
void UninitResultRankNth(void)
{
	if(g_pTextureRankNth != NULL)
	{
		g_pTextureRankNth->Release();
		g_pTextureRankNth = NULL;
	}
	if(g_pVtxBufferRankNth!= NULL)
	{
		g_pVtxBufferRankNth->Release();
		g_pVtxBufferRankNth = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexResultRankNth(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexResultRankNth(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferRankNth,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferRankNth->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for(int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++, pVtx += NUM_VERTEX)
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
			pVtx[0].vtx = D3DXVECTOR3(g_numRankNth[nCntRank].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numRankNth[nCntRank].pos.y , 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_numRankNth[nCntRank].pos.x  + g_numRankNth[nCntRank].size.x +  nCntDigit*POLYGON_SIZE_X, g_numRankNth[nCntRank].pos.y , 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_numRankNth[nCntRank].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numRankNth[nCntRank].pos.y + g_numRankNth[nCntRank].size.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_numRankNth[nCntRank].pos.x  + g_numRankNth[nCntRank].size.x + nCntDigit*POLYGON_SIZE_X, g_numRankNth[nCntRank].pos.y + g_numRankNth[nCntRank].size.y, 0.0f);

			//�e�N�X�`�����W�w��
			pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
			pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
			pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
			pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		}	
	}

	g_pVtxBufferRankNth->Unlock();

	return S_OK;
}



//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultRankNth(void)
{

}



/*******************************************************************************
�֐���:	void SetTextureResultRankNth( int nRank, int nIdx, int number) 
����:	 int nTdx�F�����ڂ�, int number�F�\�����鐔
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureResultRankNth( int nRank, int nIdx, int number) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferRankNth->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nRank * NUM_DIGIT * NUM_VERTEX);

		//�e�N�X�`�����W�̐ݒ�
		fPosXLeft = TEX_PATTERN_SIZE_X * number;
		fPosXRight = TEX_PATTERN_SIZE_X * number + TEX_PATTERN_SIZE_X;
		fPosYUp = 0;
		fPosYDown = TEX_PATTERN_SIZE_Y;

		pVtx[0].tex = D3DXVECTOR2( fPosXLeft, fPosYUp );
		pVtx[1].tex = D3DXVECTOR2( fPosXRight, fPosYUp );
		pVtx[2].tex = D3DXVECTOR2( fPosXLeft, fPosYDown );
		pVtx[3].tex = D3DXVECTOR2( fPosXRight, fPosYDown );

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferRankNth->Unlock();
	}
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void SetColorResultRankNth( int nRank, int nIdx, D3DCOLOR col) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferRankNth->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nRank * NUM_DIGIT * NUM_VERTEX);

		//color
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferRankNth->Unlock();
	}
}

/*******************************************************************************
�֐���:	void ChangeResultRankNth(int nRank, int value, D3DCOLOR col)
����:	int value�F��������_��
�߂�l:	�Ȃ�
����:	���_����ύX����
*******************************************************************************/
void ChangeResultRankNth(int nRank, int value, D3DCOLOR col)
{
	g_numRankNth[nRank].score += value;

	if(g_numRankNth[nRank].score >= MAX_NUM_BOUND) g_numRankNth[nRank].score = MAX_NUM_BOUND; //�ő�l


	for(int keta = 1; keta <= NUM_DIGIT; keta++)
	{
		if(keta == 1)
		{
			SetTextureResultRankNth(nRank ,keta, 11); //11�͓_�X�̃e�N�X�`��
			SetColorResultRankNth(nRank, keta, col );		
		}
		else
		{
			SetTextureResultRankNth(nRank ,keta, value);
			SetColorResultRankNth(nRank, keta, col );			
		}

	}

}


