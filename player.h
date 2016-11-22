/*******************************************************************************
*
* �^�C�g��:		�v���C���[����
* �v���O������:	player.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_PLAYER (1)
#define INI_NUM_LIFE (3)
#define INI_NUM_BULLET (3)

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
	D3DXVECTOR3 posLSide;				// ���f���̍����̈ʒu
	D3DXVECTOR3 posRSide;				// ���f���̉E���̈ʒu
	D3DXVECTOR3 rot;					// ���f���̌���(��])
	D3DXVECTOR3 scl;					// ���f���̑傫��(�X�P�[��)
	D3DXVECTOR3 move;					// ���f���̈ړ���
	D3DXVECTOR3 rotAngle;				// ���f���̌���(��])
	D3DXVECTOR3 rotTarget;				// ���f���̌���(��])

	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X

	int nIdxShadow;						//�g�p���Ă���e�̔ԍ�
	
	//jump
	bool inAir;							//�W�����v����
	int jumpTime;						//�W�����v���̃t���C����

	//status
	bool bAlive;						//�����Ă��邩
	int nBullet;						//�e�̐�
	int nLife;							//���C�t�̐�
	bool bRun;							//�����Ă��邩
} PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

PLAYER *GetPlayer(void);
//�v���C���[�̎��S����
void SetPlayerDeath(void);
//�v���C���[�̐ݒu
void SetPlayer( D3DXVECTOR3 pos, float rotY, int numBullet, int numLife);
#endif
