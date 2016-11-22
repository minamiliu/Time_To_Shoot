/*******************************************************************************
*
* �^�C�g��:		�G�̏���
* �v���O������:	enemy.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "stageManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ENEMY	(30)							// �ő吔

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************

//�G�̎��
typedef enum
{
	ENEMY_WHITE,
	ENEMY_BLUE,
	ENEMY_RED,
	ENEMY_TYPE_MAX
}ENEMY_TYPE;

//���f�����
typedef struct
{
	LPDIRECT3DTEXTURE9	pD3DTexture;	// �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH  pD3DXMesh;				// ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER  pD3DXBuffMat;			// �}�e���A�����ւ̃|�C���^
	DWORD	nNumMat;					// �}�e���A�����̐�
	char filePath[MAX_PATH];
} ENEMY_MODEL;

//�G�\����
typedef struct
{
	D3DXVECTOR3 pos;					// ���f���̈ʒu
	D3DXVECTOR3 posLSide;
	D3DXVECTOR3 posRSide;
	D3DXVECTOR3 rot;					// ���f���̌���(��])
	D3DXVECTOR3 scl;					// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3 move;					// ���f���̈ړ���
	D3DXVECTOR3 rotAngle;				// ���f���̌���(��])
	D3DXVECTOR3 rotTarget;				// ���f���̌���(��])

	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X

	int nIdxShadow;						//�g�p���Ă���e�̔ԍ�
	bool bUse;							
	bool bShot;							//�e����������
	bool bEscaping;						//��������
	float nCoolDown;					//�e�̃N�[���_�E��
	ENEMY_TYPE type;					//�G�̎��

	//see
	bool bSeePlayer;					//�v���C���[�𔭌�
	int nSeePFrame;						//�v���C���[�𔭌������t���C����

	//Patrol
	bool bPatrol;						//�p�g���[����
	D3DXVECTOR3 posPatrol;				//�p�g���[�����̍��W
	D3DXVECTOR3 rotPatrol;				//�p�g���[�����̌���
} ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(float fTimeSpeed);
void DrawEnemy(void);

ENEMY *GetEnemy(void);
//�G�̐ݒu
void SetEnemy( D3DXVECTOR3 pos, float rotY, ENEMY_TYPE type);
#endif
