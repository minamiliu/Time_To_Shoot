/*******************************************************************************
*
* �^�C�g��:		�����̃��f���̕\������
* �v���O������:	fountainCircle.h
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


#ifndef _FOUNTAIN_CIRCLE_H_
#define _FOUNTAIN_CIRCLE_H_

#include "main.h"

#define MAX_D3D_MAT (5)

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	LPDIRECT3DTEXTURE9	pD3DTexture[MAX_D3D_MAT];	// �e�N�X�`���ւ̃|�C���^
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

	float fRadius;						//���a
	int nIdxShadow;						//�g�p���Ă���e�̔ԍ�
	bool bUse;
}FOUNTAIN_CIRCLE;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitFountainCircle(void);
void UninitFountainCircle(void);
void UpdateFountainCircle(void);
void DrawFountainCircle(void);

FOUNTAIN_CIRCLE *GetFountainCircle(void);

#endif
