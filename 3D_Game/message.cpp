/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`�� ���b�Z�[�W����
* �v���O������:	message.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "message.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define POLYGON_POS_X	(SCREEN_WIDTH/2)
#define POLYGON_POS_Y	(SCREEN_HEIGHT/2)

#define POLYGON_TEXTURENAME_MSG_0		"data/TEXTURE/MSG/GAME_OVER.png"
#define POLYGON_TEXTURENAME_MSG_1		"data/TEXTURE/MSG/STAGE_CLEAR.png"
#define POLYGON_TEXTURENAME_MSG_2		"data/TEXTURE/MSG/RANKING.png"
#define POLYGON_TEXTURENAME_MSG_3		"data/TEXTURE/MSG/PAUSE.png"
#define POLYGON_TEXTURENAME_MSG_4		"data/TEXTURE/MSG/TIME_UP.png"
#define POLYGON_TEXTURENAME_MSG_5		"data/TEXTURE/MSG/TARGET.png"
#define POLYGON_TEXTURENAME_MSG_6		"data/TEXTURE/MSG/Exclamation.png"

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexMessage(LPDIRECT3DDEVICE9 pDevice);
void SetColorMessage(int nMenuIdx, int R, int G, int B, int A);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferMessage = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMessage[MSG_MAX] = {NULL};

MESSAGE	g_msg[MSG_MAX];




//=============================================================================
// ����������
//=============================================================================
HRESULT InitMessage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntMsg = 0; nCntMsg < MSG_MAX; nCntMsg++)
	{
		g_msg[nCntMsg].bUse = false;
		g_msg[nCntMsg].cntFrame = -1;
	}

	//GAME_OVER
	g_msg[0].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[0].size = D3DXVECTOR3( 900, 100, 0.0f);
	g_msg[0].col = D3DCOLOR_RGBA(255,255,0,255); 

	//STAGE_CLEAR
	g_msg[1].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[1].size = D3DXVECTOR3( 800, 100, 0.0f);
	g_msg[1].col = D3DCOLOR_RGBA(255,255,0,255); 

	//RANKING
	g_msg[2].pos = D3DXVECTOR3( POLYGON_POS_X, 100, 0.0f);
	g_msg[2].size = D3DXVECTOR3( 600, 100, 0.0f);
	//g_msg[2].col = D3DCOLOR_RGBA(255,0,0,255); 
	g_msg[2].col = D3DCOLOR_RGBA(30,225,180,255);

	//PAUSE
	g_msg[3].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[3].size = D3DXVECTOR3( 500, 100, 0.0f);
	g_msg[3].col = D3DCOLOR_RGBA(255,255,0,255); 

	//TIME_UP
	g_msg[4].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y, 0.0f);
	g_msg[4].size = D3DXVECTOR3( 700, 100, 0.0f);
	g_msg[4].col = D3DCOLOR_RGBA(255,255,0,255); 

	//TARGET
	g_msg[5].pos = D3DXVECTOR3( 120, SCREEN_HEIGHT - 200, 0.0f);
	g_msg[5].size = D3DXVECTOR3( 230, 80, 0.0f);
	g_msg[5].col = D3DCOLOR_RGBA(255,255,100,255);

	//EXCLAMATION MARK
	g_msg[6].pos = D3DXVECTOR3( SCREEN_WIDTH *3/4 , SCREEN_HEIGHT/2, 0.0f);
	g_msg[6].size = D3DXVECTOR3( 300, 300, 0.0f);
	g_msg[6].col = D3DCOLOR_RGBA(255,255,0,255);

	//���_���̍쐬
	if(FAILED(MakeVertexMessage(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_0, &g_pTextureMessage[0]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_1, &g_pTextureMessage[1]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_2, &g_pTextureMessage[2]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_3, &g_pTextureMessage[3]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_4, &g_pTextureMessage[4]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_5, &g_pTextureMessage[5]);
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_MSG_6, &g_pTextureMessage[6]);

	return S_OK;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMessage(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferMessage, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntMsg = 0; nCntMsg < MSG_MAX; nCntMsg++)
	{
		if(g_msg[nCntMsg].bUse)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMessage[nCntMsg]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntMsg*4,			//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);		
		}
	}


}

//=============================================================================
// �I������
//=============================================================================
void UninitMessage(void)
{
	for(int nCntMsg = 0; nCntMsg < MSG_MAX; nCntMsg++)
	{
		if(g_pTextureMessage[nCntMsg] != NULL)
		{
			g_pTextureMessage[nCntMsg]->Release();
			g_pTextureMessage[nCntMsg] = NULL;
		}
	}

	if(g_pVtxBufferMessage != NULL)
	{
		g_pVtxBufferMessage->Release();
		g_pVtxBufferMessage = NULL;
	}	


}
/*******************************************************************************
�֐���:	HRESULT MakeVertexMessage(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexMessage(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MSG_MAX,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferMessage,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferMessage->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntMsg = 0; nCntMsg < MSG_MAX; nCntMsg++,pVtx+=4)
	{

		//���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_msg[nCntMsg].pos.x - (g_msg[nCntMsg].size.x/2), g_msg[nCntMsg].pos.y - (g_msg[nCntMsg].size.y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_msg[nCntMsg].pos.x + (g_msg[nCntMsg].size.x/2), g_msg[nCntMsg].pos.y - (g_msg[nCntMsg].size.y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_msg[nCntMsg].pos.x - (g_msg[nCntMsg].size.x/2), g_msg[nCntMsg].pos.y + (g_msg[nCntMsg].size.y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_msg[nCntMsg].pos.x + (g_msg[nCntMsg].size.x/2), g_msg[nCntMsg].pos.y + (g_msg[nCntMsg].size.y/2), 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_msg[nCntMsg].col;
		pVtx[1].col = g_msg[nCntMsg].col;
		pVtx[2].col = g_msg[nCntMsg].col;
		pVtx[3].col = g_msg[nCntMsg].col;

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferMessage->Unlock();

	return S_OK;
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateMessage(void)
{
	for(int nCntMsg = 0; nCntMsg < MSG_MAX; nCntMsg++)
	{
		if( g_msg[nCntMsg].cntFrame > 0)
		{
			g_msg[nCntMsg].cntFrame--;

		}
		else if( g_msg[nCntMsg].cntFrame == 0)
		{
			g_msg[nCntMsg].cntFrame = -1;
			g_msg[nCntMsg].bUse = false;
		}
		
	}
}
//=============================================================================
// �F��ς���
//=============================================================================
void SetColorMessage(int nMenuIdx, int R, int G, int B, int A)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferMessage->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nMenuIdx * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferMessage->Unlock();
	
}
//=============================================================================
// ���b�Z�[�W�̐ݒu
//=============================================================================
void SetMessage(int nMsgIdx)
{
	g_msg[nMsgIdx].bUse = true;
}
//=============================================================================
// ���b�Z�[�W�̃N���A
//=============================================================================
void ClearMessage(int nMsgIdx)
{
	g_msg[nMsgIdx].bUse = false;
}
//=============================================================================
// �S�Ẵ��b�Z�[�W���N���A
//=============================================================================
void ClearMessage(void)
{
	for(int nCntMsg = 0; nCntMsg < MSG_MAX; nCntMsg++)
	{
		g_msg[nCntMsg].bUse = false;
	}
}
//=============================================================================
// ���b�Z�[�W��_�ł���
//=============================================================================
void SetFlashMessage(int nMsgIdx)
{

	g_msg[nMsgIdx].bUse = true;
	g_msg[nMsgIdx].cntFrame = 60;
}