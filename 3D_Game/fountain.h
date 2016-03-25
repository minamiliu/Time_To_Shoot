/*******************************************************************************
*
* �^�C�g��:		�����̐��p�[�e�B�N������
* �v���O������:	fountain.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#ifndef _FOUNTAIN_H_
#define _FOUNTAIN_H_

#include "main.h"

typedef struct
{
	D3DXMATRIX mtxWorld;							// ���[���h�}�g���b�N�X

	D3DXVECTOR3 pos;							// �n�ʂ̈ʒu
	D3DXVECTOR3 rot;							// �n�ʂ̌���(��])
	D3DXVECTOR3 scl;							// �n�ʂ̑傫��(�X�P�[��)

	D3DXVECTOR3 mov;
	float alpha;
	int nIdxShadow;
	float volume;
	float timer;
	bool bUse;
}FOUNTAIN;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFountain(void);
void UninitFountain(void);
void UpdateFountain(float fTimeSpeed);
void DrawFountain(void);

void SetFountain(int timer, D3DXVECTOR3 pos);

#endif
