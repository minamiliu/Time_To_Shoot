/*******************************************************************************
*
* �^�C�g��:		�t�F�[�h����
* �v���O������:	fade.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "fade.h"
#include "message.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_POS_X	(0)//�|���S���̍���X
#define POLYGON_POS_Y	(0)//�|���S���̍���Y
#define POLYGON_SIZE_X	(SCREEN_WIDTH)//�|���S����SIZE X
#define POLYGON_SIZE_Y	(SCREEN_HEIGHT)//�|���S����SIZE Y
//#define POLYGON_TEXTURENAME "data/TEXTURE/BG/xxx.png"


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);
void SetColorFade(int R, int G, int B, int A);
void SetFade(FADE fade);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferFade = NULL;
LPDIRECT3DTEXTURE9 g_pTextureFade = NULL;

FADE g_fade;
int g_alpha;
MODE g_nextMode;
GAME_STEP g_nextGameMode;
STAGE_MODE g_nextStageMode;
bool g_changeModeFlag;
bool g_changeGameFlag;
bool g_changeStageFlag;



//=============================================================================
// ����������
//=============================================================================
HRESULT InitFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���̍쐬
	if(FAILED(MakeVertexFade(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	//D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME, &g_pTextureFade);

	g_fade = FADE_NONE;
	g_alpha = 0; //����
	g_changeModeFlag = false;
	g_changeGameFlag = true;

	return S_OK;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFade(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferFade, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureFade);

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,						//���[�h����ŏ��̒��_�C���f�b�N�X
		NUM_POLYGON				//�|���S���̐�
	);

}

//=============================================================================
// �I������
//=============================================================================
void UninitFade(void)
{
	if(g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
	if(g_pVtxBufferFade != NULL)
	{
		g_pVtxBufferFade->Release();
		g_pVtxBufferFade = NULL;
	}
}
/*******************************************************************************
�֐���:	HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferFade,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferFade->Lock( 0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DCOLOR_RGBA(0,0,0,0);
	pVtx[1].col = D3DCOLOR_RGBA(0,0,0,0);
	pVtx[2].col = D3DCOLOR_RGBA(0,0,0,0);
	pVtx[3].col = D3DCOLOR_RGBA(0,0,0,0);

	//�e�N�X�`�����W�w��
	pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
	pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
	pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
	pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);

	g_pVtxBufferFade->Unlock();

	return S_OK;
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFade(void)
{
	switch(g_fade)
	{
		case FADE_OUT:
			g_alpha += 5;
			if(g_alpha >= 255)
			{
				g_alpha = 255;
				SetFade(FADE_CHANGE);
			}
			SetColorFade(0, 0, 0, g_alpha);
			break;
		case FADE_CHANGE:
			//all msg clear
			ClearMessage();

			if(g_changeModeFlag)
			{
				SetMode(g_nextMode);
				g_changeModeFlag = false;
			}
			if(g_changeGameFlag)
			{
				SetGameStep(g_nextGameMode);
				g_changeGameFlag = false;
			}
			if(g_changeStageFlag)
			{
				NextStage();
				SetGameStep(STEP_PLAY);
				g_changeStageFlag = false;
			}
			SetFade(FADE_IN);
			break;
		case FADE_IN:
			g_alpha -= 5;
			if(g_alpha <= 0)
			{
				g_alpha = 0;
				SetFade(FADE_NONE);
			}
			SetColorFade(0, 0, 0, g_alpha);
			break;
		case FADE_NONE:

			break;
	}
}
//=============================================================================
// �F��ς���
//=============================================================================
void SetColorFade(int R, int G, int B, int A)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferFade->Lock(0, 0, (void**)&pVtx, 0);

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferFade->Unlock();
	
}
//=============================================================================
// �t�F�[�h�A�E�g
//=============================================================================
void FadeOutToNext(MODE mode)
{
	if(g_fade == FADE_NONE)
	{
		g_fade = FADE_OUT;
		g_nextMode = mode;
		g_changeModeFlag = true;	
	}
}
//=============================================================================
// �t�F�[�h�A�E�g
//=============================================================================
void FadeOutToNext(GAME_STEP mode)
{
	if(g_fade == FADE_NONE)
	{
		g_fade = FADE_OUT;
		g_nextGameMode = mode;
		g_changeGameFlag = true;	
	}
}
//=============================================================================
// �t�F�[�h�A�E�g
//=============================================================================
void FadeOutToNext(STAGE_MODE mode)
{
	if(g_fade == FADE_NONE)
	{
		g_fade = FADE_OUT;
		g_nextStageMode = mode;
		g_changeStageFlag = true;	
	}
}
//=============================================================================
// �t�F�[�h���[�h��ς���
//=============================================================================
void SetFade(FADE fade)
{
	g_fade = fade;
}
