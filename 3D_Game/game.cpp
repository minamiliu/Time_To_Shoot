/*******************************************************************************
*
* �^�C�g��:		�Q�[���S�̏���
* �v���O������:	game.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/


#include "game.h"
#include "debugproc.h"
#include "input.h"
#include "fade.h"
#include "message.h"
#include "sound.h"

#include "camera.h"
#include "light.h"
#include "time.h"
#include "clock.h"
#include "gun.h"
#include "gunSight.h"
#include "playerLife.h"
#include "enemyNum.h"

#include "stageManager.h"
#include "meshdome.h"
#include "meshwall.h"
#include "meshfield.h"
#include "shadow.h"
#include "particle.h"
#include "player.h"
#include "playerBullet.h"
#include "playerBulletNum.h"
#include "enemy.h"
#include "enemyBullet.h"
#include "itemBullet.h"
#include "fountain.h"
#include "fountainCircle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
GAME_STEP g_gameStep;
int g_nCounterFrame;
int g_nCounterShoot;
int g_nCounterEShoot;
float g_fTimeSpeed;
bool g_bNeedRseet;
bool g_bGameClear;


/*******************************************************************************
�֐���:	void InitGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̏���������
*******************************************************************************/
void InitGame(void)
{
	//�Q�[�����[�h�̏�����
	SetGameStep(STEP_PLAY);

	//stage�̏�����
	SetStageMode(STAGE0);

	// ���C�g�̏�����
	InitLight();

	//dome
	InitMeshDome( D3DXVECTOR3( 0, 0, 0), D3DXVECTOR3( 0, 0, 0), 1500.0f, 32, 32);

	//UI
	InitTime();
	InitClock(D3DXVECTOR3(SCREEN_WIDTH/2 + 100, 70, 0));
	InitGunSight();
	InitNumLife();
	InitEnemyNum();
	
	//Model
	InitStageManager(true, INI_NUM_LIFE);
	//InitModel();

	//�O���[�o���ϐ��̏�����
	g_nCounterFrame = 0;
	g_nCounterShoot = 0;
	g_nCounterEShoot = 0;
	g_fTimeSpeed = 0.01f;
	g_bNeedRseet = false;
	g_bGameClear = false;
}

