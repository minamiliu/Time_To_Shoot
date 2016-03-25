/*******************************************************************************
*
* �^�C�g��:		�G�̒e�̏���
* �v���O������:	enemyBullet.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

#ifndef _ENEMY_BULLET_H_
#define _ENEMY_BULLET_H_

#include "main.h"

#define MAX_ENEMY_BULLET	(16)

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9	pD3DTexture;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH  pD3DXMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER  pD3DXBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD	nNumMat;					// �}�e���A�����̐�

	D3DXVECTOR3 pos;					// ���f���̈ʒu
	D3DXVECTOR3 rot;					// ���f���̌���(��])
	D3DXVECTOR3 scl;					// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3 move;					// ���f���̈ړ���
	D3DXVECTOR3 rotAngle;				// ���f���̌���(��])
	D3DXVECTOR3 rotTarget;				// ���f���̌���(��])

	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X

	int nIdxShadow;						//�g�p���Ă���e�̔ԍ�
	bool bUse;
	D3DXVECTOR3 front;					//�e�̌���
	int nJustShoot;						//�������΂���̒e�̈ړ��t���C��
} ENEMY_BULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemyBullet(void);
void UninitEnemyBullet(void);
void UpdateEnemyBullet( float fTimeSpeed);
void DrawEnemyBullet(void);

//�e�̐ݒu
void SetEnemyBullet( D3DXVECTOR3 pos, D3DXVECTOR3 target);
#endif
