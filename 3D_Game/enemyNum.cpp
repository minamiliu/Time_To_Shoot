/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`���G�̎c�@�\������
* �v���O������:	enemyNum.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "enemyNum.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number003.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_SIZE_X				(80)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(100)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(10)							// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define NUM_OBJECT					(2)								// �I�u�W�F�N�g�̐�

#define	POLYGON_POS_X				(POLYGON_SIZE_X)				//�|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(SCREEN_HEIGHT - POLYGON_SIZE_Y)	// �|���S���̊�ʒu(�x���W)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexEnemyNum(LPDIRECT3DDEVICE9 pDevice);
void SetTextureEnemyNum( int nIdx, int number);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferEnemyNum = NULL;
LPDIRECT3DTEXTURE9 g_pTextureEnemyNum = NULL;

ENEMY_NUM			g_EnemyNum; //Time�̃��[�N

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemyNum(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//EnemyNum�̏�����
	g_EnemyNum.pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_EnemyNum.value = 0;

	//���_���̍쐬
	if(FAILED(MakeVertexEnemyNum(pDevice)))
	{
		return E_FAIL;
	}


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,	// �t�@�C���̖��O
								&g_pTextureEnemyNum);	// �ǂݍ��ރ������[

	return S_OK;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemyNum(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferEnemyNum, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEnemyNum);

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
void UninitEnemyNum(void)
{
	if(g_pTextureEnemyNum != NULL)
	{
		g_pTextureEnemyNum->Release();
		g_pTextureEnemyNum = NULL;
	}
	if(g_pVtxBufferEnemyNum != NULL)
	{
		g_pVtxBufferEnemyNum->Release();
		g_pVtxBufferEnemyNum = NULL;
	}
}

/*******************************************************************************
�֐���:	MakeVertexEnemyNum(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexEnemyNum(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferEnemyNum,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferEnemyNum->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++, pVtx += NUM_VERTEX)
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
		pVtx[0].vtx = D3DXVECTOR3(g_EnemyNum.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_EnemyNum.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_EnemyNum.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_EnemyNum.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_EnemyNum.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_EnemyNum.pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_EnemyNum.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_EnemyNum.pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferEnemyNum->Unlock();

	return S_OK;
}



//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemyNum(void)
{
	
}



/*******************************************************************************
�֐���:	void SetTextureEnemyNum( int nIdx, int number) 
����:	 int nTdx�F�����ڂ�, int number�F�\�����鐔
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureEnemyNum( int nIdx, int number) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferEnemyNum->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (NUM_OBJECT - nIdx) * NUM_VERTEX;

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
		g_pVtxBufferEnemyNum->Unlock();
	}
}

/*******************************************************************************
�֐���:	void ChangeEnemyNum(int value)
����:	int value�F��������_��
�߂�l:	�Ȃ�
����:	���_����ύX����
*******************************************************************************/
void ChangeEnemyNum(int value)
{
	g_EnemyNum.value += value;
	if( g_EnemyNum.value < 0) g_EnemyNum.value = 0; 

	SetEnemyNum( g_EnemyNum.value);
}

/*******************************************************************************
* �G�̐ݒu
*******************************************************************************/
void SetEnemyNum(int value)
{
	g_EnemyNum.value = value;
	if( value < 0) value = 0;

	int number;
	for(int keta = 1; keta <= NUM_OBJECT; keta++)
	{
		number = value % (int)powf( 10.0f, keta) / powf( 10.0f, keta-1);
		SetTextureEnemyNum(keta,number);
	}
}

/*******************************************************************************
* �G�̃f�[�^���擾
*******************************************************************************/
int GetEnemyNum(void)
{
	return g_EnemyNum.value;
}
