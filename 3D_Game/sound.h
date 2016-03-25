/*******************************************************************************
*
* �^�C�g��:		�T�E���h����
* �v���O������:	sound.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�t�@�C��
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_OP = 0,		// BGM0
	SOUND_LABEL_BGM_RANK,		// BGM1
	SOUND_LABEL_SE_ESHOT,		// �G�e���ˉ�
	SOUND_LABEL_SE_PSHOT,		// �v���C���[�e���ˉ�
	SOUND_LABEL_SE_RELOAD,		// RELOAD
	SOUND_LABEL_SE_BULLET_OUT,	// �e�؂�
	SOUND_LABEL_SE_CLEAR,		// STAGE CLEAR
	SOUND_LABEL_SE_WATER,		// ����
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_HURT,		// HURT
	SOUND_LABEL_SE_RUN,			// RUN
	SOUND_LABEL_SE_SWITCH,		// SWITCH
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

//���̉��ʍX�V
void UpdateSoundVolume(SOUND_LABEL label, float percent);
#endif
