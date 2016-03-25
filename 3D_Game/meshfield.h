/*******************************************************************************
*
* �^�C�g��:		���b�V���t�B�[���h����
* �v���O������:	meshfield.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshField(int nNumBlockX, int nNumBlockZ, float fSizeBlockX, float fSizeBlockZ);
void UninitMeshField(void);
//void UpdateMeshField(void);
void DrawMeshField(void);

#endif
