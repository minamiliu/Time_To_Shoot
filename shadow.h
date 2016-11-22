/*******************************************************************************
*
* �^�C�g��:		�ۉe����
* �v���O������:	shadow.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitShadow(void);
void UninitShadow(void);
//void UpdateShadow(void);
void DrawShadow(void);

//�e�̐���
int CreateShadow(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
//�e�̊J��
void ReleaseShadow(int nIdxShadow);
//�e�̍��W�ݒu
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);

//�e�̐F��ς���
void SetColorShadow(int nIdxShadow, D3DXCOLOR col);

#endif
