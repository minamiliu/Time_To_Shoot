/*******************************************************************************
*
* �^�C�g��:		�e�̃��f������
* �v���O������:	gun.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "gun.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"
#include "hitCheck.h"


//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_VERTEX (4)
#define NUM_POLYGON (2)

#define MODEL_PLANE		"data/MODEL/gun.x"					// �ǂݍ��ރe�N�X�`���t�@�C����

#define VALUE_MOVE		(5.0f)									// �ړ���
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 						// ��]��
#define DIVIDE_ROTATE	(30) 									// ��]�ʂ������ɕ�����

#define MODEL_POS_X	(0)
#define MODEL_POS_Y	(30)
#define MODEL_POS_Z	(-50)

#define MAX_KEYFRAME	(4)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GUN g_gun;


D3DXVECTOR3 g_BulletOutAnime[MAX_KEYFRAME]={ 
	D3DXVECTOR3( D3DXToRadian(-60.0f), D3DXToRadian(-30.0f), D3DXToRadian(-140.0f)), 
	D3DXVECTOR3( D3DXToRadian(-60.0f), D3DXToRadian(0.0f), D3DXToRadian(-140.0f)), 
	D3DXVECTOR3( D3DXToRadian(-60.0f), D3DXToRadian(-60.0f), D3DXToRadian(-140.0f)), 
	D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// �ʒu�E��]�E�X�P�[���̏����ݒ�
	g_gun.pos = D3DXVECTOR3( MODEL_POS_X, MODEL_POS_Y, MODEL_POS_Z);
	g_gun.rot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_gun.scl = D3DXVECTOR3( 2.0f, 2.0f, 2.0f);
	//g_gun.move = D3DXVECTOR3( VALUE_MOVE, VALUE_MOVE, 0.0f);
	g_gun.bShot = false;
	g_gun.bShake = false;
	g_gun.bBulletOut = false;

	// ���f���Ɋւ���ϐ��̏�����							
	g_gun.pD3DTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	g_gun.pD3DXMesh = NULL;		// ���b�V�����ւ̃|�C���^
	g_gun.pD3DXBuffMat = NULL;		// �}�e���A�����ւ̃|�C���^
	g_gun.nNumMat = 0;				// �}�e���A�����̐�

	// X�t�@�C���̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(
		MODEL_PLANE,				// �ǂݍ��ރ��f���t�@�C����(X�t�@�C��)
		D3DXMESH_SYSTEMMEM,			// ���b�V���̍쐬�I�v�V�������w��
		pDevice,					// IDirect3DDevice9�C���^�[�t�F�C�X�ւ̃|�C���^
		NULL,						// �אڐ��f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		&g_gun.pD3DXBuffMat,		// �}�e���A���f�[�^���܂ރo�b�t�@�ւ̃|�C���^
		NULL,						// �G�t�F�N�g�C���X�^���X�̔z����܂ރo�b�t�@�ւ̃|�C���^
		&g_gun.nNumMat,			// D3DXMATERIAL�\���̂̐�
		&g_gun.pD3DXMesh			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X
		)))
	{
		return E_FAIL;
	}		

														
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitGun(void)
{
	// �e�N�X�`���̊J��

	// ���b�V���̊J��
	if(g_gun.pD3DXMesh != NULL)
	{
		g_gun.pD3DXMesh->Release();
		g_gun.pD3DXMesh = NULL;
	}
	// �}�e���A���̊J��
	if(g_gun.pD3DXBuffMat != NULL)
	{
		g_gun.pD3DXBuffMat->Release();
		g_gun.pD3DXBuffMat = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateGun(void)
{
	CAMERA *pCamera = GetCamera();

	//�e�̓J�����̉E���̉��x�̂Ƃ���ɒu��
	float angle = pCamera->rot.y + D3DXToRadian(15.0f);
	if( angle > D3DX_PI) angle -= D3DX_PI * 2.0f;

	//�e�̈ʒu��ݒ�
	g_gun.pos.x = pCamera->posV.x + 2 * sinf(angle);
	g_gun.pos.z = pCamera->posV.z + 2 * cosf(angle);
	g_gun.pos.y = pCamera->posV.y - 1.68f;

	//�����낷���A�e�̈ʒu
	g_gun.pos.y += sinf( D3DXToRadian(22.0f) - pCamera->rot.x ) * 2.0f ;
	if( D3DXToRadian(22.0f) - pCamera->rot.x < 0)
	{
		g_gun.pos.x = pCamera->posV.x + ( 2 +  (D3DXToRadian(22.0f) - pCamera->rot.x)*0.5f ) * sinf(angle);
		g_gun.pos.z = pCamera->posV.z + ( 2 +  (D3DXToRadian(22.0f) - pCamera->rot.x)*0.5f ) * cosf(angle);				
	}

	//�e�̌���
	g_gun.rot = GetRotationAngle( g_gun.pos, pCamera->posR, g_gun.rot);	

	//����
	if( g_gun.bShot)
	{
		g_gun.nShootFrame--;
		if( g_gun.nShootFrame == 0) //�ꔭ�̔��ˊ��� 
		{
			g_gun.bShot = false;
		}
		else if( g_gun.nShootFrame > 50) //���˂����u�ԁA�e�̔���
		{
			float angle = pCamera->rot.y + D3DXToRadian(170.0f); 
			if( angle > D3DX_PI) angle -= D3DX_PI * 2.0f;
			g_gun.posChild.x = 0.5f * sinf(angle);
			g_gun.posChild.z = 0.5f * cosf(angle);
		}
		else //�e�����Ɉʒu�ɖ߂�
		{
			g_gun.posChild.x -= g_gun.posChild.x * 0.05f;
			g_gun.posChild.z -= g_gun.posChild.z * 0.05f;
		}
		
		//�e�q�̍��W�𑫂��āA�e�̈ʒu��ݒ�
		g_gun.pos.x += g_gun.posChild.x;
		g_gun.pos.z += g_gun.posChild.z;	
	}

	//�h���
	const float shakeMove = 0.01f;
	if( g_gun.bShake)
	{

		float diff = g_gun.posChildTarget.y - g_gun.posChild.y;
		if( fabs(diff) <= shakeMove)
		{
			//�㉺���]
			g_gun.posChildTarget.y *= -1;
		}
		else if( diff > 0.0f)
		{
			g_gun.posChild.y += shakeMove;
		}
		else
		{
			g_gun.posChild.y -= shakeMove;
		}
		
		
		//�e�q�̍��W�𑫂��āA�e�̈ʒu��ݒ�
		g_gun.pos += g_gun.posChild;		
	}

	//�e�؂�
	if( g_gun.bBulletOut)
	{
		if(g_gun.nCheckFrame > 0){
			g_gun.nCheckFrame--;
		}
		else
		{
			g_gun.nCheckFrame = -1;

			//���̉�]�ʒu�ɓ���������
			if( fabs(g_gun.rotChild.x - g_gun.rotChildTarget.x) < VALUE_ROTATE &&
				fabs(g_gun.rotChild.y - g_gun.rotChildTarget.y) < VALUE_ROTATE &&
				fabs(g_gun.rotChild.z - g_gun.rotChildTarget.z) < VALUE_ROTATE
				)
			{

				g_gun.nKeyFrame++;
				if( g_gun.nKeyFrame < MAX_KEYFRAME)
				{
					g_gun.rotChildAngle =  Get2VecRotAngle( g_gun.rotChildTarget, g_BulletOutAnime[g_gun.nKeyFrame], DIVIDE_ROTATE, -1);
					g_gun.rotChildTarget = g_BulletOutAnime[g_gun.nKeyFrame];	
				} 
			}
			else
			{
				g_gun.rotChild += g_gun.rotChildAngle;
				g_gun.rotChildAngle += (-g_gun.rotChildAngle) * 0.001f;
			}
			
		
		}
		//�e�q�̍��W�𑫂��āA�e�̈ʒu��ݒ�
		g_gun.rot += g_gun.rotChild;

		if( g_gun.nKeyFrame == MAX_KEYFRAME)
		{
			g_gun.bBulletOut = false;
		}
	}

	//PrintDebugProc("\nCamera�̈ʒu�F(X:%f, Y:%f, Z:%f)\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);
	//PrintDebugProc("\n�e�̈ʒu�F(X:%f, Y:%f, Z:%f)\n", g_gun.pos.x, g_gun.pos.y, g_gun.pos.z);
	//PrintDebugProc("�e�̌����@�F(X:%f, Y:%f, Z:%f)\n", g_gun.rot.x, g_gun.rot.y, g_gun.rot.z);
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawGun(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;
	
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity( &g_gun.mtxWorld);
	
	// �X�P�[���𔽉f
	D3DXMatrixScaling( &mtxScl, g_gun.scl.x, g_gun.scl.y, g_gun.scl.z);
	D3DXMatrixMultiply( &g_gun.mtxWorld, &g_gun.mtxWorld, &mtxScl);
	
	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll( &mtxRot, g_gun.rot.y, g_gun.rot.x, g_gun.rot.z);
	D3DXMatrixMultiply( &g_gun.mtxWorld, &g_gun.mtxWorld, &mtxRot);
	
	// �ړ��𔽉f
	D3DXMatrixTranslation( &mtxTranslate, g_gun.pos.x, g_gun.pos.y, g_gun.pos.z);
	D3DXMatrixMultiply( &g_gun.mtxWorld, &g_gun.mtxWorld, &mtxTranslate);
	
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform( D3DTS_WORLD, &g_gun.mtxWorld);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);
	
	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL *)g_gun.pD3DXBuffMat->GetBufferPointer();
	for(int nCntMat = 0; nCntMat < (int)g_gun.nNumMat; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial( &pD3DXMat[nCntMat].MatD3D);
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture( 0, g_gun.pD3DTexture);
		// �`��
		g_gun.pD3DXMesh->DrawSubset( nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial( &matDef);
}

//=============================================================================
// ���f���̎擾
//=============================================================================
GUN *GetGun(void)
{
	return &g_gun;
}

/*******************************************************************************
* �e�̔����A�j��
*******************************************************************************/
void SetGunShootAni(void)
{
	g_gun.bShot = true;
	g_gun.nShootFrame = 60;
	g_gun.posChild = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
}
/*******************************************************************************
* �e�̈ړ��A�j��
*******************************************************************************/
void SetGunShakeAni(bool bShake)
{
	const float shakeMax = 0.05f;
	g_gun.bShake = bShake;
	g_gun.posChild = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	//�h���Y�͈͂�ݒ�
	g_gun.posChildTarget = D3DXVECTOR3( 0.0f, shakeMax, 0.0f);
}
/*******************************************************************************
* �e�؂�̃A�j��
*******************************************************************************/
void SetGunBulletOutAni(bool bBulletOut)
{
	g_gun.bBulletOut = bBulletOut;
	g_gun.rotChild = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	g_gun.nCheckFrame = 30;

	//�e�؂�̎��A�e�̊p�x
	g_gun.nKeyFrame = 0;
	g_gun.rotChildTarget = g_BulletOutAnime[0];


	//���v���A�܂��͋t���v�������߂�
	g_gun.rotChildAngle =  Get2VecRotAngle( g_gun.rot, g_gun.rotChildTarget + g_gun.rot, DIVIDE_ROTATE, -1);
	g_gun.rotChild += g_gun.rotChildAngle;
}

/*******************************************************************************
* �e�̔��ˏ����͂ǂ���
*******************************************************************************/
bool IsGunReady(void)
{
	if(g_gun.bBulletOut || g_gun.bShot)
	{
		return false;
	}
	return true;
}
