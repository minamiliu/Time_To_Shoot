/*******************************************************************************
*
* �^�C�g��:		���b�V���E�H�[������
* �v���O������:	meshwall.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
						int nNumBlockX, int nNumBlockY, float fSizeBlockX, float fSizeBlockY);
void UninitMeshWall(void);
//void UpdateMeshWall(void);
void DrawMeshWall(void);

//�ǂƂ̓����蔻��
bool HitCheckMeshWall( D3DXVECTOR3 tNowPos, D3DXVECTOR3 tNextPos, D3DXVECTOR3 *wall_nor, D3DXVECTOR3 *HitPoint);

#endif
