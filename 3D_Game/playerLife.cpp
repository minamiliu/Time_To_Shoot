/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`�� �v���C���[�̃��C�t����
* �v���O������:	playerLife.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "playerLife.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/UI/PlayerLife.png"// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_POS_X				(25)							// �|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(25)							// �|���S���̊�ʒu(�x���W)
#define	POLYGON_SIZE_X				(100)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(100)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(2)							// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define NUM_DIGIT					(3)
#define NUM_OBJECT					(NUM_DIGIT*MAX_PLAYER)			// �I�u�W�F�N�g�̐�


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexNumLife(LPDIRECT3DDEVICE9 pDevice);
//void SetVertexScore(void);
void SetTextureNumLife( int nPlayerIdx, int nIdx, int number);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferNumLife = NULL;
LPDIRECT3DTEXTURE9 g_pTextureNumLife = NULL;

NUM_LIFE	g_numLife[MAX_PLAYER];



//=============================================================================
// ����������
//=============================================================================
HRESULT InitNumLife()
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//bool *pPlayerList = GetPlayerList();

	//P1 life
	g_numLife[0].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_numLife[0].col = D3DCOLOR_RGBA(255, 50, 50,255); //yellow 


	//�l�����̂݁A������
	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_numLife[nCntPlayer].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_numLife[nCntPlayer].num = 3;
	}

	//���_���̍쐬
	if(FAILED(MakeVertexNumLife(pDevice)))
	{
		return E_FAIL;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,	// �t�@�C���̖��O
								&g_pTextureNumLife);	// �ǂݍ��ރ������[

	return S_OK;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawNumLife(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferNumLife, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureNumLife);

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
void UninitNumLife(void)
{
	if(g_pTextureNumLife != NULL)
	{
		g_pTextureNumLife->Release();
		g_pTextureNumLife = NULL;
	}
	if(g_pVtxBufferNumLife!= NULL)
	{
		g_pVtxBufferNumLife->Release();
		g_pVtxBufferNumLife = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexNumLife(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexNumLife(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferNumLife,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferNumLife->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		for(int nCntDigit = 0; nCntDigit < NUM_DIGIT; nCntDigit++, pVtx += NUM_VERTEX)
		{

			//rhw
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;


			//color
			pVtx[0].col = g_numLife[nCntPlayer].col;
			pVtx[1].col = g_numLife[nCntPlayer].col;
			pVtx[2].col = g_numLife[nCntPlayer].col;
			pVtx[3].col = g_numLife[nCntPlayer].col;

			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(g_numLife[nCntPlayer].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numLife[nCntPlayer].pos.y , 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_numLife[nCntPlayer].pos.x  + g_numLife[nCntPlayer].size.x +  nCntDigit*POLYGON_SIZE_X, g_numLife[nCntPlayer].pos.y , 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_numLife[nCntPlayer].pos.x  + nCntDigit*POLYGON_SIZE_X, g_numLife[nCntPlayer].pos.y + g_numLife[nCntPlayer].size.y, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_numLife[nCntPlayer].pos.x  + g_numLife[nCntPlayer].size.x + nCntDigit*POLYGON_SIZE_X, g_numLife[nCntPlayer].pos.y + g_numLife[nCntPlayer].size.y, 0.0f);

			//�e�N�X�`�����W�w��
			pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
			pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
			pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
			pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		}	
	}

	g_pVtxBufferNumLife->Unlock();

	return S_OK;
}



//=============================================================================
// �X�V����
//=============================================================================
void UpdateNumLife(void)
{

}



/*******************************************************************************
�֐���:	void SetTextureNumLife( int nPlayerIdx, int nIdx, int number)
����:	 int nTdx�F�����ڂ�, int number�F�\�����鐔
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureNumLife( int nPlayerIdx, int nIdx, int number) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferNumLife->Lock(0, 0, (void**)&pVtx, 0);

		//pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;
		pVtx += (NUM_DIGIT - nIdx) * NUM_VERTEX + (nPlayerIdx * NUM_DIGIT * NUM_VERTEX);

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
		g_pVtxBufferNumLife->Unlock();
	}
}

/*******************************************************************************
�֐���:	void ChangeNumLife(int nPlayerIdx, int value)
����:	int value�F��������_��
�߂�l:	�Ȃ�
����:	���_����ύX����
*******************************************************************************/
void ChangeNumLife(int nPlayerIdx, int value)
{
	g_numLife[nPlayerIdx].num = value;

	if(g_numLife[nPlayerIdx].num > NUM_DIGIT ) g_numLife[nPlayerIdx].num = NUM_DIGIT; //�ő�l
	if(g_numLife[nPlayerIdx].num < 0 ) g_numLife[nPlayerIdx].num = 0; //�ŏ��l

	int number = g_numLife[nPlayerIdx].num;
	for(int keta = NUM_DIGIT; keta >= 1; keta--)
	{
		if(number > 0)
		{
			SetTextureNumLife(nPlayerIdx , keta , 1);
			number--;
		}		
		else
		{
			SetTextureNumLife(nPlayerIdx , keta , 0);
		}
	}
}
