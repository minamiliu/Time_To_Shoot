/*******************************************************************************
*
* �^�C�g��:		�e�̃��f������
* �v���O������:	gun.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _GUN_H_
#define _GUN_H_

#include "main.h"

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

	//�L�[�t���C��
	D3DXVECTOR3 posChild;				//�q���W
	D3DXVECTOR3 posChildTarget;			//�q���W�̖ڕW
	D3DXVECTOR3 rotChild;				//�q���W�̌���
	D3DXVECTOR3 rotChildAngle;			//�q���W�̖���̉�]��
	D3DXVECTOR3 rotChildTarget;			//�q���W�̌����̖ڕW

	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X

	bool bShot;							//�e����������
	bool bShake;						//�ړ����Ă��邩
	bool bBulletOut;					//�e�؂�ɂȂ�����
	int nShootFrame;					//�e�����A�j���K�v�ȃt���C����
	int nCheckFrame;					//�e�؂�̃`�F�b�N�t���C����
	int nKeyFrame;						//�e�؂�̃L�[�t���C����
} GUN;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitGun(void);
void UninitGun(void);
void UpdateGun(void);
void DrawGun(void);

GUN *GetGun(void);
void SetGunShootAni(void);					//�e�����A�j���ݒu
void SetGunShakeAni(bool bShake);			//�ړ��̃A�j���ݒu
void SetGunBulletOutAni(bool bBulletOut);	//�e�؂�̃A�j���ݒu
bool IsGunReady(void);						//�e�̃A�j���I�������
#endif
