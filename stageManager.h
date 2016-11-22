/*******************************************************************************
*
* �^�C�g��:		�X�e�[�W�Ǘ�
* �v���O������:	stageManager.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _STAGEMANAGER_H_
#define _STAGEMANAGER_H_

#include "main.h"
#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_STAGE	(3)
#define STAGE_ROOF	(200)
#define STAGE_WALL_HEIGHT	(100)

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

//�Q�[���̑��X�e�[�W
typedef enum
{
	STAGE0 = 0,
	STAGE1,
	STAGE2,
	STAGE3,
	STAGE_MAX
}STAGE_MODE;


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitStageManager(bool bFirst, int playerLife);
void UpdateStageManager(float fTimeSpeed);
void DrawStageManager(void);
void UninitStageManager(void);

//�X�e�[�W���[�h���擾
STAGE_MODE GetStageMode(void);
//�X�e�[�W���[�h��ݒu
void SetStageMode(STAGE_MODE mode);
//���̃X�e�[�W��
bool NextStage(void);
#endif