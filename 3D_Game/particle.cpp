/*******************************************************************************
*
* �^�C�g��:		�G�̌�(�p�[�e�B�N��)����
* �v���O������:	particle.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "particle.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PARTICLE	(5)
#define NUM_VERTEX	(4)
#define NUM_POLYGON (2)

#define TEXTURE_FIELD	"data/TEXTURE/shadow000.jpg"					// �ǂݍ��ރe�N�X�`���t�@�C����


#define SIZE_X			(10.0f)											// �n�ʂ̃T�C�Y(X����)
#define SIZE_Y			(10.0f)											// �n�ʂ̃T�C�Y(Z����)

#define CREATE_RATE	(1)
																		

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);
void SetColorParticle(int nIdxPtc, int nIdxDot, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pD3DTextureParticle;								// �e�N�X�`���ւ̃|�C���^

PARTICLE g_aParticle[10];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		g_aParticle[nCntPtc].bDraw = false;
		g_aParticle[nCntPtc].nNeedCreateNum = 0;
		g_aParticle[nCntPtc].nPtcCreateRate = CREATE_RATE;
		g_aParticle[nCntPtc].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);

		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
		{
			g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
			g_aParticle[nCntPtc].aPtcDot[nCntDot].rotDot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
			g_aParticle[nCntPtc].aPtcDot[nCntDot].sclDot = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
			g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot = D3DXVECTOR3( 1.0f, 5.0f, 1.0f);

			g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha = 0;
			g_aParticle[nCntPtc].aPtcDot[nCntDot].timer = 0;
			g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = false;
		}	

		g_aParticle[nCntPtc].pD3DVtxBuffParticle = NULL;
	}

	
	// ���_���̍쐬
	MakeVertexParticle(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureParticle);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitParticle(void)
{
	if(g_pD3DTextureParticle != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureParticle->Release();
		g_pD3DTextureParticle = NULL;
	}

	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		if(g_aParticle[nCntPtc].pD3DVtxBuffParticle != NULL)
		{
			g_aParticle[nCntPtc].pD3DVtxBuffParticle->Release();
			g_aParticle[nCntPtc].pD3DVtxBuffParticle = NULL;
		}
		g_aParticle[nCntPtc].bDraw = false;

		for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
		{
			g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = false;
		}
		
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateParticle( float fTimeSpeed)
{
	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		if(g_aParticle[nCntPtc].bDraw)
		{
			//���Ԃɂ��A���Y�����ς��
			g_aParticle[nCntPtc].nPtcCreateRate = CREATE_RATE / fTimeSpeed;
			g_aParticle[nCntPtc].nPtcCreateCounter++;

			for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
			{
				if(!g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse && g_aParticle[nCntPtc].nPtcCreateCounter >= g_aParticle[nCntPtc].nPtcCreateRate && g_aParticle[nCntPtc].nNeedCreateNum > 0)
				{
					g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = true;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].timer = 0.0f;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.x = (rand() % 100 - 50) * 0.03f;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.z = (rand() % 100 - 50) * 0.03f;

					g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot = g_aParticle[nCntPtc].pos;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow = CreateShadow( g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot, 10.0f, 10.0f);
					g_aParticle[nCntPtc].nNeedCreateNum--;
					g_aParticle[nCntPtc].nPtcCreateCounter = 0;
					break;
				}
				else
				{
					g_aParticle[nCntPtc].aPtcDot[nCntDot].timer += 1.0f * fTimeSpeed;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x += g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.x * fTimeSpeed;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y = (g_aParticle[nCntPtc].pos.y + g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.y * g_aParticle[nCntPtc].aPtcDot[nCntDot].timer - 0.5f * 0.2f * g_aParticle[nCntPtc].aPtcDot[nCntDot].timer * g_aParticle[nCntPtc].aPtcDot[nCntDot].timer);
					g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.z += g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.z * fTimeSpeed;

					g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha += 2.0f * fTimeSpeed;
					SetColorParticle( nCntPtc, nCntDot, D3DCOLOR_RGBA(255, 0, 0, (int)g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha) );

					if(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y < -SIZE_Y/2)
					{
						g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.y = g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot.y * 2.0f / 3.0f;
						g_aParticle[nCntPtc].aPtcDot[nCntDot].timer = 0.0f;
					}

					SetPositionShadow( g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow, g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot);
					SetColorShadow( g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow, D3DCOLOR_RGBA( 255, 255, 255, (int)((255 - g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha) * 0.3)) );

					if(g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha >= 255 && g_aParticle[nCntPtc].nNeedDeteNum > 0)
					{
						g_aParticle[nCntPtc].nNeedDeteNum--;
						g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = false;
						g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot =  g_aParticle[nCntPtc].pos;
						g_aParticle[nCntPtc].aPtcDot[nCntDot].movDot = D3DXVECTOR3( 1.0f, 5.0f, 1.0f);
						g_aParticle[nCntPtc].aPtcDot[nCntDot].alpha = 0;
						ReleaseShadow(  g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow);
					}

				}

			}

			if( g_aParticle[nCntPtc].nNeedDeteNum <= 0 &&  g_aParticle[nCntPtc].nNeedCreateNum <= 0)
			{
				for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
				{
					ReleaseShadow(g_aParticle[nCntPtc].aPtcDot[nCntDot].nIdxShadow);
					g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse = false;
				}

				g_aParticle[nCntPtc].bDraw = false;
			}
		}
	}







}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	CAMERA *pCamera = GetCamera();

	// ���Z����
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);	// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);			// Z��r�Ȃ�

	// ���C�e�B���O���[�h��OFF
	pDevice->SetRenderState( D3DRS_LIGHTING, FALSE);

	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		if(g_aParticle[nCntPtc].bDraw)
		{		
			for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++)
			{
				if(g_aParticle[nCntPtc].aPtcDot[nCntDot].bUse)
				{
					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity( &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld);

					//���[���h�}�g���b�N�X�ɃJ�����̍s���]�u�s��Ƃ��Đݒ肷��
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._11 = pCamera->mtxView._11;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._22 = pCamera->mtxView._22;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._33 = pCamera->mtxView._33;
				
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._12 = pCamera->mtxView._21;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._13 = pCamera->mtxView._31;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._23 = pCamera->mtxView._32;
					
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._21 = pCamera->mtxView._12;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._31 = pCamera->mtxView._13;
					g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld._32 = pCamera->mtxView._23;

					// �X�P�[���𔽉f
					D3DXMatrixScaling( &mtxScl, g_aParticle[nCntPtc].aPtcDot[nCntDot].sclDot.x, g_aParticle[nCntPtc].aPtcDot[nCntDot].sclDot.y, g_aParticle[nCntPtc].aPtcDot[nCntDot].sclDot.z);
					D3DXMatrixMultiply( &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &mtxScl);
	
					// ��]�𔽉f
					D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aParticle[nCntPtc].aPtcDot[nCntDot].rotDot.y, g_aParticle[nCntPtc].aPtcDot[nCntDot].rotDot.x, g_aParticle[nCntPtc].aPtcDot[nCntDot].rotDot.z);
					D3DXMatrixMultiply( &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &mtxRot);
	
					// �ړ��𔽉f
					D3DXMatrixTranslation( &mtxTranslate, g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x, g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y, g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.z);
					D3DXMatrixMultiply( &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld, &mtxTranslate);
	
					// ���[���h�}�g���b�N�X�̐ݒ�
					pDevice->SetTransform( D3DTS_WORLD, &g_aParticle[nCntPtc].aPtcDot[nCntDot].mtxWorld);

					// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
					pDevice->SetStreamSource(0,  g_aParticle[nCntPtc].pD3DVtxBuffParticle, 0, sizeof(VERTEX_3D));

					// ���_�t�H�[�}�b�g�̐ݒ�
					pDevice->SetFVF(FVF_VERTEX_3D);

					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pD3DTextureParticle);

					//���e�X�g
					pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
					pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
					pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

					//�|���S���̕`��
					pDevice->DrawPrimitive(
						D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
						nCntDot*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
						NUM_POLYGON				//�|���S���̐�
					);			
				}
			}


			
		}
	}

	// ���C�e�B���O���[�h��ON
	pDevice->SetRenderState( D3DRS_LIGHTING, TRUE);

	// ���e�X�g�𖳌���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// �ʏ�u�����h 
	pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);	// ���� = �]����(SRC) + �]����(DEST)
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);	
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);			// Z��r����
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{


	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
		if(FAILED(pDevice->CreateVertexBuffer(
			sizeof(VERTEX_3D)*NUM_VERTEX*MAX_DOT,	//���_�f�[�^�̃o�b�t�@�T�C�Y
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,					//���_�t�H�[�}�b�g
			D3DPOOL_MANAGED,
			&g_aParticle[nCntPtc].pD3DVtxBuffParticle,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
			NULL)))
		{
			return E_FAIL;
		}


		//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_aParticle[nCntPtc].pD3DVtxBuffParticle->Lock( 0, 0, (void**)&pVtx, 0);

		for(int nCntDot = 0; nCntDot < MAX_DOT; nCntDot++, pVtx+=4)
		{
			// ���_���W�̐ݒ�
			pVtx[0].vtx = D3DXVECTOR3(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x - (SIZE_X/2), g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y + (SIZE_Y/2), 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x + (SIZE_X/2), g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y + (SIZE_Y/2), 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x - (SIZE_X/2), g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y - (SIZE_Y/2), 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.x + (SIZE_X/2), g_aParticle[nCntPtc].aPtcDot[nCntDot].posDot.y - (SIZE_Y/2), 0.0f);

			// �@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

			// ���ˌ��̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[1].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[2].col = D3DCOLOR_RGBA(255,255,255,255);
			pVtx[3].col = D3DCOLOR_RGBA(255,255,255,255);

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0F, 0.0F);
			pVtx[1].tex = D3DXVECTOR2(1.0F, 0.0F);
			pVtx[2].tex = D3DXVECTOR2(0.0F, 1.0F);
			pVtx[3].tex = D3DXVECTOR2(1.0F, 1.0F);
		}	
		// ���_�f�[�^���A�����b�N����
		g_aParticle[nCntPtc].pD3DVtxBuffParticle->Unlock();
	}


	return S_OK;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorParticle(int nIdxPtc, int nIdxDot, D3DXCOLOR col)
{
	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_aParticle[nIdxPtc].pD3DVtxBuffParticle->Lock( 0, 0, (void**)&pVtx, 0);

	// pVtx���C���f�b�N�X���Y����
	pVtx += nIdxDot * 4;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	// ���_�f�[�^���A�����b�N����
	g_aParticle[nIdxPtc].pD3DVtxBuffParticle->Unlock();	


}

//=============================================================================
// �p�[�e�B�N���̐ݒu
//=============================================================================
void SetParticle(int needNum, D3DXVECTOR3 pos)
{
	for(int nCntPtc = 0; nCntPtc < MAX_PARTICLE; nCntPtc++)
	{
		if( !g_aParticle[nCntPtc].bDraw)
		{
			g_aParticle[nCntPtc].bDraw = true;
			g_aParticle[nCntPtc].nNeedCreateNum = needNum;
			g_aParticle[nCntPtc].nNeedDeteNum = needNum;
			g_aParticle[nCntPtc].pos = pos;
			g_aParticle[nCntPtc].nPtcCreateCounter = 0;
			break;
		}
	}
}





