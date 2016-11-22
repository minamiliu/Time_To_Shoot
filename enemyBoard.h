/*******************************************************************************
* �^�C�g��:		�t�B�[���h����
* �v���O������:	field.cpp
* �쐬��:		TH�������@���
* �쐬��:		2015/10/20
********************************************************************************
* �X�V����:		- 2015/10/20	 ���
*					- V1.00		Initial Version
*******************************************************************************/
/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#ifndef _ENEMYBOARD_H_
#define _ENEMYBOARD_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
#define MAX_ENEMYBOARD	(32)

typedef struct
{
	D3DXMATRIX mtxWorld;												// ���[���h�}�g���b�N�X

	D3DXVECTOR3 pos;													// �n�ʂ̈ʒu
	D3DXVECTOR3 rot;													// �n�ʂ̌���(��])
	D3DXVECTOR3 scl;													// �n�ʂ̑傫��(�X�P�[��)
	bool bUse;
}ENEMYBOARD;

HRESULT InitEnemyBoard(void);
void UninitEnemyBoard(void);
void UpdateEnemyBoard(void);
void DrawEnemyBoard(void);
bool HitCheckEnemyBoard( D3DXVECTOR3 tNowPos, D3DXVECTOR3 tNextPos, int nIdxBoard);

void SetVertexEnemyBoard(int nIdxBoard, float fSizeX, float fSizeZ);
int CreateEnemyBoard(D3DXVECTOR3 pos, float fSizeX, float fSizeZ);
void ReleaseEnemyBoard(int nIdxBoard);
void SetPositionEnemyBoard(int nIdxBoard, D3DXVECTOR3 pos);

#endif
