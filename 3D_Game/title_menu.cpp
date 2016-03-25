/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`�� �^�C�g�����j���[����
* �v���O������:	result_BG.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "title_menu.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define MAX_TITLE_MENU	(1)
#define POLYGON_SIZE_X	(500)//�|���S����SIZE X
#define POLYGON_SIZE_Y	(50)//�|���S����SIZE Y

#define POLYGON_TEXTURENAME_TITLE_MENU_0		"data/TEXTURE/TITLE/menu_PressEnter.png"
//#define POLYGON_TEXTURENAME_TITLE_MENU_1		"data/TEXTURE/TITLE/menu_BATTLE.png"
//#define POLYGON_TEXTURENAME_TITLE_MENU_2		"data/TEXTURE/TITLE/menu_RANK.png"


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexTitleMenu(LPDIRECT3DDEVICE9 pDevice);
void SetColorTitleMenu(int nMenuIdx, int R, int G, int B, int A);
void SetVertexTitleMenu(int nMenuIdx);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitleMenu = NULL;
LPDIRECT3DTEXTURE9 g_pTextureTitleMenu[MAX_TITLE_MENU] = {NULL};

TITLE_MENU	g_titleMenu[MAX_TITLE_MENU];

int g_menu;
float g_angle;


//=============================================================================
// ����������
//=============================================================================
HRESULT InitTitleMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_menu = 0;
	g_angle = 0;

	//menu_cooperate
	g_titleMenu[0].pos = D3DXVECTOR3( SCREEN_WIDTH/2, 550, 0.0f);
	g_titleMenu[0].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	g_titleMenu[0].col = D3DCOLOR_RGBA(255,255,255,255); 

	////menu_battle
	//g_titleMenu[1].pos = D3DXVECTOR3( 625, 550, 0.0f);
	//g_titleMenu[1].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	//g_titleMenu[1].col = D3DCOLOR_RGBA(255,255,255,255); 

	////menu_rank
	//g_titleMenu[2].pos = D3DXVECTOR3( 625, 650, 0.0f);
	//g_titleMenu[2].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	//g_titleMenu[2].col = D3DCOLOR_RGBA(255,255,255,255); 


	//���_���̍쐬
	if(FAILED(MakeVertexTitleMenu(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TITLE_MENU_0, &g_pTextureTitleMenu[0]);
	//D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TITLE_MENU_1, &g_pTextureTitleMenu[1]);
	//D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME_TITLE_MENU_2, &g_pTextureTitleMenu[2]);

	return S_OK;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTitleMenu(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferTitleMenu, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for(int nCntIcon = 0; nCntIcon < MAX_TITLE_MENU; nCntIcon++)
	{

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitleMenu[nCntIcon]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(
			D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
			nCntIcon*4,			//���[�h����ŏ��̒��_�C���f�b�N�X
			NUM_POLYGON				//�|���S���̐�
		);		
	}


}

//=============================================================================
// �I������
//=============================================================================
void UninitTitleMenu(void)
{
	for(int nCntIcon = 0; nCntIcon < MAX_TITLE_MENU; nCntIcon++)
	{
		if(g_pTextureTitleMenu[nCntIcon] != NULL)
		{
			g_pTextureTitleMenu[nCntIcon]->Release();
			g_pTextureTitleMenu[nCntIcon] = NULL;
		}
	}

	if(g_pVtxBufferTitleMenu != NULL)
	{
		g_pVtxBufferTitleMenu->Release();
		g_pVtxBufferTitleMenu = NULL;
	}	


}
/*******************************************************************************
�֐���:	HRESULT MakeVertexTitleMenu(LPDIRECT3DDEVICE9 pDevice)
����:	LPDIRECT3DDEVICE9 pDevice : Device�I�u�W�F�N�g
�߂�l:	HRESUL : ���������� ����I��:S_OK
����:	�w�i�̃|���S���̒��_���̍쐬�֐�
*******************************************************************************/
HRESULT MakeVertexTitleMenu(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_TITLE_MENU,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferTitleMenu,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTitleMenu->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntIcon = 0; nCntIcon < MAX_TITLE_MENU; nCntIcon++,pVtx+=4)
	{

		//���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_titleMenu[nCntIcon].pos.x - (g_titleMenu[nCntIcon].size.x/2), g_titleMenu[nCntIcon].pos.y - (g_titleMenu[nCntIcon].size.y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_titleMenu[nCntIcon].pos.x + (g_titleMenu[nCntIcon].size.x/2), g_titleMenu[nCntIcon].pos.y - (g_titleMenu[nCntIcon].size.y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_titleMenu[nCntIcon].pos.x - (g_titleMenu[nCntIcon].size.x/2), g_titleMenu[nCntIcon].pos.y + (g_titleMenu[nCntIcon].size.y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_titleMenu[nCntIcon].pos.x + (g_titleMenu[nCntIcon].size.x/2), g_titleMenu[nCntIcon].pos.y + (g_titleMenu[nCntIcon].size.y/2), 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_titleMenu[nCntIcon].col;
		pVtx[1].col = g_titleMenu[nCntIcon].col;
		pVtx[2].col = g_titleMenu[nCntIcon].col;
		pVtx[3].col = g_titleMenu[nCntIcon].col;

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
	}



	g_pVtxBufferTitleMenu->Unlock();

	return S_OK;
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateTitleMenu(void)
{

	//if(GetKeyboardTrigger(DIK_DOWN) || GetGamePadTrigger( 0, BUTTON_DOWN) )
	//{
	//	SetColorTitleMenu( g_menu, 255, 255, 255, 255);
	//	g_titleMenu[g_menu].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	//	SetVertexTitleMenu(g_menu);
	//	g_angle = 0;

	//	g_menu++;
	//	if(g_menu == MAX_TITLE_MENU) g_menu = 0;

	//	//SE
	//	SetSE(SOUND_LABEL_SE_SELECT);
	//}
	//if(GetKeyboardTrigger(DIK_UP) || GetGamePadTrigger( 0, BUTTON_UP) )
	//{
	//	SetColorTitleMenu( g_menu, 255, 255, 255, 255);
	//	g_titleMenu[g_menu].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
	//	SetVertexTitleMenu(g_menu);
	//	g_angle = 0;

	//	g_menu--;
	//	if(g_menu == -1) g_menu = MAX_TITLE_MENU - 1;

	//	//SE
	//	SetSE(SOUND_LABEL_SE_SELECT);
	//}
	
	//change color
	//SetColorTitleMenu( g_menu, 255, 255, 0, 255);

	//change size
	g_titleMenu[g_menu].size.x += sin(g_angle) * 0.5f;
	g_titleMenu[g_menu].size.y += sin(g_angle) * 0.5f;
	g_angle += 0.1f;
	if(g_angle >= D3DX_PI * 2) g_angle = 0;

	SetVertexTitleMenu(g_menu);
}



//=============================================================================
// �F�̐ݒ�
//=============================================================================
void SetColorTitleMenu(int nMenuIdx, int R, int G, int B, int A)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTitleMenu->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += nMenuIdx * NUM_VERTEX; 

	//color
	pVtx[0].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[1].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[2].col = D3DCOLOR_RGBA(R,G,B,A);
	pVtx[3].col = D3DCOLOR_RGBA(R,G,B,A);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferTitleMenu->Unlock();
	
}


//=============================================================================
// ���_�̐ݒ�
//=============================================================================
void SetVertexTitleMenu(int nMenuIdx)
{

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferTitleMenu->Lock( 0, 0, (void**)&pVtx, 0);

	pVtx += nMenuIdx * NUM_VERTEX; 

	//���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(g_titleMenu[nMenuIdx].pos.x - (g_titleMenu[nMenuIdx].size.x/2), g_titleMenu[nMenuIdx].pos.y - (g_titleMenu[nMenuIdx].size.y/2), 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_titleMenu[nMenuIdx].pos.x + (g_titleMenu[nMenuIdx].size.x/2), g_titleMenu[nMenuIdx].pos.y - (g_titleMenu[nMenuIdx].size.y/2), 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_titleMenu[nMenuIdx].pos.x - (g_titleMenu[nMenuIdx].size.x/2), g_titleMenu[nMenuIdx].pos.y + (g_titleMenu[nMenuIdx].size.y/2), 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_titleMenu[nMenuIdx].pos.x + (g_titleMenu[nMenuIdx].size.x/2), g_titleMenu[nMenuIdx].pos.y + (g_titleMenu[nMenuIdx].size.y/2), 0.0f);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferTitleMenu->Unlock();
}


//=============================================================================
// ���j���[�I�����擾
//=============================================================================
int GetTitleMenu(void)
{
	return g_menu;
}