/*******************************************************************************
�֐���:	void UpdateGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̍X�V�֐�
*******************************************************************************/
void UpdateGame(void)
{
	//�����l
	g_fTimeSpeed = 0.01f;

	GAME_STEP stepNow = GetGameStep();
	switch(stepNow)
	{
		case STEP_PLAY:

			UpdatePlayer();
			UpdateCamera();
			UpdateGun();
			
			UpdatePlayerBullet(g_fTimeSpeed);
			UpdateEnemy( g_fTimeSpeed);
			UpdateEnemyBullet( g_fTimeSpeed);

			//UpdateBillBoard();
			UpdateParticle( g_fTimeSpeed);
			UpdateStageManager( g_fTimeSpeed);

			//UpdateModel();
			UpdateGunSight();
			UpdateItemBullet();
			UpdateClock( g_fTimeSpeed);
			UpdateTime( g_fTimeSpeed);

			if(GetKeyboardTrigger(DIK_P) )
			{
				SetGameStep(STEP_PAUSE);
				SetMessage(MSG_PAUSE);
			}
			break;
		case STEP_SHOOT:
			
			ChangeTimeSpeed( 1.0f);
			
			UpdateEnemy( g_fTimeSpeed);
			//UpdatePlayer();
			UpdatePlayerBullet( g_fTimeSpeed);
			//UpdateEnemyBullet( g_fTimeSpeed);
			UpdateClock( g_fTimeSpeed);
			UpdateTime( g_fTimeSpeed);

			g_nCounterShoot++;
			if( g_nCounterShoot > 15)
			{
				g_nCounterShoot = 0;
				SetGameStep(STEP_PLAY);
			}
			break;
		case STEP_PAUSE:
			StopSound();
			if(GetKeyboardTrigger(DIK_P) )
			{
				SetGameStep(STEP_PLAY);
				ClearMessage(MSG_PAUSE);
			}
			break;
		case STEP_DIE:
			ChangeTimeSpeed( 1.0f);

			UpdateEnemy( g_fTimeSpeed);
			UpdateEnemyBullet( g_fTimeSpeed);
			UpdatePlayerBullet( g_fTimeSpeed);
			UpdateParticle( g_fTimeSpeed);

			g_nCounterFrame++;
			if( g_nCounterFrame > 90)
			{
				g_nCounterFrame = 0;
				if( GetPlayer()->nLife <= 0)
				{
					FadeOutToNext(MODE_RESULT);
				}
				else
				{
					FadeOutToNext(STEP_RESET);
					SetGameStep(STEP_NOTHING);
				}
				
			}

			break;
		case STEP_RESET:
			InitStageManager( false, GetPlayer()->nLife);
			SetGameStep(STEP_PLAY);	
			break;
		case STEP_CLEAR:
			StopSound(SOUND_LABEL_SE_RUN);
			SetMessage(MSG_STAGECLEAR);

			g_nCounterFrame++;
			if( g_nCounterFrame > 90)
			{
				g_nCounterFrame = 0;

				switch(GetStageMode())
				{
				case STAGE0:
					FadeOutToNext(STAGE1);
					break;
				case STAGE1:
					FadeOutToNext(STAGE2);
					break;
				case STAGE2:
					FadeOutToNext(STAGE3);
					break;
				case STAGE3:
					FadeOutToNext(MODE_RESULT);
					g_bGameClear = true;
					break;
				}
				
				SetGameStep(STEP_NOTHING);
			}
			break;
		case STEP_NOTHING:
			//just let time go, and do nothing
			break;
	}	

	//UI update
	UpdateNumBullet();

	//Debug update
	UpdateMeshDome();

	if(GetKeyboardTrigger(DIK_RETURN) )
	{	
		FadeOutToNext(MODE_RESULT);
	}
	if(GetKeyboardTrigger(DIK_F3))
	{	
		PlaySound( SOUND_LABEL_SE_CLEAR);
		SetGameStep(STEP_CLEAR);
	}
	if(GetKeyboardTrigger(DIK_F2))
	{
		PlaySound(SOUND_LABEL_SE_SWITCH);
		SwitchDebug();
	}
}

/*******************************************************************************
�֐���:	void DrawGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̕`��֐�
*******************************************************************************/
void DrawGame(void)
{
	//�J�����̐ݒu
	SetCamera();

	//�X�e�[�W�̕`�揈��
	DrawStageManager();
	DrawMeshDome();
	
	//DrawModel();
	DrawParticle();
	DrawEnemy();
	DrawEnemyBullet();
	DrawItemBullet();

	if( IsDebugMode()) DrawPlayer();
	
	DrawPlayerBullet();
	DrawGun();
	DrawShadow();
	//DrawBillBoard();	

	DrawTime();
	DrawGunSight();
	DrawNumBullet();
	DrawNumLife();
	DrawClock();
	DrawEnemyNum();
}
/*******************************************************************************
�֐���:	void UninitGame(void)
����:	�Ȃ�
�߂�l:	�Ȃ�
����:	�Q�[���̊J���֐�
*******************************************************************************/
void UninitGame(void)
{
	// �J�����̏I������
	UninitCamera();
	
	// ���C�g�̏I������
	UninitLight();

	// �X�e�[�W�̏I������
	UninitStageManager();
	UninitMeshDome();

	//UninitModel();
	UninitPlayer();
	UninitPlayerBullet();
	UninitGun();
	UninitEnemy();
	UninitEnemyBullet();
	UninitShadow();

	//UninitTime();
	UninitGunSight();
	UninitNumBullet();
	UninitNumLife();
	UninitItemBullet();

	//UninitBillBoard();
	UninitFountain();
	UninitParticle();
	UninitClock();
	UninitEnemyNum();
}

void SetGameStep(GAME_STEP step)
{
	g_gameStep = step;
}

GAME_STEP GetGameStep(void)
{
	return g_gameStep;
}

void ChangeTimeSpeed( float fTimeSpeed)
{
	if( fTimeSpeed > g_fTimeSpeed)
	{
		g_fTimeSpeed = fTimeSpeed;
	}
}

bool IsGameClear(void)
{
	return g_bGameClear;
}






