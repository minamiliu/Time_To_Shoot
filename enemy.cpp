/*******************************************************************************
*
* �^�C�g��:		�G�̏���
* �v���O������:	enemy.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "enemy.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "meshwall.h"
#include "hitCheck.h"
#include "sound.h"
#include "particle.h"
#include "player.h"
#include "enemyBullet.h"
#include "game.h"
#include "fade.h"


#include "message.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_ENEMY_WHITE	"data/MODEL/enemy.x"					// �ǂݍ��ރe�N�X�`���t�@�C����
#define MODEL_ENEMY_BLUE	"data/MODEL/enemy_blue.x"				// �ǂݍ��ރe�N�X�`���t�@�C����
#define MODEL_ENEMY_RED		"data/MODEL/enemy_red.x"				// �ǂݍ��ރe�N�X�`���t�@�C����

#define VALUE_MOVE		(1.0f)									// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 						// ��]��
#define DIVIDE_ROTATE	(10) 									// ��]�ʂ������ɕ�����

#define MODEL_POS_X	(0)
#define MODEL_POS_Y	(70)
#define MODEL_POS_Z	(0)

#define MODEL_SPHERE (20)
#define COOLDOWN	(90.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
ENEMY g_enemy[MAX_ENEMY];
ENEMY_MODEL g_modelData[ENEMY_TYPE_MAX];

//=============================================================================
// ����������
//=============================================================================
HRESULT InitEnemy(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �ʒu�E��]�E�X�P�[���̏����ݒ�
		g_enemy[nCntEnemy].pos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].scl = D3DXVECTOR3( 2.0f, 2.0f, 2.0f);
		g_enemy[nCntEnemy].move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
		g_enemy[nCntEnemy].rotAngle = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rotTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].nCoolDown = -1;
		g_enemy[nCntEnemy].bUse = false;
		g_enemy[nCntEnemy].bShot = false;
		g_enemy[nCntEnemy].bSeePlayer = false;
		g_enemy[nCntEnemy].nSeePFrame = 0;
		g_enemy[nCntEnemy].bPatrol = true;
		g_enemy[nCntEnemy].posPatrol = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].rotPatrol = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
		g_enemy[nCntEnemy].bEscaping = false;
		g_enemy[nCntEnemy].type = ENEMY_WHITE;

	}

	//���f���Ɋւ���ϐ��̏�����
	strcpy( g_modelData[ENEMY_WHITE].filePath, MODEL_ENEMY_WHITE);
	strcpy( g_modelData[ENEMY_BLUE].filePath, MODEL_ENEMY_BLUE);
	strcpy( g_modelData[ENEMY_RED].filePath, MODEL_ENEMY_RED);

	//���f���Ɋւ���ϐ��̏�����
	for(int nCntType = 0; nCntType < ENEMY_TYPE_MAX; nCntType++)
	{
		g_modelData[nCntType].pD3DTexture = NULL;
		g_modelData[nCntType].pD3DXMesh = NULL;
		g_modelData[nCntType].pD3DXBuffMat = NULL;
		g_modelData[nCntType].nNumMat = 0;

		// X�t�@�C���̓ǂݍ���
		if(FAILED(D3DXLoadMeshFromX(
			g_modelData[nCntType].filePath,			// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
			D3DXMESH_SYSTEMMEM,						// ���b�V���̍쐬�I�v�V�������w��
			pDevice,								// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
			NULL,									// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			&g_modelData[nCntType].pD3DXBuffMat,	// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
			NULL,									// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
			&g_modelData[nCntType].nNumMat,			// D3DXMATERIAL�\���̂̐�
			&g_modelData[nCntType].pD3DXMesh		// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
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
void UninitEnemy(void)
{
	for(int nCntType = 0; nCntType < ENEMY_TYPE_MAX; nCntType++)
	{
		// ���b�V���̊J��
		if(g_modelData[nCntType].pD3DXMesh != NULL)
		{
			g_modelData[nCntType].pD3DXMesh->Release();
			g_modelData[nCntType].pD3DXMesh = NULL;
		}
		// �}�e���A���̊J��
		if(g_modelData[nCntType].pD3DXBuffMat != NULL)
		{
			g_modelData[nCntType].pD3DXBuffMat->Release();
			g_modelData[nCntType].pD3DXBuffMat = NULL;
		}
		//�e�N�X�`���̊J��
		if(g_modelData[nCntType].pD3DTexture != NULL)
		{
			g_modelData[nCntType].pD3DTexture->Release();
			g_modelData[nCntType].pD3DTexture = NULL;
		}
	}


	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�e�Ɣ���̊J��
		ReleaseShadow(g_enemy[nCntEnemy].nIdxShadow);

		g_enemy[nCntEnemy].bUse = false;
	}


}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(float fTimeSpeed)
{
	CAMERA *pCamera = GetCamera();
	PLAYER *pPlayer = GetPlayer();
	int nCountAlive = 0;

	//PrintDebugProc("enemy mark %d\n", g_enemy[0].nSeePFrame);

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{
			nCountAlive++;

			{//�v���C���[�ɋC�Â����f����
				if( !HitCheckMeshWall( g_enemy[nCntEnemy].pos, pCamera->posV, NULL, NULL) && pPlayer->bAlive && //�ǂɎՂ�ꂽ�� 
					//HitCheckBall( g_enemy[nCntEnemy].pos, 1000.0f, pCamera->posV, 20.0f) && //���̋����ȓ�
					HitCheckViewArea( g_enemy[nCntEnemy].pos, pCamera->posV, g_enemy[nCntEnemy].rot, D3DXToRadian( 90.0f) ) && //���ɂ͌����Ȃ�
					!IsDebugMode()) //�f�o�b�O���[�h�̂Ƃ��ɁA�����蔻��͂Ȃ�
				{
					if( g_enemy[nCntEnemy].nSeePFrame <= 1000) g_enemy[nCntEnemy].nSeePFrame++;
					if( g_enemy[nCntEnemy].nSeePFrame >= 50)
					{
						g_enemy[nCntEnemy].bSeePlayer = true;

						//�v���C���[���������̏���ۑ�
						if( g_enemy[nCntEnemy].bPatrol)
						{
							//�т�����}�[�N��ݒ�
							SetFlashMessage(MSG_EXCLAMATION);

							g_enemy[nCntEnemy].bPatrol = false;
						
							g_enemy[nCntEnemy].posPatrol = g_enemy[nCntEnemy].pos;
							g_enemy[nCntEnemy].rotPatrol = g_enemy[nCntEnemy].rotTarget;
						}
					}
				}
				else if( HitCheckBall( g_enemy[nCntEnemy].pos, 120.0f, pCamera->posV, 20.0f) ) //���̋����ȓ�
				{
					if( g_enemy[nCntEnemy].nSeePFrame <= 65535) g_enemy[nCntEnemy].nSeePFrame++;
					if( g_enemy[nCntEnemy].nSeePFrame >= 50)
					{
						g_enemy[nCntEnemy].bSeePlayer = true;

						//�v���C���[���������̏���ۑ�
						if( g_enemy[nCntEnemy].bPatrol)
						{
							g_enemy[nCntEnemy].bPatrol = false;
						
							g_enemy[nCntEnemy].posPatrol = g_enemy[nCntEnemy].pos;
							g_enemy[nCntEnemy].rotPatrol = g_enemy[nCntEnemy].rotTarget;
						}
					}					
				}
				else
				{
					if( g_enemy[nCntEnemy].nSeePFrame > 0)
					{
						g_enemy[nCntEnemy].nSeePFrame--;
					}
					g_enemy[nCntEnemy].bSeePlayer = false;
				}
			}

			//CoolDown
			if( g_enemy[nCntEnemy].nCoolDown > 0)
			{
				g_enemy[nCntEnemy].nCoolDown -= 1 * fTimeSpeed;
				if(g_enemy[nCntEnemy].nCoolDown <= 0)
				{
					g_enemy[nCntEnemy].nCoolDown = -1;
					g_enemy[nCntEnemy].bShot = false;
				}
			}

			//�v���C���[�ɋC�Â�����A�s������
			if( g_enemy[nCntEnemy].bSeePlayer || g_enemy[nCntEnemy].nSeePFrame >= 50)
			{
				if( !g_enemy[nCntEnemy].bEscaping)
				{
					//����
					g_enemy[nCntEnemy].rotTarget = GetRotationAngle( g_enemy[nCntEnemy].pos, pCamera->posV, g_enemy[nCntEnemy].rot);		
				}

				//�e�𔭎�
				if(!g_enemy[nCntEnemy].bShot)
				{
					//����
					g_enemy[nCntEnemy].rot = GetRotationAngle( g_enemy[nCntEnemy].pos, pCamera->posV, g_enemy[nCntEnemy].rot);

					if(g_enemy[nCntEnemy].type == ENEMY_RED)
					{
						for(int cnt = 0; cnt < 3; cnt++)
						{
							SetEnemyBullet( g_enemy[nCntEnemy].pos, pCamera->posV + D3DXVECTOR3( rand() % 60 - 30, rand() % 40 - 20, rand() % 60 - 30));
						}

						g_enemy[nCntEnemy].bShot = true;
						g_enemy[nCntEnemy].nCoolDown = COOLDOWN + rand() % 60;
						
						PlaySound(SOUND_LABEL_SE_ESHOT);
					}
					else if(g_enemy[nCntEnemy].type == ENEMY_BLUE)
					{
						SetEnemyBullet( g_enemy[nCntEnemy].pos, pCamera->posV);
						g_enemy[nCntEnemy].bShot = true;
						g_enemy[nCntEnemy].nCoolDown = COOLDOWN/2 + rand() % 60;
						
						PlaySound(SOUND_LABEL_SE_ESHOT);					
					}
					else
					{
						SetEnemyBullet( g_enemy[nCntEnemy].pos, pCamera->posV);
						g_enemy[nCntEnemy].bShot = true;
						g_enemy[nCntEnemy].nCoolDown = COOLDOWN + rand() % 60;
						
						PlaySound(SOUND_LABEL_SE_ESHOT);					
					}

				}

			}
			else
			{
				if( g_enemy[nCntEnemy].bPatrol)
				{
					//nothing
				}
				else
				{
					//�p�g���[���ɖ߂�
					g_enemy[nCntEnemy].rotTarget = GetRotationAngle( g_enemy[nCntEnemy].pos, g_enemy[nCntEnemy].posPatrol, g_enemy[nCntEnemy].rot);
					D3DXVECTOR3 vec = g_enemy[nCntEnemy].pos - g_enemy[nCntEnemy].posPatrol;
					if( D3DXVec3Length( &vec) < VALUE_MOVE)
					{
						g_enemy[nCntEnemy].bPatrol = true;
						g_enemy[nCntEnemy].pos = g_enemy[nCntEnemy].posPatrol;
						g_enemy[nCntEnemy].rotTarget = g_enemy[nCntEnemy].rotPatrol;
					}
				}
			}

			//�e�L���A�����鏈��
			//if( g_enemy[nCntEnemy].bShot && !g_enemy[nCntEnemy].bEscaping)
			//{
			//	float movRot = rand() % 180 - 90;
			//	g_enemy[nCntEnemy].rotTarget.y = g_enemy[nCntEnemy].rot.y + D3DXToRadian(movRot);
			//	if( g_enemy[nCntEnemy].rotTarget.y > D3DX_PI)
			//	{
			//		g_enemy[nCntEnemy].rotTarget.y -= D3DX_PI * 2.0f;
			//	}
			//	g_enemy[nCntEnemy].bEscaping = true;
			//}

			bool bMove = true;
			//�v���C���[�ɋ߂Â��߂��Ȃ��悤��
			D3DXVECTOR3 vec = g_enemy[nCntEnemy].pos - pPlayer->pos;
			if( (D3DXVec3Length(&vec) < 200.0f) && g_enemy[nCntEnemy].bSeePlayer)
			{
				bMove = false;
			}

			{//�ړ�����


				if( bMove)
				{
					//�ړ������̏�����
					g_enemy[nCntEnemy].move.x = VALUE_MOVE;
					g_enemy[nCntEnemy].move.z = VALUE_MOVE;	

					if(g_enemy[nCntEnemy].type == ENEMY_BLUE)
					{
						g_enemy[nCntEnemy].move.x = 0;
						g_enemy[nCntEnemy].move.z = 0;
					}
				}


				//���v���A�܂��͋t���v�������߂�
				g_enemy[nCntEnemy].rotAngle =  Get2VecRotAngle( g_enemy[nCntEnemy].rot, g_enemy[nCntEnemy].rotTarget, DIVIDE_ROTATE, -1);


				//��]����
				g_enemy[nCntEnemy].rotAngle += (-g_enemy[nCntEnemy].rotAngle) * 0.001f;

				//���f��Y���p�x�C��
				if(g_enemy[nCntEnemy].rot.y > D3DX_PI)
				{
					g_enemy[nCntEnemy].rot.y = g_enemy[nCntEnemy].rot.y - D3DX_PI*2;
				}
				else if(g_enemy[nCntEnemy].rot.y < 0.0f - D3DX_PI)
				{
					g_enemy[nCntEnemy].rot.y = g_enemy[nCntEnemy].rot.y + D3DX_PI*2;
				}

				//���̉�]�ʒu�ɓ���������
				if( fabs(g_enemy[nCntEnemy].rot.y - g_enemy[nCntEnemy].rotTarget.y) < VALUE_ROTATE &&
					fabs(g_enemy[nCntEnemy].rot.x - g_enemy[nCntEnemy].rotTarget.x) < VALUE_ROTATE &&
					fabs(g_enemy[nCntEnemy].rot.z - g_enemy[nCntEnemy].rotTarget.z) < VALUE_ROTATE
					)
				{
					g_enemy[nCntEnemy].rot = g_enemy[nCntEnemy].rotTarget;
					g_enemy[nCntEnemy].rotAngle = D3DXVECTOR3( 0, 0, 0);
				}
				else //���̉�]�ʒu�ɂ܂��������ĂȂ�
				{
					g_enemy[nCntEnemy].rot += g_enemy[nCntEnemy].rotAngle * fTimeSpeed;
				}	

				//��]���Ă��Ȃ���
				if(g_enemy[nCntEnemy].rotAngle == D3DXVECTOR3( 0, 0, 0))
				{
					//��������
					g_enemy[nCntEnemy].move.x -= g_enemy[nCntEnemy].move.x * 0.05f;
					g_enemy[nCntEnemy].move.z -= g_enemy[nCntEnemy].move.z * 0.05f;

					D3DXVECTOR3 tNextPos = g_enemy[nCntEnemy].pos;
					tNextPos.x += g_enemy[nCntEnemy].move.x * sinf(g_enemy[nCntEnemy].rot.y);
					tNextPos.z += g_enemy[nCntEnemy].move.z * cosf(g_enemy[nCntEnemy].rot.y);

					D3DXVECTOR3 tSphere  = D3DXVECTOR3( 0, 0, 0);
					D3DXVECTOR3 front = tNextPos - g_enemy[nCntEnemy].pos;
					D3DXVec3Normalize( &tSphere, &front);
					tSphere *= MODEL_SPHERE;

					//�ǂ̖@���x�N�g�����擾�ϐ��̏�����
					D3DXVECTOR3 wall_nor = D3DXVECTOR3(0,0,0);

					//Side
					g_enemy[nCntEnemy].posLSide.x = g_enemy[nCntEnemy].pos.x + 15.0f * sinf(g_enemy[nCntEnemy].rotTarget.y + D3DXToRadian(-90.0f));
					g_enemy[nCntEnemy].posLSide.z = g_enemy[nCntEnemy].pos.z + 15.0f * cosf(g_enemy[nCntEnemy].rotTarget.y + D3DXToRadian(-90.0f));

					g_enemy[nCntEnemy].posRSide.x = g_enemy[nCntEnemy].pos.x + 15.0f * sinf(g_enemy[nCntEnemy].rotTarget.y + D3DXToRadian(90.0f));
					g_enemy[nCntEnemy].posRSide.z = g_enemy[nCntEnemy].pos.z + 15.0f * cosf(g_enemy[nCntEnemy].rotTarget.y + D3DXToRadian(90.0f));

					//�ǂɓ���������
					if( HitCheckMeshWall( g_enemy[nCntEnemy].posLSide, g_enemy[nCntEnemy].posLSide + tSphere, &wall_nor, NULL) ||
						HitCheckMeshWall( g_enemy[nCntEnemy].posRSide, g_enemy[nCntEnemy].posRSide + tSphere, &wall_nor, NULL) )
					{

						if( g_enemy[nCntEnemy].bSeePlayer || !g_enemy[nCntEnemy].bPatrol)
						{
							//�ǂ��菈���F�ړ��x�N�g�����C��
							GetWallScratchVector( &front, front, wall_nor);
				
							D3DXVec3Normalize( &tSphere, &front);
							tSphere *= MODEL_SPHERE;

							if( !HitCheckMeshWall( g_enemy[nCntEnemy].pos, g_enemy[nCntEnemy].pos + tSphere, &wall_nor, NULL) )
							{
								//�ړ�����
								g_enemy[nCntEnemy].pos += front;
							}
						}
						
						if( g_enemy[nCntEnemy].bPatrol)
						{
							//180���]
							g_enemy[nCntEnemy].rotTarget.y = g_enemy[nCntEnemy].rot.y + D3DXToRadian(180.0f);
							if( g_enemy[nCntEnemy].rotTarget.y > D3DX_PI)
							{
								g_enemy[nCntEnemy].rotTarget.y -= D3DX_PI * 2.0f;
							}	
						}



					}
					else
					{
						//�ړ�����
						g_enemy[nCntEnemy].pos += front * fTimeSpeed;
					}		

				}			
			}

			//shadow
			SetPositionShadow( g_enemy[nCntEnemy].nIdxShadow, g_enemy[nCntEnemy].pos);

		}
	}

	if( nCountAlive == 0)
	{
		PlaySound( SOUND_LABEL_SE_CLEAR);
		SetGameStep(STEP_CLEAR);
	}

	//PrintDebugProc("\n�G�̈ʒu�F(X:%f, Y:%f, Z:%f)\n", g_enemy[0].pos.x, g_enemy[0].pos.y, g_enemy[0].pos.z);
	//PrintDebugProc("�G�̌����@�F(X:%f, Y:%f, Z:%f)\n", g_enemy[0].rot.x, D3DXToDegree(g_enemy[0].rot.y), g_enemy[0].rot.z);
	//if( g_enemy[0].bSeePlayer) PrintDebugProc("see Player\n");
	//if( g_enemy[0].bPatrol)PrintDebugProc("patral\n");

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if(g_enemy[nCntEnemy].bUse)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity( &g_enemy[nCntEnemy].mtxWorld);
	
			// �X�P�[���𔽉f
			D3DXMatrixScaling( &mtxScl, g_enemy[nCntEnemy].scl.x, g_enemy[nCntEnemy].scl.y, g_enemy[nCntEnemy].scl.z);
			D3DXMatrixMultiply( &g_enemy[nCntEnemy].mtxWorld, &g_enemy[nCntEnemy].mtxWorld, &mtxScl);
	
			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll( &mtxRot, g_enemy[nCntEnemy].rot.y, g_enemy[nCntEnemy].rot.x, g_enemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply( &g_enemy[nCntEnemy].mtxWorld, &g_enemy[nCntEnemy].mtxWorld, &mtxRot);
	
			// �ړ��𔽉f
			D3DXMatrixTranslation( &mtxTranslate, g_enemy[nCntEnemy].pos.x, g_enemy[nCntEnemy].pos.y, g_enemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply( &g_enemy[nCntEnemy].mtxWorld, &g_enemy[nCntEnemy].mtxWorld, &mtxTranslate);
	
			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform( D3DTS_WORLD, &g_enemy[nCntEnemy].mtxWorld);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);
	
			
			ENEMY_TYPE type = g_enemy[nCntEnemy].type;
			// �}�e���A�����ɑ΂���|�C���^���擾
			pD3DXMat = (D3DXMATERIAL *)g_modelData[type].pD3DXBuffMat->GetBufferPointer();
			for(int nCntMat = 0; nCntMat < (int)g_modelData[type].nNumMat; nCntMat++)
			{
				// �}�e���A���̐ݒ�
				pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
				// �e�N�X�`���̐ݒ�
				pDevice->SetTexture( 0, g_modelData[type].pD3DTexture);
				// �`��
				g_modelData[type].pD3DXMesh->DrawSubset( nCntMat);
			}

			// �}�e���A�����f�t�H���g�ɖ߂�
			pDevice->SetMaterial( &matDef);		
		}
	}

}

//=============================================================================
// ���f���̎擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_enemy[0];
}

void SetEnemy( D3DXVECTOR3 pos, float rotY, ENEMY_TYPE type)
{
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if( !g_enemy[nCntEnemy].bUse)
		{
			g_enemy[nCntEnemy].pos = pos;
			g_enemy[nCntEnemy].rotTarget.y = rotY;
			g_enemy[nCntEnemy].rot.y = rotY;
			g_enemy[nCntEnemy].type = type;
			g_enemy[nCntEnemy].bUse = true;
			g_enemy[nCntEnemy].nIdxShadow = CreateShadow( g_enemy[nCntEnemy].pos, 50.0f, 50.0f);
			break;
		}
	}
}




