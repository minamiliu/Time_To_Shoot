/*******************************************************************************
*
* �^�C�g��:		�A�C�e��(�e)�̏���
* �v���O������:	itemBullet.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

#ifndef _ITEM_BULLET_H_
#define _ITEM_BULLET_H_

#include "main.h"

#define MAX_ITEM_BULLET (3)

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


	D3DXMATRIX mtxWorld;				// ���[���h�}�g���b�N�X

	int nIdxShadow;						//�g�p���Ă���e�̔ԍ�
	bool bUse;
}ITEM_BULLET;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitItemBullet(void);
void UninitItemBullet(void);
void UpdateItemBullet(void);
void DrawItemBullet(void);

//�A�C�e���e�̐ݒu
void SetItemBullet( D3DXVECTOR3 pos);
#endif
