/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`�� ���U���g�̃����L���O�̃X�R�[�A�\������
* �v���O������:	result_BG.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "result_rank_tex.h"
#include "result_rank.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number003.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(250)							// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(150)							// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(50)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(100)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(10)							// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define NUM_DIGIT					(6)
#define MAX_NUM_BOUND				(999999)
#define NUM_OBJECT					(NUM_DIGIT*MAX_RANK)			// �I�u�W�F�N�g�̐�

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexResultRankTex(LPDIRECT3DDEVICE9 pDevice);
void SetTextureResultRankTex( int nRank, int nIdx, int number);
void SetColorResultRankTex( int nRank, int nIdx, D3DCOLOR col);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferRankScore = NULL;
LPDIRECT3DTEXTURE9 g_pTextureRankScore = NULL;

RESULT_RANK_TEX	g_numRankScore[MAX_RANK]; //score�̃��[�N




//=============================================================================
// ����������
//=============================================================================
HRESULT InitResultRankTex()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//�l�����̂݁A������
	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{		
		g_numRankScore[nCntRank].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_numRankScore[nCntRank].score = 0;

	}
	for(int nCntRank = 0; nCntRank < MAX_RANK/2; nCntRank++)
	{
		g_numRankScore[nCntRank].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y + nCntRank*110, 0.0f);  //��5
		g_numRankScore[nCntRank + MAX_RANK/2].pos = D3DXVECTOR3( POLYGON_POS_X + 550, POLYGON_POS_Y + nCntRank*110, 0.0f); //�E5

		switch(nCntRank)
		{
			case 0:
				g_numRankScore[nCntRank].col = g_numRankScore[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,0,0,255); //red
				break;
			case 1:
				g_numRankScore[nCntRank].col = g_numRankScore[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,128,0,255); //orange
				break;
			case 2:
				g_numRankScore[nCntRank].col = g_numRankScore[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
				break;
			case 3:
				g_numRankScore[nCntRank].col = g_numRankScore[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(0,255,0,255); //green
				break;
			case 4:
				g_numRankScore[nCntRank].col = g_numRankScore[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(0,0,255,255); //blue
				break;
		}	
	}

	g_numRankScore[MAX_RANK-1].pos = D3DXVECTOR3( -1000, -1000 , 0.0f);


	//���_���̍쐬
	if(FAILED(MakeVertexResultRankTex(pDevice)))
	{
		return E_FAIL;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,	// �t�@�C���̖��O
								&g_pTextureRankScore);	// �ǂݍ��ރ������[

	return S_OK;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultRankTex(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferRankScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRankScore);

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
void UninitResultRankTex(void)
{
	if(g_pTextureRankScore != NULL)
	{
		g_pTextureRankScore->Release();
		g_pTextureRankScore = NULL;
	}
	if(g_pVtxBufferRankScore!= NULL)
	{
		g_pVtxBufferRankScore->Release();
		g_pVtxBufferRankScore = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexResultRankTex(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexResultRankTex(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferRankScore,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferRankScore->Lock( 0, 0, (void**)&pVtx, 0);

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
			pVtx[0].col = g_numRankScore[nCntRank].col;
			pVtx[1].col = g_numRankScore[nCntRank].col;
			pVtx[2].col = g_numRankScore[nCntRank].col;
			pVtx[3].col = g_numRankScore[nCntRank].col;

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(g_numRankScore[nCntRank].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numRankScore[nCntRank].pos.y , 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_numRankScore[nCntRank].pos.x  + g_numRankScore[nCntRank].size.x +  nCntDigit*POLYGON_SIZE_X, g_numRankScore[nCntRank].pos.y , 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_numRankScore[nCntRank].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numRankScore[nCntRank].pos.y + g_numRankScore[nCntRank].size.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_numRankScore[nCntRank].pos.x  + g_numRankScore[nCntRank].size.x + nCntDigit*POLYGON_SIZE_X, g_numRankScore[nCntRank].pos.y + g_numRankScore[nCntRank].size.y, 0.0f);

			//�e�N�X�`�����W�w��
			pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
			pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
			pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
			pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		}	
	}

	g_pVtxBufferRankScore->Unlock();

	return S_OK;
}



//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultRankTex(void)
{

}



/*******************************************************************************
�֐���:	void SetTextureResultRankTex( int nRank, int nIdx, int number) 
����:	 int nTdx�F�����ڂ�, int number�F�\�����鐔
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureResultRankTex( int nRank, int nIdx, int number) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferRankScore->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pVtxBufferRankScore->Unlock();
	}
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void SetColorResultRankTex( int nRank, int nIdx, D3DCOLOR col) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferRankScore->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nRank * NUM_DIGIT * NUM_VERTEX);

		//color
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// ���_�f�[�^���A�����b�N����
		g_pVtxBufferRankScore->Unlock();
	}
}

/*******************************************************************************
�֐���:	void ChangeResultRankTex(int nRank, int value, D3DCOLOR col)
����:	int value�F��������_��
�߂�l:	�Ȃ�
����:	���_����ύX����
*******************************************************************************/
void ChangeResultRankTex(int nRank, int value, D3DCOLOR col)
{
	g_numRankScore[nRank].score += value;

	if(g_numRankScore[nRank].score >= MAX_NUM_BOUND) g_numRankScore[nRank].score = MAX_NUM_BOUND; //�ő�l

	int number = 0;

	for(int keta = 1; keta <= NUM_DIGIT; keta++)
	{
		number = g_numRankScore[nRank].score % (int)powf( 10.0f, keta) / powf( 10.0f, keta-1);
		SetTextureResultRankTex(nRank ,keta, number);
		//SetColorResultRankTex(nRank, keta, col );
	}
}


