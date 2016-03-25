/*******************************************************************************
*
* �^�C�g��:		�J��������
* �v���O������:	camera.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
********************************************************************************

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "camera.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	CAM_POS_V_X				(0.0f)					// �J�����̎��_�����ʒu(X���W)
#define	CAM_POS_V_Y				(100.0f)				// �J�����̎��_�����ʒu(Y���W)
#define	CAM_POS_V_Z				(-100.0f)				// �J�����̎��_�����ʒu(Z���W)
#define	CAM_POS_R_X				(0.0f)					// �J�����̒����_�����ʒu(X���W)
#define	CAM_POS_R_Y				(0.0f)					// �J�����̒����_�����ʒu(Y���W)
#define	CAM_POS_R_Z				(0.0f)					// �J�����̒����_�����ʒu(Z���W)
#define	VIEW_ANGLE				(D3DXToRadian(60.0f))							// �r���[���ʂ̎���p
#define	VIEW_ASPECT				((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z				(1.f)											// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z				(2500.0f)										// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA		(2.0f)											// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA		(D3DX_PI * 0.01f)								// �J�����̉�]��
#define	VALUE_ADJUST_DISTANCE	(2.0f)											// ���_�ƒ����_�̋���������

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA g_camera;		//�J�������
bool g_cameraMode;		//TPS��FPS���[�h�̐؂�ւ�

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	float fVecX,fVecZ;

	// ���_������
	g_camera.posV = D3DXVECTOR3( CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
	// �����_������
	g_camera.posR = D3DXVECTOR3( CAM_POS_R_X, CAM_POS_R_Y, CAM_POS_R_Z);
	// ������x�N�g��������
	g_camera.vecU = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
	// �J���������i��]�p�j�̏�����
	g_camera.rot = D3DXVECTOR3( (D3DXToRadian(22.0f)), 0.0f, 0.0f);
	// ���_�ƒ����_�̋��������߂�
	fVecX = g_camera.posV.x - g_camera.posR.x;						// X�����i���_�[�����_�j
	fVecZ = g_camera.posV.z - g_camera.posR.z;						// Z�����i���_�[�����_�j
	// ���_�ƒ����_�̋���
	g_camera.fDistance = sqrtf(fVecX * fVecX + fVecZ * fVecZ);

	g_cameraMode = 0;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	{//�J�����Ǐ]
		PLAYER *pPlayer = GetPlayer();
		
		//�����_
		g_camera.rot.y += GetMouseAxisX() * 0.0025f;
		g_camera.rot.x += GetMouseAxisY() * 0.0025f;

		if( GetMouseAxisX() != 0 || GetMouseAxisY() != 0)
		{
			ChangeTimeSpeed( 0.5f);
		}

		g_camera.rot.y += GetGamePadRightAxisX(0) * 0.0001f;
		g_camera.rot.x += GetGamePadRightAxisY(0) * 0.0001f;

		if( GetGamePadRightAxisX(0) != 0 || GetGamePadRightAxisY(0) != 0)
		{
			ChangeTimeSpeed( 0.5f);
		}

		//�����_�p�x�C��
		if(g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y = g_camera.rot.y - D3DX_PI*2;
		}
		else if(g_camera.rot.y < 0.0f - D3DX_PI)
		{
			g_camera.rot.y = g_camera.rot.y + D3DX_PI*2;
		}
		if(g_camera.rot.x >= 1.0f)
		{
			g_camera.rot.x = 1.0f;
		}
		else if(g_camera.rot.x <= -1.0f)
		{
			g_camera.rot.x = -1.0f;
		}

		//���_
		switch( g_cameraMode)
		{
		case 0:
			g_camera.posV = pPlayer->pos;
			break;
		case 1:
			g_camera.posV = pPlayer->pos;
			g_camera.posV.x += 150 * sinf(g_camera.rot.y+D3DXToRadian(180.0f));
			g_camera.posV.z += 150 * cosf(g_camera.rot.y+D3DXToRadian(180.0f));
			break;
		}

		//����
		g_camera.posV.y += 80;

		//����
		g_camera.posR.x = g_camera.posV.x + g_camera.fDistance * sinf(g_camera.rot.y);
		g_camera.posR.z = g_camera.posV.z + g_camera.fDistance * cosf(g_camera.rot.y);
		//����
		g_camera.posR.y = g_camera.posV.y - g_camera.fDistance * sinf(g_camera.rot.x);
	}

	if(GetKeyboardTrigger(DIK_F1) && IsDebugMode())
	{
		g_cameraMode = 1 - g_cameraMode;
	}
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice(); 

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);
	
	
	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH( &g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);
	
	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform( D3DTS_VIEW, &g_camera.mtxView);
	
	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_camera.mtxProjection);
	
	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH( &g_camera.mtxProjection, VIEW_ANGLE, VIEW_ASPECT, VIEW_NEAR_Z, VIEW_FAR_Z);
	
	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�(�����ϊ��̐ݒ�)
	pDevice->SetTransform( D3DTS_PROJECTION, &g_camera.mtxProjection);
}

//=============================================================================
// �J�����̎擾
//=============================================================================
CAMERA *GetCamera(void)
{
	return &g_camera;
}

