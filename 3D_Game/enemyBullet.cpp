/*******************************************************************************
*
* �^�C�g��:		�G�̒e�̏���
* �v���O������:	enemyBullet.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "enemyBullet.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "meshwall.h"
#include "hitCheck.h"
#include "sound.h"
#include "player.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_BULLET		"data/MODEL/player_bullet.x"					// �ǂݍ��ރe�N�X�`���t�@�C����

#define VALUE_MOVE		(0.0f)									// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.1f) 						// ��]��

#define MODEL_POS_X	(0)
#define MODEL_POS_Y	(100)
#define MODEL_POS_Z	(0)

#define MODEL_SPHERE (20)
#define BULLET_SPEED	(7.5f)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY_BULLET g_E_Bullet[MAX_ENEMY_BULLET];


//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for(int nCntBullet = 0; nCntBullet < MAX_ENEMY_BULLET; nCntBullet++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_E_Bullet[nCntBullet].pos = D3DXVECTOR3( MODEL_POS_X, MODEL_POS_Y, MODEL_POS_Z);
		g_E_Bullet[nCntBullet].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_E_Bullet[nCntBullet].scl = D3DXVECTOR3( 0.5f, 0.5f, 0.5f);
		g_E_Bullet[nCntBullet].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_E_Bullet[nCntBullet].rotAngle = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_E_Bullet[nCntBullet].rotTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_E_Bullet[nCntBullet].bUse = false;
		g_E_Bullet[nCntBullet].nJustShoot = 0;

		// ���f���Ɋւ���ϐ��̏�����							
		g_E_Bullet[nCntBullet].pD3DTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
		g_E_Bullet[nCntBullet].pD3DXMesh = NULL;		// ���b�V�����ւ̃|�C���^
		g_E_Bullet[nCntBullet].pD3DXBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
		g_E_Bullet[nCntBullet].nNumMat = 0;				// �}�e���A�����̐�

		// X�t�@�C���̓ǂݍ���
		if(FAILED(D3DXLoadMeshFromX(
			MODEL_BULLET,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,			// ���b�V���̍쐬�I�v�V�������w��
			pDevice,					// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,						// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&g_E_Bullet[nCntBullet].pD3DXBuffMat,		// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,						// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&g_E_Bullet[nCntBullet].nNumMat,			// D3DXMATERIAL�\���̂̐�
			&g_E_Bullet[nCntBullet].pD3DXMesh			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
			)))
		{
			return E_FAIL;
		}		

	}
													
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemyBullet(void)
{
	for(int nCntBullet = 0; nCntBullet < MAX_ENEMY_BULLET; nCntBullet++)
	{
		// ���b�V���̊J��
		if(g_E_Bullet[nCntBullet].pD3DXMesh != NULL)
		{
			g_E_Bullet[nCntBullet].pD3DXMesh->Release();
			g_E_Bullet[nCntBullet].pD3DXMesh = NULL;
		}
		// �}�e���A���̊J��
		if(g_E_Bullet[nCntBullet].pD3DXBuffMat != NULL)
		{
			g_E_Bullet[nCntBullet].pD3DXBuffMat->Release();
			g_E_Bullet[nCntBullet].pD3DXBuffMat = NULL;
		}
		g_E_Bullet[nCntBullet].bUse = false;
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemyBullet( float fTimeSpeed)
{
	CAMERA *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();

	for(int nCntBullet = 0; nCntBullet < MAX_ENEMY_BULLET; nCntBullet++)
	{
		if(g_E_Bullet[nCntBullet].bUse)
		{
			//�X�e�[�W�̒��ɂ��Ȃ�������
			if(!HitCheckBall( g_E_Bullet[nCntBullet].pos, 2000.0f, D3DXVECTOR3( 0.0f, 0.0f, 0.0f), 0.0f))
			{
				g_E_Bullet[nCntBullet].bUse = false;
			}

			//�ǂɓ���������
			if( HitCheckMeshWall( g_E_Bullet[nCntBullet].pos, g_E_Bullet[nCntBullet].pos + g_E_Bullet[nCntBullet].front, NULL, NULL))
			{
				g_E_Bullet[nCntBullet].bUse = false;			
			}
			else if( g_E_Bullet[nCntBullet].nJustShoot > 0)
			{
				g_E_Bullet[nCntBullet].nJustShoot--;
				g_E_Bullet[nCntBullet].pos += g_E_Bullet[nCntBullet].front;
			}
			else 
			{
				g_E_Bullet[nCntBullet].pos += g_E_Bullet[nCntBullet].front * fTimeSpeed;
			}

			//�v���C���[�ɓ���������
			if( pPlayer->bAlive)
			{
				if( HitCheckBall( g_E_Bullet[nCntBullet].pos, 1.0f, pCamera->posV, 10.0f) ||
					HitCheckBall( g_E_Bullet[nCntBullet].pos, 1.0f, pCamera->posV - D3DXVECTOR3( 0.0f, 20.0f, 0.0f), 10.0f) ||
					HitCheckBall( g_E_Bullet[nCntBullet].pos, 1.0f, pCamera->posV - D3DXVECTOR3( 0.0f, 40.0f, 0.0f), 10.0f))
				{
					g_E_Bullet[nCntBullet].bUse = false;
					PlaySound(SOUND_LABEL_SE_HURT);

					if( !IsDebugMode()) SetPlayerDeath();
				}			
			}

		

		}
	}



}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemyBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for(int nCntBullet = 0; nCntBullet < MAX_ENEMY_BULLET; nCntBullet++)
	{
		if(g_E_Bullet[nCntBullet].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_E_Bullet[nCntBullet].mtxWorld);
	
			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_E_Bullet[nCntBullet].scl.x, g_E_Bullet[nCntBullet].scl.y, g_E_Bullet[nCntBullet].scl.z);
			D3DXMatrixMultiply( &g_E_Bullet[nCntBullet].mtxWorld, &g_E_Bullet[nCntBullet].mtxWorld, &mtxScl);
	
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_E_Bullet[nCntBullet].rot.y, g_E_Bullet[nCntBullet].rot.x, g_E_Bullet[nCntBullet].rot.z);
			D3DXMatrixMultiply( &g_E_Bullet[nCntBullet].mtxWorld, &g_E_Bullet[nCntBullet].mtxWorld, &mtxRot);
	
			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTranslate, g_E_Bullet[nCntBullet].pos.x, g_E_Bullet[nCntBullet].pos.y, g_E_Bullet[nCntBullet].pos.z);
			D3DXMatrixMultiply( &g_E_Bullet[nCntBullet].mtxWorld, &g_E_Bullet[nCntBullet].mtxWorld, &mtxTranslate);
	
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform( D3DTS_WORLD, &g_E_Bullet[nCntBullet].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);
	
			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL *)g_E_Bullet[nCntBullet].pD3DXBuffMat->GetBufferPointer();
			for(int nCntMat = 0; nCntMat < (int)g_E_Bullet[nCntBullet].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture( 0, g_E_Bullet[nCntBullet].pD3DTexture);
				// �`��
				g_E_Bullet[nCntBullet].pD3DXMesh->DrawSubset( nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial( &matDef);		
		}
	}


	

}

void SetEnemyBullet( D3DXVECTOR3 pos, D3DXVECTOR3 target)
{
	D3DXVECTOR3 front = target - pos;
	D3DXVec3Normalize( &front, &front);

	for(int nCntBullet = 0; nCntBullet < MAX_ENEMY_BULLET; nCntBullet++)
	{
		if(!g_E_Bullet[nCntBullet].bUse) 
		{
			//������ύX
			g_E_Bullet[nCntBullet].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
			g_E_Bullet[nCntBullet].rot = GetRotationAngle( pos, target, g_E_Bullet[nCntBullet].rot);

			//�ʒu��ݒ�
			g_E_Bullet[nCntBullet].pos = pos;
			g_E_Bullet[nCntBullet].front = front * BULLET_SPEED;
			g_E_Bullet[nCntBullet].bUse = true;

			//
			g_E_Bullet[nCntBullet].nJustShoot = 15;
			break;
		}
	}	
}





