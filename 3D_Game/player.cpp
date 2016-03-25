/*******************************************************************************
*
* �^�C�g��:		�v���C���[����
* �v���O������:	player.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "player.h"
#include "debugproc.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "shadow.h"
#include "meshwall.h"
#include "hitCheck.h"
#include "game.h"
#include "gun.h"
#include "playerBullet.h"
#include "playerBulletNum.h"
#include "playerLife.h"
#include "fountainCircle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_PLAYER	"data/MODEL/player.x"					// �ǂݍ��ރe�N�X�`���t�@�C����

#define VALUE_MOVE		(3.0f)									// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.1f) 						// ��]��
#define DIVIDE_ROTATE	(3) 									// ��]�ʂ������ɕ�����

#define PLAYER_POS_X	(-50)
#define PLAYER_POS_Y	(50)
#define PLAYER_POS_Z	(-150)


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
PLAYER g_player;
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_player.pos = D3DXVECTOR3( PLAYER_POS_X, PLAYER_POS_Y, PLAYER_POS_Z);
	g_player.posLSide = g_player.posRSide = g_player.pos;
	g_player.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_player.scl = D3DXVECTOR3( 1.0f, 1.0f, 1.0f);
	g_player.move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
	g_player.rotAngle = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_player.rotTarget = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_player.jumpTime = 0;
	g_player.inAir = false;
	g_player.bAlive = true;
	g_player.bRun = false;
	g_player.nBullet = INI_NUM_LIFE;
	g_player.nLife = INI_NUM_BULLET;

	// ���f���Ɋւ���ϐ��̏�����							
	g_player.pD3DTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	g_player.pD3DXMesh = NULL;		// ���b�V�����ւ̃|�C���^
	g_player.pD3DXBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
	g_player.nNumMat = 0;				// �}�e���A�����̐�

	// X�t�@�C���̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(
		MODEL_PLAYER,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,			// ���b�V���̍쐬�I�v�V�������w��
		pDevice,					// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,						// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_player.pD3DXBuffMat,		// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,						// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_player.nNumMat,			// D3DXMATERIAL�\���̂̐�
		&g_player.pD3DXMesh			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		)))
	{
		return E_FAIL;
	}		

	g_player.nIdxShadow = CreateShadow( g_player.pos, 50.0f, 50.0f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	// �e�N�X�`���̊J��

	// ���b�V���̊J��
	if(g_player.pD3DXMesh != NULL)
	{
		g_player.pD3DXMesh->Release();
		g_player.pD3DXMesh = NULL;
	}
	// �}�e���A���̊J��
	if(g_player.pD3DXBuffMat != NULL)
	{
		g_player.pD3DXBuffMat->Release();
		g_player.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *pCamera = GetCamera();
	GUN *pGun = GetGun();

	//�e������
	if((GetMouseLeftTrigger() || GetGamePadTrigger(0, BUTTON_R1)) && !pGun->bShot)
	{
		//�e����
		if( g_player.nBullet > 0 && IsGunReady())
		{
			PlaySound(SOUND_LABEL_SE_PSHOT);

			SetPlayerBullet( pGun->pos, pCamera->posR);
		
			SetGameStep(STEP_SHOOT);

			SetGunShootAni();

			g_player.nBullet--;
			ChangeNumBullet( 0, g_player.nBullet);
		}
		else//�e�Ȃ�
		{
			if(IsGunReady())
			{
				SetGunBulletOutAni(true);
				PlaySound(SOUND_LABEL_SE_BULLET_OUT);
			}
		}

	}
	if(GetMouseRightTrigger() && IsDebugMode())
	{
		g_player.nBullet = 3;
		ChangeNumBullet( 0, g_player.nBullet);
	}



	//JUMP
	if(GetKeyboardPress(DIK_SPACE) || GetGamePadTrigger( 0, BUTTON_A))
	{
		g_player.move.y = VALUE_MOVE * 1.4f;
		g_player.inAir = true;
	}
	if( g_player.inAir)
	{
		ChangeTimeSpeed( 0.5f);
		g_player.jumpTime++;
		g_player.pos.y = PLAYER_POS_Y + g_player.move.y * g_player.jumpTime - 0.5f * 0.2f * g_player.jumpTime * g_player.jumpTime;
	}
	if( g_player.pos.y < PLAYER_POS_Y)
	{
		g_player.inAir = false;
		g_player.jumpTime = 0;
	}

	//�ړ�����
	bool isKeyPressed = false;
	//�΂߈ړ�
	if( (GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_W)) ||(GetGamePadTrigger( 0, LSTICK_RIGHT) && GetGamePadTrigger( 0, LSTICK_UP)) ) //�E��
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(45.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(45.0f);
		if( g_player.rotTarget.y > D3DX_PI)
		{
			g_player.rotTarget.y -= D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if((GetKeyboardPress(DIK_D) && GetKeyboardPress(DIK_S)) ||(GetGamePadTrigger( 0, LSTICK_RIGHT) && GetGamePadTrigger( 0, LSTICK_DOWN)) ) //�E��
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(135.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(135.0f);
		if( g_player.rotTarget.y > D3DX_PI)
		{
			g_player.rotTarget.y -= D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if((GetKeyboardPress(DIK_A)  && GetKeyboardPress(DIK_W)) ||(GetGamePadTrigger( 0, LSTICK_LEFT) && GetGamePadTrigger( 0, LSTICK_UP)) ) //����
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(-45.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(-45.0f);
		if( g_player.rotTarget.y < -D3DX_PI)
		{
			g_player.rotTarget.y += D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if((GetKeyboardPress(DIK_A) && GetKeyboardPress(DIK_S)) ||(GetGamePadTrigger( 0, LSTICK_LEFT) && GetGamePadTrigger( 0, LSTICK_DOWN)) ) //����
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(-135.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(-135.0f);
		if( g_player.rotTarget.y < -D3DX_PI)
		{
			g_player.rotTarget.y += D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if(GetKeyboardPress(DIK_W) || GetGamePadTrigger( 0, LSTICK_UP))
	{
		//g_player.rot.y = pCamera->rot.y;
		g_player.rotTarget.y = pCamera->rot.y;
		isKeyPressed = true;
	}
	else if(GetKeyboardPress(DIK_S) || GetGamePadTrigger( 0, LSTICK_DOWN))
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(180.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(180.0f);
		if( g_player.rotTarget.y > D3DX_PI)
		{
			g_player.rotTarget.y -= D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if(GetKeyboardPress(DIK_A) || GetGamePadTrigger( 0, LSTICK_LEFT))
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(-90.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(-90.0f);
		if( g_player.rotTarget.y < -D3DX_PI)
		{
			g_player.rotTarget.y += D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	else if(GetKeyboardPress(DIK_D) || GetGamePadTrigger( 0, LSTICK_RIGHT))
	{
		//g_player.rot.y = pCamera->rot.y + D3DXToRadian(90.0f);
		g_player.rotTarget.y = pCamera->rot.y + D3DXToRadian(90.0f);
		if( g_player.rotTarget.y > D3DX_PI)
		{
			g_player.rotTarget.y -= D3DX_PI * 2.0f;
		}
		isKeyPressed = true;
	}
	if(isKeyPressed)
	{
		if(GetKeyboardPress(DIK_LSHIFT) || GetGamePadPress( 0, BUTTON_L1))
		{
			//�ړ������̏�����
			g_player.move.x = VALUE_MOVE * 1.5f;
			g_player.move.z = VALUE_MOVE * 1.5f;

			ChangeTimeSpeed( 1.5f);
		}
		else
		{
			//�ړ������̏�����
			g_player.move.x = VALUE_MOVE;
			g_player.move.z = VALUE_MOVE;

			ChangeTimeSpeed( 1.0f);
		}	

		//���v���A�܂��͋t���v�������߂�
		g_player.rotAngle =  Get2VecRotAngle( g_player.rot, g_player.rotTarget, DIVIDE_ROTATE, VALUE_ROTATE);


		if(!g_player.bRun)
		{
			//���鉹
			PlaySound(SOUND_LABEL_SE_RUN);
			SetGunShakeAni(true);
		}
		g_player.bRun = true;
	}
	else
	{
		if(g_player.bRun)
		{
			//���鉹 �ꎞ��~
			StopSound(SOUND_LABEL_SE_RUN);
			SetGunShakeAni(false);
		}
		g_player.bRun = false;
	}


	//��]����
	g_player.rotAngle.y += (-g_player.rotAngle.y) * 0.001f;

	//���f���p�x�C��
	if(g_player.rot.y > D3DX_PI)
	{
		g_player.rot.y = g_player.rot.y - D3DX_PI*2;
	}
	else if(g_player.rot.y < 0.0f - D3DX_PI)
	{
		g_player.rot.y = g_player.rot.y + D3DX_PI*2;
	}

	//���̉�]�ʒu�ɓ���������
	if(abs(g_player.rot.y - g_player.rotTarget.y) < VALUE_ROTATE)
	{
		g_player.rot.y = g_player.rotTarget.y;
		g_player.rotAngle.y = 0;
	}
	//���̉�]�ʒu�ɂ܂��������ĂȂ�
	else
	{
		g_player.rot.y += g_player.rotAngle.y;
	}	
	


	//��]���Ă��Ȃ���
	if(g_player.rotAngle.y == 0)
	{
		D3DXVECTOR3 tNextPos = g_player.pos;
		tNextPos.x += g_player.move.x * sinf(g_player.rot.y);
		tNextPos.z += g_player.move.z * cosf(g_player.rot.y);

		//��������
		g_player.move.x -= g_player.move.x * 0.25f;
		g_player.move.z -= g_player.move.z * 0.25f;

		D3DXVECTOR3 tSphere  = D3DXVECTOR3( 0, 0, 0);
		D3DXVECTOR3 front = tNextPos - g_player.pos;
		D3DXVec3Normalize( &tSphere, &front);
		tSphere *= 20.0f;


		//�ǂ̖@��
		D3DXVECTOR3 wall_nor = D3DXVECTOR3(0,0,0);

		//Side
		g_player.posLSide.x = g_player.pos.x +  15.0f * sinf(g_player.rotTarget.y+D3DXToRadian(-90.0f));
		g_player.posLSide.z = g_player.pos.z +  15.0f * cosf(g_player.rotTarget.y+D3DXToRadian(-90.0f));
		g_player.posLSide.y = g_player.pos.y;
											
		g_player.posRSide.x = g_player.pos.x +  15.0f * sinf(g_player.rotTarget.y+D3DXToRadian(90.0f));
		g_player.posRSide.z = g_player.pos.z +  15.0f * cosf(g_player.rotTarget.y+D3DXToRadian(90.0f));
		g_player.posLSide.y = g_player.pos.y;

		//�ǂɓ���������
		if( //HitCheckMeshWall( g_player.posRSide, g_player.posRSide + tSphere, &wall_nor, NULL) ||
			//HitCheckMeshWall( g_player.posLSide, g_player.posLSide + tSphere, &wall_nor, NULL) ||	
			HitCheckMeshWall( g_player.pos, g_player.pos + tSphere, &wall_nor, NULL)
			)
		{

			//�ǂ��菈���F�ړ��x�N�g�����C��
			GetWallScratchVector( &front, front, wall_nor);
				
			D3DXVec3Normalize( &tSphere, &front);
			tSphere *= 20.0f;

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			if( !HitCheckMeshWall( g_player.pos, g_player.pos + tSphere, &wall_nor, NULL) &&
				!HitCheckMeshWall( g_player.posLSide, g_player.posLSide + tSphere, &wall_nor, NULL) &&
				!HitCheckMeshWall( g_player.posRSide, g_player.posRSide + tSphere, &wall_nor, NULL))
			{
				//�ړ�����
				g_player.pos += front;
			}

		}
		else
		{
			//�ړ�����
			g_player.pos += front;
		}

		//�����Ƃ̓����蔻��
		FOUNTAIN_CIRCLE *pCircle = GetFountainCircle();
		if( pCircle->bUse && HitCheckXZ( g_player.pos, 20.0f, pCircle->pos, pCircle->fRadius))
		{
			g_player.pos -= front;	
		}

	}

	if( IsDebugMode())
	{
		//���f���㉺�ړ�
		if(GetKeyboardPress(DIK_PRIOR))
		{
			g_player.pos.y += VALUE_MOVE * 0.5f;
		}
		if(GetKeyboardPress(DIK_NEXT))
		{
			g_player.pos.y -= VALUE_MOVE * 0.5f;
		}
	
	}
	
	//shadow
	SetPositionShadow( g_player.nIdxShadow, g_player.pos);

	//PrintDebugProc("Player�̌����@�F(X:%f, Y:%f, Z:%f)\n", g_player.rot.x, g_player.rot.y, g_player.rot.z);
	//PrintDebugProc("Player�̈ʒu�F(X:%f, Y:%f, Z:%f)\n", g_player.pos.x, g_player.pos.y, g_player.pos.z);
	//PrintDebugProc("Left�̈ʒu�F(X:%f, Y:%f, Z:%f)\n", g_player.posLSide.x, g_player.posLSide.y, g_player.posLSide.z);
	//PrintDebugProc("Right�̈ʒu�F(X:%f, Y:%f, Z:%f)\n", g_player.posRSide.x, g_player.posRSide.y, g_player.posRSide.z);

	

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_player.mtxWorld);
	
	// �X�P�[���𔽉f
	D3DXMatrixScaling( &mtxScl, g_player.scl.x, g_player.scl.y, g_player.scl.z);
	D3DXMatrixMultiply( &g_player.mtxWorld, &g_player.mtxWorld, &mtxScl);
	
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply( &g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);
	
	// �ړ��𔽉f
	D3DXMatrixTranslation( &mtxTranslate, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply( &g_player.mtxWorld, &g_player.mtxWorld, &mtxTranslate);
	
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform( D3DTS_WORLD, &g_player.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);
	
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL *)g_player.pD3DXBuffMat->GetBufferPointer();
	for(int nCntMat = 0; nCntMat < (int)g_player.nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture( 0, g_player.pD3DTexture);
		// �`��
		g_player.pD3DXMesh->DrawSubset( nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial( &matDef);
}

//=============================================================================
// ���f���̎擾
//=============================================================================
PLAYER *GetPlayer(void)
{
	return &g_player;
}

void SetPlayerDeath(void)
{
	//����������
	g_player.bRun = false;
	StopSound(SOUND_LABEL_SE_RUN);

	g_player.bAlive = false;
	ChangeNumLife( 0, --g_player.nLife);

	CAMERA *pCamera = GetCamera();
	//�J������]
	float a = pCamera->rot.y + D3DXToRadian(90.0f);
	float x = sinf(a);
	float z = cosf(a);
	pCamera->vecU = D3DXVECTOR3( x, 0.0f, z);

	//�n�ʂ�
	pCamera->posV.y = 30.0f;
	
	ReleaseShadow( g_player.nIdxShadow);

	SetGameStep(STEP_DIE);
}

//=============================================================================
// �v���C���[�̐ݒu
//=============================================================================
void SetPlayer( D3DXVECTOR3 pos, float rotY, int numBullet, int numLife)
{
	CAMERA *pCamera = GetCamera();

	g_player.pos = pos;
	g_player.rot.y = rotY;
	pCamera->rot.y = rotY;
	g_player.nBullet = numBullet;
	g_player.nLife = numLife;

	//UI�̏�����
	ChangeNumBullet( 0, numBullet);
	ChangeNumLife( 0, numLife);
}

