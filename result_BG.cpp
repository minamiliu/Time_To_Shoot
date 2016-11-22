/*******************************************************************************
*
* �^�C�g��:		���U���g�̔w�i����
* �v���O������:	result_BG.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "result_BG.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//�|���S���̍���X
#define POLYGON_POS_Y	(0)//�|���S���̍���Y
#define POLYGON_SIZE_X	(SCREEN_WIDTH)//�|���S����SIZE X
#define POLYGON_SIZE_Y	(SCREEN_HEIGHT)//�|���S����SIZE Y
#define POLYGON_TEXTURENAME "data/TEXTURE/BG_Ranking.jpg"
#define NUM_TEX_X	(4.0f)
#define NUM_TEX_Y	(3.0f)


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexResultBG(LPDIRECT3DDEVICE9 pDevice);
void SetVertexBG(void);
/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultBG = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultBG = NULL;

D3DXVECTOR3 g_scroll;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitResultBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̍쐬
	if(FAILED(MakeVertexResultBG(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME, &g_pTextureResultBG);

	g_scroll = D3DXVECTOR3(0,0,0);

	return S_OK;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultBG(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferResultBG, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultBG);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,						//���[�h����ŏ��̒��_�C���f�b�N�X
		NUM_POLYGON				//�|���S���̐�
	);

}
/*******************************************************************************
�֐���:	void UninitBG(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�w�i�̃|���S���̊J���֐�
*******************************************************************************/
void UninitResultBG(void)
{
	if(g_pTextureResultBG != NULL)
	{
		g_pTextureResultBG->Release();
		g_pTextureResultBG = NULL;
	}
	if(g_pVtxBufferResultBG != NULL)
	{
		g_pVtxBufferResultBG->Release();
		g_pVtxBufferResultBG = NULL;
	}
}
/*******************************************************************************
�֐���:	HRESULT MakeVertexResultBG(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexResultBG(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferResultBG,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferResultBG->Lock( 0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(POLYGON_POS_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(POLYGON_POS_X + POLYGON_SIZE_X, POLYGON_POS_Y + POLYGON_SIZE_Y, 0.0f);


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

	////�e�N�X�`�����W�w��
	//pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
	//pVtx[1].tex = D3DXVECTOR2(NUM_TEX_X, 0.0F);
	//pVtx[2].tex = D3DXVECTOR2(0.0F, NUM_TEX_Y);
	//pVtx[3].tex = D3DXVECTOR2(NUM_TEX_X, NUM_TEX_Y);

	//�e�N�X�`�����W�w��
	pVtx[0].tex = D3DXVECTOR2(g_scroll.x, g_scroll.y);
	pVtx[1].tex = D3DXVECTOR2(g_scroll.x + NUM_TEX_X, g_scroll.y + 0.0F);
	pVtx[2].tex = D3DXVECTOR2(g_scroll.x + 0.0F, g_scroll.y + NUM_TEX_Y);
	pVtx[3].tex = D3DXVECTOR2(g_scroll.x + NUM_TEX_X, g_scroll.y + NUM_TEX_Y);

	g_pVtxBufferResultBG->Unlock();

	return S_OK;
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultBG(void)
{
	g_scroll.y += 0.001;
	SetVertexBG();
}

//=============================================================================
// ���_�f�[�^�̐ݒ�
//=============================================================================
void SetVertexBG(void)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferResultBG->Lock( 0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�w��
	pVtx[0].tex = D3DXVECTOR2(g_scroll.x, g_scroll.y);
	pVtx[1].tex = D3DXVECTOR2(g_scroll.x + NUM_TEX_X, g_scroll.y + 0.0F);
	pVtx[2].tex = D3DXVECTOR2(g_scroll.x + 0.0F, g_scroll.y + NUM_TEX_Y);
	pVtx[3].tex = D3DXVECTOR2(g_scroll.x + NUM_TEX_X, g_scroll.y + NUM_TEX_Y);

	g_pVtxBufferResultBG->Unlock();
}