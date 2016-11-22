/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`���e�̏Ə��폈��
* �v���O������:	gunSight.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _GUNSIGHT_H_
#define _GUNSIGHT_H_

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "main.h"

/*******************************************************************************
* ���C�u�����̃����N
*******************************************************************************/


/*******************************************************************************
* �}�N����`
*******************************************************************************/


/*******************************************************************************
* �\���̒�`
*******************************************************************************/
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 size;	//�T�C�Y
	bool bUse;

}GUNSIGHT;
/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitGunSight(void);
void DrawGunSight(void);		
void UninitGunSight(void);
void UpdateGunSight(void);
										
#endif