/*******************************************************************************
*
* �^�C�g��:		�����̐��p�[�e�B�N������
* �v���O������:	fountain.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "fountain.h"
#include "camera.h"
#include "shadow.h"
#include "sound.h"
#include "hitCheck.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PARTICLE	(128)
#define NUM_VERTEX	(4)
#define NUM_POLYGON (2)

#define TEXTURE_FIELD	"data/TEXTURE/shadow000.jpg"					// �ǂݍ��ރe�N�X�`���t�@�C����


#define VALUE_MOVE		(5.0f)											// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 								// ��]��

#define SIZE_X			(20.0f)											// �n�ʂ̃T�C�Y(X����)
#define SIZE_Y			(20.0f)											// �n�ʂ̃T�C�Y(Z����)
																		
#define CREATE_RATE	(1)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT MakeVertexFountain(LPDIRECT3DDEVICE9 pDevice);
void SetColorFountain(int nIdxFountain, D3DXCOLOR col);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
FOUNTAIN g_aFountain[MAX_PARTICLE];
LPDIRECT3DTEXTURE9		g_pD3DTextureFountain;								// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffFountain;								// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
bool g_bDraw;
int g_bDrawTimer;
D3DXVECTOR3 g_posPtc;
int g_nPtcCreateRate;
int g_nPtcCreateCounter;
float g_fDisToPlayer = 0;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitFountain(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_bDraw = false;
	g_bDrawTimer = 0;
	g_posPtc = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++)
	{
		g_aFountain[nCntBoard].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aFountain[nCntBoard].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_aFountain[nCntBoard].scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
		g_aFountain[nCntBoard].mov = D3DXVECTOR3( 1.0f, 5.0f, 1.0f);

		g_aFountain[nCntBoard].volume = 1.0f;
		g_aFountain[nCntBoard].alpha = 0;
		g_aFountain[nCntBoard].timer = 0;
		g_aFountain[nCntBoard].bUse = false;
	}

	g_pD3DVtxBuffFountain = NULL;

	
	// ���_���̍쐬
	MakeVertexFountain(pDevice);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile( pDevice, TEXTURE_FIELD, &g_pD3DTextureFountain);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitFountain(void)
{
	if(g_pD3DTextureFountain != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureFountain->Release();
		g_pD3DTextureFountain = NULL;
	}


	if(g_pD3DVtxBuffFountain != NULL)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffFountain->Release();
		g_pD3DVtxBuffFountain = NULL;
	}	


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateFountain(float fTimeSpeed)
{
	if(!g_bDraw) return;

	//���Ԃɂ��A���Y�����ς��
	g_nPtcCreateRate = CREATE_RATE / fTimeSpeed;
	g_nPtcCreateCounter++;

	//sound volume
	PLAYER *pPlayer = GetPlayer();
	float distance = GetDistanceXZ(pPlayer->pos, g_posPtc);
	if( distance > 1000.0f || fTimeSpeed <= 0.01f)
	{
		UpdateSoundVolume( SOUND_LABEL_SE_WATER, 0.0f);
	}
	else
	{
		float volume = (1000 - (int)distance % 1000)  * 0.001f;
		UpdateSoundVolume( SOUND_LABEL_SE_WATER, volume);		
	}

	for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++)
	{
		if(!g_aFountain[nCntBoard].bUse && g_nPtcCreateCounter >= g_nPtcCreateRate)
		{
			g_aFountain[nCntBoard].bUse = true;
			g_aFountain[nCntBoard].timer = 0;
			g_aFountain[nCntBoard].pos = g_posPtc;
			g_aFountain[nCntBoard].mov.x = (rand() % 100 - 50) * 0.03f;
			g_aFountain[nCntBoard].mov.z = (rand() % 100 - 50) * 0.03f;
			g_aFountain[nCntBoard].mov.y = 7.0f;
			g_aFountain[nCntBoard].nIdxShadow = CreateShadow( g_aFountain[nCntBoard].pos, 10.0f, 10.0f);
			g_nPtcCreateCounter = 0;
			break;
		}
		else
		{
			g_aFountain[nCntBoard].timer += 1.0f * fTimeSpeed;
			g_aFountain[nCntBoard].pos.x += g_aFountain[nCntBoard].mov.x * fTimeSpeed;
			g_aFountain[nCntBoard].pos.y = g_posPtc.y + g_aFountain[nCntBoard].mov.y * g_aFountain[nCntBoard].timer - 0.5f * 0.2f * g_aFountain[nCntBoard].timer * g_aFountain[nCntBoard].timer;
			g_aFountain[nCntBoard].pos.z += g_aFountain[nCntBoard].mov.z * fTimeSpeed;

			g_aFountain[nCntBoard].alpha += 2.0f * fTimeSpeed;
			SetColorFountain( nCntBoard, D3DCOLOR_RGBA(255, 255, 255, (int)g_aFountain[nCntBoard].alpha) );

			if(g_aFountain[nCntBoard].pos.y < -SIZE_Y/2)
			{
				g_aFountain[nCntBoard].mov.y = g_aFountain[nCntBoard].mov.y * 2.0f / 3.0f;
				g_aFountain[nCntBoard].timer = 0;
			}

			SetPositionShadow( g_aFountain[nCntBoard].nIdxShadow, g_aFountain[nCntBoard].pos);
			SetColorShadow( g_aFountain[nCntBoard].nIdxShadow, D3DCOLOR_RGBA( 255, 255, 255, (int)((255 - g_aFountain[nCntBoard].alpha) * 0.3)) );

			if(g_aFountain[nCntBoard].alpha >= 255)
			{
				g_aFountain[nCntBoard].bUse = false;
				g_aFountain[nCntBoard].pos = g_posPtc;
				g_aFountain[nCntBoard].mov = D3DXVECTOR3( 1.0f, 5.0f, 1.0f);
				g_aFountain[nCntBoard].alpha = 0;
				ReleaseShadow( g_aFountain[nCntBoard].nIdxShadow);
			}

		}

	}

	//setparticle timer
	if(g_bDrawTimer > 0)
	{
		g_bDrawTimer--;
		for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++)
		{
			ReleaseShadow(g_aFountain[nCntBoard].nIdxShadow);
		}
		
	}
	else if(g_bDrawTimer == -1)
	{
		//�������[�v
	}
	else
	{
		g_bDraw = false;
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawFountain(void)
{
	if(!g_bDraw) return;

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
		
	for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++)
	{
		if(g_aFountain[nCntBoard].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_aFountain[nCntBoard].mtxWorld);

			//���[���h�}�g���b�N�X�ɃJ�����̍s���]�u�s��Ƃ��Đݒ肷��
			g_aFountain[nCntBoard].mtxWorld._11 = pCamera->mtxView._11;
			g_aFountain[nCntBoard].mtxWorld._22 = pCamera->mtxView._22;
			g_aFountain[nCntBoard].mtxWorld._33 = pCamera->mtxView._33;

			g_aFountain[nCntBoard].mtxWorld._12 = pCamera->mtxView._21;
			g_aFountain[nCntBoard].mtxWorld._13 = pCamera->mtxView._31;
			g_aFountain[nCntBoard].mtxWorld._23 = pCamera->mtxView._32;

			g_aFountain[nCntBoard].mtxWorld._21 = pCamera->mtxView._12;
			g_aFountain[nCntBoard].mtxWorld._31 = pCamera->mtxView._13;
			g_aFountain[nCntBoard].mtxWorld._32 = pCamera->mtxView._23;

			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_aFountain[nCntBoard].scl.x, g_aFountain[nCntBoard].scl.y, g_aFountain[nCntBoard].scl.z);
			D3DXMatrixMultiply( &g_aFountain[nCntBoard].mtxWorld, &g_aFountain[nCntBoard].mtxWorld, &mtxScl);
	
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_aFountain[nCntBoard].rot.y, g_aFountain[nCntBoard].rot.x, g_aFountain[nCntBoard].rot.z);
			D3DXMatrixMultiply( &g_aFountain[nCntBoard].mtxWorld, &g_aFountain[nCntBoard].mtxWorld, &mtxRot);
	
			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTranslate, g_aFountain[nCntBoard].pos.x, g_aFountain[nCntBoard].pos.y, g_aFountain[nCntBoard].pos.z);
			D3DXMatrixMultiply( &g_aFountain[nCntBoard].mtxWorld, &g_aFountain[nCntBoard].mtxWorld, &mtxTranslate);
	
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform( D3DTS_WORLD, &g_aFountain[nCntBoard].mtxWorld);

			// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetStreamSource(0, g_pD3DVtxBuffFountain, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pD3DTextureFountain);

			//���e�X�g
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				nCntBoard*NUM_VERTEX,	//���[�h����ŏ��̒��_�C���f�b�N�X
				NUM_POLYGON				//�|���S���̐�
			);			
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
HRESULT MakeVertexFountain(LPDIRECT3DDEVICE9 pDevice)
{
	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
	if(FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D)*NUM_VERTEX*MAX_PARTICLE,	//���_�f�[�^�̃o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,					//���_�t�H�[�}�b�g
		D3DPOOL_MANAGED,
		&g_pD3DVtxBuffFountain,			//���_�o�b�t�@�C���^�[�t�F�[�X�̃|�C���^
		NULL)))
	{
		return E_FAIL;
	}


	//���_�o�b�t�@�̒��g�𖄂߂�
	VERTEX_3D *pVtx;

	// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
	g_pD3DVtxBuffFountain->Lock( 0, 0, (void**)&pVtx, 0);

	for(int nCntBoard = 0; nCntBoard < MAX_PARTICLE; nCntBoard++, pVtx+=4)
	{
		// ���_���W�̐ݒ�
		pVtx[0].vtx = D3DXVECTOR3(g_aFountain[nCntBoard].pos.x - (SIZE_X/2), g_aFountain[nCntBoard].pos.y + (SIZE_Y/2), 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(g_aFountain[nCntBoard].pos.x + (SIZE_X/2), g_aFountain[nCntBoard].pos.y + (SIZE_Y/2), 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(g_aFountain[nCntBoard].pos.x - (SIZE_X/2), g_aFountain[nCntBoard].pos.y - (SIZE_Y/2), 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(g_aFountain[nCntBoard].pos.x + (SIZE_X/2), g_aFountain[nCntBoard].pos.y - (SIZE_Y/2), 0.0f);

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
	g_pD3DVtxBuffFountain->Unlock();

	return S_OK;
}

//=============================================================================
// ���_�J���[�̐ݒ�
//=============================================================================
void SetColorFountain(int nIdxFountain, D3DXCOLOR col)
{
	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffFountain->Lock( 0, 0, (void**)&pVtx, 0);

		// pVtx���C���f�b�N�X���Y����
		pVtx += nIdxFountain * 4;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffFountain->Unlock();
	}
}

/*******************************************************************************
* �����̐ݒu
*******************************************************************************/
void SetFountain(int drawTime, D3DXVECTOR3 pos)
{
	g_bDraw = true;
	g_bDrawTimer = drawTime;
	g_posPtc = pos;
}





