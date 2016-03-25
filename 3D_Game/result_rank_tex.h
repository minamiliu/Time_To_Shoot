/*******************************************************************************
*
* �^�C�g��:		�e�N�X�`�� ���U���g�̃����L���O�̃X�R�[�A�\������
* �v���O������:	result_BG.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _RESULT_RANK_TEX_H_
#define _RESULT_RANK_TEX_H_

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
	D3DXVECTOR3 pos;
	D3DXVECTOR3 size;
	D3DCOLOR col;
	int score;

}RESULT_RANK_TEX;

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT InitResultRankTex();
void DrawResultRankTex(void);		
void UninitResultRankTex(void);
void UpdateResultRankTex(void);

//�X�R�[�A�ύX
void ChangeResultRankTex(int nRank, int value, D3DCOLOR col);
#endif