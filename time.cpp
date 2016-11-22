/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`�� ���Ԃ̐����\������
* �v���O������:	time.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "time.h"
#include "fade.h"
#include "message.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)


#define	TEXTURE_POLYGON				"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	POLYGON_SIZE_X				(35)							// �|���S���̕�
#define	POLYGON_SIZE_Y				(70)							// �|���S���̍���

#define	TEX_PATTERN_DIVIDE_X		(10)							// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�w����)
#define	TEX_PATTERN_DIVIDE_Y		(1)								// �A�j���[�V�����p�^�[���̃e�N�X�`�����ł̕�����(�x����)

#define	TEX_PATTERN_SIZE_X			(1.0f/TEX_PATTERN_DIVIDE_X)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�w����)(1.0f/X����������)
#define	TEX_PATTERN_SIZE_Y			(1.0f/TEX_PATTERN_DIVIDE_Y)		// �P�p�^�[���̃e�N�X�`���T�C�Y(�x����)(1.0f/Y����������)

#define	NUM_ANIM_PATTERN			(TEX_PATTERN_DIVIDE_X*TEX_PATTERN_DIVIDE_Y)	// �A�j���[�V�����̃p�^�[����(X�����������~Y����������)
#define	TIME_CHANGE_PATTERN			(10)							// �A�j���[�V�����̐؂�ւ��^�C�~���O(�t���[����)

#define	VALUE_MOVE					(0.0f)							// �|���S���̈ړ���
#define NUM_OBJECT					(3)								// �I�u�W�F�N�g�̐�
#define STAGE_TIME					(999)

#define	POLYGON_POS_X				(SCREEN_WIDTH/2 - NUM_OBJECT/2*POLYGON_SIZE_X)	//�|���S���̊�ʒu(�w���W)
#define	POLYGON_POS_Y				(70)											// �|���S���̊�ʒu(�x���W)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);
void SetTextureTime( int nIdx, int number);
HRESULT ChangeVertexColor(LPDIRECT3DDEVICE9 pDevice, int R, int G, int B);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTime = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;

TIME			g_time; //Time�̃��[�N



//=============================================================================
// ����������
//=============================================================================
HRESULT InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Time�̏�����
	g_time.pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_time.time = STAGE_TIME;
	g_time.fCounterSeconds = 0.0f;

	//���_���̍쐬
	if(FAILED(MakeVertexTime(pDevice)))
	{
		return E_FAIL;
	}

	SetTime(STAGE_TIME);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_POLYGON,	// �t�@�C���̖��O
								&g_pTextureTime);	// �ǂݍ��ރ������[

	return S_OK;
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawTime(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferTime, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

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
void UninitTime(void)
{
	if(g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}
	if(g_pVtxBufferTime != NULL)
	{
		g_pVtxBufferTime->Release();
		g_pVtxBufferTime = NULL;
	}
}

/*******************************************************************************
�֐���:	HRESULT MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*NUM_OBJECT,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferTime,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTime->Lock( 0, 0, (void**)&pVtx, 0);

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
		pVtx[0].vtx = D3DXVECTOR3(g_time.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_time.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_time.pos.x - (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_time.pos.x + (POLYGON_SIZE_X/2) + nCntPlace*POLYGON_SIZE_X, g_time.pos.y + (POLYGON_SIZE_Y/2), 0.0f);

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X) );
		pVtx[1].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1) , TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X));
		pVtx[2].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X), TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
		pVtx[3].tex = D3DXVECTOR2( TEX_PATTERN_SIZE_X * (0 % TEX_PATTERN_DIVIDE_X + 1),  TEX_PATTERN_SIZE_Y * (0 / TEX_PATTERN_DIVIDE_X + 1));
	}



	g_pVtxBufferTime->Unlock();

	return S_OK;
}



//=============================================================================
// �X�V����
//=============================================================================
void UpdateTime(float fTimeSpeed)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	g_time.fCounterSeconds += 1 * fTimeSpeed;

	if(g_time.fCounterSeconds >= 60)
	{
		if(g_time.time <= 10)
		{
			//���_�J���[�̕ύX
			ChangeVertexColor(pDevice,255,0,0);
		}
		
		//time's up
		if(g_time.time <= 0)
		{
			FadeOutToNext(MODE_RESULT);
			SetMessage(MSG_TIME_UP);
		}
		else
		{
			g_time.time--;
			SetTime(g_time.time);			
		}
	
		g_time.fCounterSeconds = 0.0f;
	}
}



/*******************************************************************************
�֐���:	void SetTextureTime( int nIdx, int number)  
����:	 int nTdx�F�����ڂ�, int number�F�\�����鐔
�߂�l:	�Ȃ�
����:	�e�N�X�`�����W�̐ݒ�
*******************************************************************************/
void SetTextureTime( int nIdx, int number) 
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_2D *pVtx;
		float fPosXLeft, fPosXRight;
		float fPosYUp, fPosYDown;
		

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pVtxBufferTime->Lock(0, 0, (void**)&pVtx, 0);

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
		g_pVtxBufferTime->Unlock();
	}
}

/*******************************************************************************
�֐���:	void SetTime(int value)
����:	int value�F��������_��
�߂�l:	�Ȃ�
����:	���_����ύX����
*******************************************************************************/
void SetTime(int value)
{

	int number;

	for(int keta = 1; keta <= NUM_OBJECT; keta++)
	{
		number = value % (int)powf( 10.0f, keta) / powf( 10.0f, keta-1);
		SetTextureTime(keta,number);
	}
}

//=============================================================================
// �F��ς���
//=============================================================================
HRESULT ChangeVertexColor(LPDIRECT3DDEVICE9 pDevice, int R, int G, int B)
{

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTime->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntPlace = 0; nCntPlace < NUM_OBJECT; nCntPlace++, pVtx += NUM_VERTEX)
	{
		//color
		pVtx[0].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[1].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[2].col = D3DCOLOR_RGBA(R,G,B,255);
		pVtx[3].col = D3DCOLOR_RGBA(R,G,B,255);
	}



	g_pVtxBufferTime->Unlock();

	return S_OK;
}
//=============================================================================
// ���ԃf�[�^���擾
//=============================================================================
int GetTime(void)
{
	return g_time.time;
}
