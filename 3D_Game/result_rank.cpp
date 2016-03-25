/*******************************************************************************
*
* �^�C�g��:		���U���g�̃����L���O����
* �v���O������:	result_rank.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include <stdio.h>
#include "result_rank.h"
#include "result_rank_tex.h"
#include "result_rank_nth.h"
#include "time.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)
#define POLYGON_SIZE_X	(100)//�|���S����SIZE X
#define POLYGON_SIZE_Y	(100)//�|���S����SIZE Y
#define POLYGON_POS_X	(50 + POLYGON_SIZE_X/2)//�|���S���̍���X
#define POLYGON_POS_Y	(150 + POLYGON_SIZE_Y/2)//�|���S���̍���Y

#define POLYGON_TEXTURENAME "data/TEXTURE/RESULT/CROWN2.png"

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
void ScoreFileReadWrite(void);
void BubbleSort(int data[], int size);
HRESULT MakeVertexResultPlayer(LPDIRECT3DDEVICE9 pDevice);
void SetVertexResultRank(int nRankIdx);
/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/

LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTextureResultPlayer = NULL;

RESULT_RANK_PLAYER g_resultPlayer[MAX_RANK];

int rank[MAX_RANK];
int g_playerRank;

const int key = 0x45bf6c7d; //�C�ӂ̈Í����L�[
float g_angle2;



//=============================================================================
// ����������
//=============================================================================
HRESULT InitResultRank(void)
{
	//�O���֐��̏�����
	InitResultRankNth();
	InitResultRankTex();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{		
		g_resultPlayer[nCntRank].size = D3DXVECTOR3( POLYGON_SIZE_X, POLYGON_SIZE_Y, 0.0f);
		g_resultPlayer[nCntRank].bUse = false;
		g_resultPlayer[nCntRank].col = D3DCOLOR_RGBA(255,255,0,255); //yellow

	}
	for(int nCntRank = 0; nCntRank < MAX_RANK/2; nCntRank++)
	{
		g_resultPlayer[nCntRank].pos = D3DXVECTOR3( POLYGON_POS_X, POLYGON_POS_Y + nCntRank*110, 0.0f);  //��5
		g_resultPlayer[nCntRank + MAX_RANK/2].pos = D3DXVECTOR3( POLYGON_POS_X + 550, POLYGON_POS_Y + nCntRank*110, 0.0f); //�E5

		//switch(nCntRank)
		//{
		//	case 0:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,0,0,255); //red
		//		break;
		//	case 1:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,128,0,255); //orange
		//		break;
		//	case 2:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(255,255,0,255); //yellow
		//		break;
		//	case 3:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(0,255,0,255); //green
		//		break;
		//	case 4:
		//		g_resultPlayer[nCntRank].col = g_resultPlayer[nCntRank + MAX_RANK/2].col = D3DCOLOR_RGBA(0,0,255,255); //blue
		//		break;
		//}	
	}
	g_resultPlayer[MAX_RANK-1].pos = D3DXVECTOR3( -1000, -1000 , 0.0f);

	//������
	g_angle2 = 0;

	//���_���̍쐬
	if(FAILED(MakeVertexResultPlayer(pDevice)))
	{
		return E_FAIL;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, POLYGON_TEXTURENAME, &g_pTextureResultPlayer);

	//SCORE FILE ����
	ScoreFileReadWrite();

	//rank�̕\��
	for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		if(nCnt == g_playerRank)
		{
			g_resultPlayer[nCnt].bUse = true;
		}
		ChangeResultRankTex(nCnt, rank[nCnt], D3DCOLOR_RGBA(255,255,255,255) );
		ChangeResultRankNth(nCnt, nCnt+1, D3DCOLOR_RGBA(255,255,255,255) );
	}

	return S_OK;
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawResultRank(void)
{
	//�O���֐��̌Ăяo��
	DrawResultRankTex();
	DrawResultRankNth();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBufferResultPlayer, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultPlayer);

	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if(g_resultPlayer[nCntRank].bUse)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntRank*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);
		}
	}


}

//=============================================================================
// �I������
//=============================================================================
void UninitResultRank(void)
{
	//�O���֐��̌Ăяo��
	UninitResultRankTex();
	UninitResultRankNth();

	if(g_pTextureResultPlayer != NULL)
	{
		g_pTextureResultPlayer->Release();
		g_pTextureResultPlayer = NULL;
	}
	if(g_pVtxBufferResultPlayer != NULL)
	{
		g_pVtxBufferResultPlayer->Release();
		g_pVtxBufferResultPlayer = NULL;
	}
}


//=============================================================================
// �X�V����
//=============================================================================
void UpdateResultRank()
{
	//�O���֐��̌Ăяo��
	UpdateResultRankTex();
	UpdateResultRankNth();


	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		if(g_resultPlayer[nCntRank].bUse)
		{
			//change size
			g_resultPlayer[nCntRank].size.x += sin(g_angle2)*0.5f;
			g_resultPlayer[nCntRank].size.y += sin(g_angle2)*0.5f;
			g_angle2 += 0.1f;
			if(g_angle2 >= 6.28) g_angle2 = 0;

			SetVertexResultRank(nCntRank);	
		}	
	}


}

//=============================================================================
// �|���S���̒��_���̍쐬�֐�
//=============================================================================
HRESULT MakeVertexResultPlayer(LPDIRECT3DDEVICE9 pDevice)
{
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D)*NUM_VERTEX*MAX_RANK,	//���_�f�[�^�̃o�b�t�@�T�C�Y 
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_2D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED, 
		&g_pVtxBufferResultPlayer,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferResultPlayer->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++, pVtx += NUM_VERTEX)
	{
		//���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_resultPlayer[nCntRank].pos.x - (POLYGON_SIZE_X/2), g_resultPlayer[nCntRank].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_resultPlayer[nCntRank].pos.x + (POLYGON_SIZE_X/2), g_resultPlayer[nCntRank].pos.y - (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_resultPlayer[nCntRank].pos.x - (POLYGON_SIZE_X/2), g_resultPlayer[nCntRank].pos.y + (POLYGON_SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_resultPlayer[nCntRank].pos.x + (POLYGON_SIZE_X/2), g_resultPlayer[nCntRank].pos.y + (POLYGON_SIZE_Y/2), 0.0f);


		//rhw
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;


		//color
		pVtx[0].col = g_resultPlayer[nCntRank].col;
		pVtx[1].col = g_resultPlayer[nCntRank].col;
		pVtx[2].col = g_resultPlayer[nCntRank].col;
		pVtx[3].col = g_resultPlayer[nCntRank].col;

		//�e�N�X�`�����W�w��
		pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
		pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
		pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
		pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);	
	}



	g_pVtxBufferResultPlayer->Unlock();

	return S_OK;
}


//=============================================================================
// ���_�ݒ�
//=============================================================================
void SetVertexResultRank(int nRankIdx)
{

	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_2D *pVtx;

	//���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pVtxBufferResultPlayer->Lock( 0, 0, (void**)&pVtx, 0);

	pVtx += nRankIdx * NUM_VERTEX; 

	//���_���W�̐ݒ�
	pVtx[0].vtx = D3DXVECTOR3(g_resultPlayer[nRankIdx].pos.x - (g_resultPlayer[nRankIdx].size.x/2), g_resultPlayer[nRankIdx].pos.y - (g_resultPlayer[nRankIdx].size.y/2), 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(g_resultPlayer[nRankIdx].pos.x + (g_resultPlayer[nRankIdx].size.x/2), g_resultPlayer[nRankIdx].pos.y - (g_resultPlayer[nRankIdx].size.y/2), 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(g_resultPlayer[nRankIdx].pos.x - (g_resultPlayer[nRankIdx].size.x/2), g_resultPlayer[nRankIdx].pos.y + (g_resultPlayer[nRankIdx].size.y/2), 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(g_resultPlayer[nRankIdx].pos.x + (g_resultPlayer[nRankIdx].size.x/2), g_resultPlayer[nRankIdx].pos.y + (g_resultPlayer[nRankIdx].size.y/2), 0.0f);

	// ���_�f�[�^���A�����b�N����
	g_pVtxBufferResultPlayer->Unlock();
}

//=============================================================================
// �o�u���\�[�g
//=============================================================================
void BubbleSort(int data[], int size)
{
	int temp;
	for(int i = 0; i <size; i++)
	{
		for(int j = size - 1; j > i ;j--)
		{
			if(data[j-1] < data[j])
			{
				temp = data[j-1];
				data[j-1] = data[j];
				data[j] = temp;
			}
		}
	}
}

//=============================================================================
// �t�@�C���ǂݍ��݂Ə����o��
//=============================================================================
void ScoreFileReadWrite(void)
{
	FILE *pRank;

	//�t�@�C���̓ǂݍ���
	pRank = fopen( "data/FILE/ranking.bin","rb" );

	if(pRank == NULL) //�G���[
	{
		pRank = fopen( "data/FILE/ranking.bin","wb" );
		for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			rank[nCnt] = 0;
		}	
	}
	else //����
	{
		fread(rank,sizeof(int),MAX_RANK,pRank);
		//����
		for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			rank[nCnt] ^= key;
		}
	}
	fclose(pRank);


	//����̓_��
	if( IsGameClear())
	{
		rank[MAX_RANK-1] = GetTime();
	}
	else
	{
		rank[MAX_RANK-1] = 0;
	}
	
	
	//�\�[�g����
	BubbleSort(rank,MAX_RANK);
	
	if( !IsGameClear())
	{
		g_playerRank = MAX_RANK-1;
	}
	else
	{
		//�v�����[�̓_�����w��
		for(int nCnt = MAX_RANK-1; nCnt >= 0; nCnt--)
		{
			if(GetTime() == rank[nCnt])
			{
				g_playerRank = nCnt;
				break;
			}
		}	
	}


	//�Í���
	for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		rank[nCnt] ^= key;
	}
	//�t�@�C���̏�������
	pRank = fopen( "data/FILE/ranking.bin","wb" );
	fwrite(rank,sizeof(int),MAX_RANK,pRank);
	fclose(pRank);
	//�Í���
	for(int nCnt = 0; nCnt < MAX_RANK; nCnt++)
	{
		rank[nCnt] ^= key;
	}
}
