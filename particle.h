/*******************************************************************************
*
* �^�C�g��:		�G�̌�(�p�[�e�B�N��)����
* �v���O������:	particle.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

#define MAX_DOT	(64)

//�ꖇ�̃h�b�g�̍\����
typedef struct
{
	D3DXMATRIX mtxWorld;								// ���[���h�}�g���b�N�X

	D3DXVECTOR3 posDot;									// �ꖇ�h�b�g�̈ʒu
	D3DXVECTOR3 rotDot;									// �ꖇ�h�b�g�̌���(��])
	D3DXVECTOR3 sclDot;									// �ꖇ�h�b�g�̑傫��(�X�P�[��)
	D3DXVECTOR3 movDot;									// �ꖇ�h�b�g�̈ړ���

	float alpha;										//�A���t�@�l
	int nIdxShadow;										//�g�p���Ă���e�̔ԍ�
	float timer;										//�t���C���𐔂���
	bool bUse;
}PARTICLE_DOT;

//��̃p�[�e�B�N���̍\����
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pD3DVtxBuffParticle;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
	PARTICLE_DOT aPtcDot[MAX_DOT];						//�h�b�g��
	bool bDraw;											//�\�����邩
	int nNeedCreateNum;									//�������鐔
	int nNeedDeteNum;									//�����Ă�����
	int nPtcCreateRate;									//�h�b�g������
	int nPtcCreateCounter;								//�h�b�g�����̃N�[���_�E��
	D3DXVECTOR3 pos;									//�p�[�e�B�N���̈ʒu
}PARTICLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitParticle(void);
void UninitParticle(void);
void UpdateParticle( float fTimeSpeed);
void DrawParticle(void);

//�p�[�e�B�N���̐ݒu
void SetParticle(int needNum, D3DXVECTOR3 pos);

#endif
