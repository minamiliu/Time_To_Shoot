/*******************************************************************************
*
* �^�C�g��:		�t�F�[�h����
* �v���O������:	fade.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _FADE_H_
#define _FADE_H_

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"
#include "game.h"
#include "stageManager.h"

/*******************************************************************************
* ���C�u�����̃����N
*******************************************************************************/


/*******************************************************************************
* �}�N����`
*******************************************************************************/


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_CHANGE,
	FADE_OUT,
	FADE_MAX
}FADE;

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitFade(void);
void DrawFade(void);		
void UninitFade(void);
void UpdateFade(void);

void FadeOutToNext(MODE mode);
void FadeOutToNext(GAME_STEP mode);
void FadeOutToNext(STAGE_MODE mode);
#endif