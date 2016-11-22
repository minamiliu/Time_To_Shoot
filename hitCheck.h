/*******************************************************************************
*
* �^�C�g��:		�����蔻��̏���
* �v���O������:	hitCheck.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#ifndef _HITCHECK_H_
#define _HITCHECH_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

// ����
float DotProduct( D3DXVECTOR3 vl, D3DXVECTOR3 vr);

// �O��
D3DXVECTOR3 CrossProduct( D3DXVECTOR3 vl, D3DXVECTOR3 vr );

// �|���S���Ɛ����Ƃ̓����蔻��
bool HitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1);

// �ǂ���x�N�g��
D3DXVECTOR3* GetWallScratchVector(D3DXVECTOR3 *out, D3DXVECTOR3 &front, D3DXVECTOR3 &normal);

// ��_���擾
D3DXVECTOR3 GetHitPos(void);

// �ۂ̓����蔻��
bool HitCheckBall( D3DXVECTOR3 pos1, float radius1, D3DXVECTOR3 pos2, float radius2);

// ���ʂ̉~�̓����蔻��
bool HitCheckXZ( D3DXVECTOR3 pos1, float radius1, D3DXVECTOR3 pos2, float radius2);

// �Q�_�̌����x�N�g�����擾
D3DXVECTOR3 GetRotationAngle( D3DXVECTOR3 myPos, D3DXVECTOR3 target, D3DXVECTOR3 myAngle);

// ��]�p�x���擾
D3DXVECTOR3 Get2VecRotAngle( D3DXVECTOR3 rot, D3DXVECTOR3 rotTarget, float divide, float value_rot);

// ������̔���
bool HitCheckViewArea( D3DXVECTOR3 myPos, D3DXVECTOR3 tarPos, D3DXVECTOR3 myRot, float myView);

// �Q�_�������擾

float GetDistanceXZ( D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);
#endif
