/*******************************************************************************
*
* �^�C�g��:		�����蔻��̏���
* �v���O������:	gunSight.cpp
* �쐬��:		HAL�����Q�[���w�ȁ@����G
*
*******************************************************************************/


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "hitCheck.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define		NO_USE_D3DX	// �錾���R�����g�ɂ����D3DXVec3Dot�����g�p����
#define VALUE_ROTATE	(D3DX_PI * 0.02f) 						// ��]��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3 g_hitPos;

//=============================================================================
// ����
//=============================================================================
float DotProduct( D3DXVECTOR3 vl, D3DXVECTOR3 vr)
{
	float ans;

#ifdef NO_USE_D3DX
	// �_�C���N�g�w�ł́A�A�A
	ans = (vl.x * vr.x) + (vl.y * vr.y) + (vl.z * vr.z);

#else


#endif

	return ans;

}


//=============================================================================
// �O��
//=============================================================================
D3DXVECTOR3 CrossProduct( D3DXVECTOR3 vl, D3DXVECTOR3 vr )
{
	D3DXVECTOR3 ret;

#ifdef NO_USE_D3DX
	// �_�C���N�g�w�ł́A�A�A
	ret.x = (vl.y * vr.z) - (vl.z*vr.y);
	ret.y = (vl.z * vr.x) - (vl.x*vr.z);
	ret.z = (vl.x * vr.y) - (vl.y*vr.x);
#else

#endif

	return ret;
}


//=============================================================================
// �|���S���Ɛ����Ƃ̓����蔻��
// p0  :�|���S���̒��_1
// p1  :�|���S���̒��_2
// p2  :�|���S���̒��_3
// pos0:�n�_�i�ړ��O�j
// pos1:�I�_�i�ړ���j
// 
// �Ԃ�l�F0�͓������Ă��Ȃ�  1:�������Ă���
//=============================================================================
bool HitCheck(D3DXVECTOR3 p0, D3DXVECTOR3 p1, D3DXVECTOR3 p2, D3DXVECTOR3 pos0, D3DXVECTOR3 pos1)
{
	D3DXVECTOR3		nor;		// �|���S���̖@��

	{	// �|���S���̊O�ς��Ƃ��Ė@�������߂�
		D3DXVECTOR3 vec01 = p1 - p0;
		D3DXVECTOR3 vec02 = p2 - p0;
		nor = CrossProduct( vec01, vec02);
		D3DXVec3Normalize( &nor, &nor);
	}

	{	// ���ʂƐ����̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		// ���߂��@���ƃx�N�g���Q�i�����̗��[�ƃ|���S����̔C�ӂ̓_�j�̓��ςƂ��ďՓ˂��Ă���\���𒲂ׂ�
		D3DXVECTOR3 vec1 = pos0 - p0;
		D3DXVECTOR3 vec2 = pos1 - p0;
		float a = DotProduct( vec1, nor);
		float b = DotProduct( vec2, nor);
		if( (a * b) > 0 ) return false;
	}

	{	// �|���S���Ɛ����̌�_�����߂�
		D3DXVECTOR3 vec1 = pos0 - p0;
		D3DXVECTOR3 vec2 = pos1 - p0;
		float	d1 = fabs( DotProduct( nor, vec1));	// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
		float	d2 = fabs( DotProduct( nor, vec2));	// ����������߂�ׂ̓_�ƃ|���S���Ƃ̋���
		float	a = d1 / (d1+d2);					// ������
		
		
		// pos0����pos1�ւ̃x�N�g���i�ړ��x�N�g���j
		D3DXVECTOR3 vec3 = pos1 -pos0; 

		// ��_
		D3DXVECTOR3 p3 = pos0 + vec3 * a;

		// ��_��n��
		g_hitPos = p3;

		{	// ���߂���_���|���S���̒��ɂ��邩���ׂ�
			
			// �|���S���̊e�ӂ̃x�N�g��
			D3DXVECTOR3 v1 = p1 - p0;
			D3DXVECTOR3 v2 = p2 - p1;
			D3DXVECTOR3 v3 = p0 - p2;

			// �e���_�ƌ�_�Ƃ̃x�N�g��
			D3DXVECTOR3 v4 = p3 - p1;
			D3DXVECTOR3 v5 = p3 - p2;
			D3DXVECTOR3 v6 = p3 - p0;

			// �e�ӂƂ̊O�ςŖ@�������߂�
			D3DXVECTOR3 n1 = CrossProduct( v1, v4);
			D3DXVECTOR3 n2 = CrossProduct( v2, v5);
			D3DXVECTOR3 n3 = CrossProduct( v3, v6);

			// ���ꂼ��̃x�N�g���̌�������ςŃ`�F�b�N
			if( DotProduct( n1, n2) < -0.02f ) return false;
			if( DotProduct( n1, n3) < -0.02f ) return false;
		}
	}

	return true;
}


//=============================================================================
// �ǂ���x�N�g��
//
// out : ���K���ǂ���x�N�g���i�߂�l�j
// front : �i�s�x�N�g��
// normal: �Փ˓_�ł̖@���x�N�g��
//
//=============================================================================
D3DXVECTOR3* GetWallScratchVector(D3DXVECTOR3 *out, D3DXVECTOR3 &front, D3DXVECTOR3 &normal)
{
    D3DXVECTOR3 tNormal;
    D3DXVec3Normalize(&tNormal, &normal);

	*out = (front - DotProduct( front, tNormal) * tNormal);
	return out;
}

//*****************************************************************************
// ��_���擾
//*****************************************************************************
D3DXVECTOR3 GetHitPos(void)
{
	return g_hitPos;
}

//*****************************************************************************
// �ۂ̓����蔻��
//*****************************************************************************
bool HitCheckBall( D3DXVECTOR3 pos1, float radius1, D3DXVECTOR3 pos2, float radius2)
{

	//�o�E���f�B���O�X�t�B�A
	if( (pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y) + (pos1.z - pos2.z)*(pos1.z - pos2.z) 
		< (radius1 + radius2)*(radius1 + radius2) )
	{
		return true;
	}	
		
	return false;
}
//*****************************************************************************
// ���ʂ̉~�̓����蔻��
//*****************************************************************************
bool HitCheckXZ( D3DXVECTOR3 pos1, float radius1, D3DXVECTOR3 pos2, float radius2)
{

	//�o�E���f�B���O�X�t�B�A
	if( (pos1.x - pos2.x)*(pos1.x - pos2.x)  + (pos1.z - pos2.z)*(pos1.z - pos2.z) 
		< (radius1 + radius2)*(radius1 + radius2) )
	{
		return true;
	}	
		
	return false;
}
//*****************************************************************************
// �Q�_�̌����x�N�g�����擾
//*****************************************************************************
D3DXVECTOR3 GetRotationAngle( D3DXVECTOR3 myPos, D3DXVECTOR3 target, D3DXVECTOR3 myAngle)
{
	// �����p�x
	D3DXVECTOR3 dir = target - myPos;
	// Y���p�x
	float yAngle = atan2( dir.x, dir.z );

	//180�x�ȏ㍷�������+-360�x���ċt��
	if( yAngle - myAngle.y > D3DX_PI ) {
		yAngle -= D3DX_PI * 2.0f;
	} else if( myAngle.y - yAngle > D3DX_PI ) {
		yAngle += D3DX_PI * 2.0f;
	}

	//Y���p�x�C��
	if(yAngle > D3DX_PI)
	{
		yAngle = yAngle - D3DX_PI*2;
	}
	else if(yAngle < 0.0f - D3DX_PI)
	{
		yAngle = yAngle + D3DX_PI*2;
	}

	// X���p�x
	float zxLen = sqrt( dir.x * dir.x + dir.z * dir.z );
	float xAngle = atan2( dir.y, zxLen );

	return D3DXVECTOR3( -xAngle, yAngle, 0.0f); 
}
//*****************************************************************************
// ��]�p�x���擾
//*****************************************************************************
D3DXVECTOR3 Get2VecRotAngle( D3DXVECTOR3 rot, D3DXVECTOR3 rotTarget, float divide, float value_rot)
{
	float tAngle[3];
	D3DXVECTOR3 re;

	tAngle[0] = rotTarget.x - rot.x;
	tAngle[1] = rotTarget.y - rot.y;
	tAngle[2] = rotTarget.z - rot.z;

	for(int cntXYZ = 0; cntXYZ < 3; cntXYZ++)
	{
		if(value_rot == -1)
		{
			if( tAngle[cntXYZ] > 0)
			{
				if(tAngle[cntXYZ] >= D3DX_PI)
				{
					//tAngle[cntXYZ] = -value_rot;
					tAngle[cntXYZ] = -fabs(tAngle[cntXYZ]/divide);
				}
				else
				{
					//tAngle[cntXYZ] = value_rot;
					tAngle[cntXYZ] = fabs(tAngle[cntXYZ]/divide);
				}
						
			}
			else
			{
				if(tAngle[cntXYZ] < -D3DX_PI)
				{
					//tAngle[cntXYZ] = value_rot;
					tAngle[cntXYZ] = fabs(tAngle[cntXYZ]/divide);
				}
				else
				{
					//tAngle[cntXYZ] = -value_rot;
					tAngle[cntXYZ] = -fabs(tAngle[cntXYZ]/divide);
				}
			}		
		}
		else
		{
			if( tAngle[cntXYZ] > 0)
			{
				if(tAngle[cntXYZ] >= D3DX_PI)
				{
					tAngle[cntXYZ] = -value_rot;
				}
				else
				{
					tAngle[cntXYZ] = value_rot;
				}
						
			}
			else
			{
				if(tAngle[cntXYZ] < -D3DX_PI)
				{
					tAngle[cntXYZ] = value_rot;
				}
				else
				{
					tAngle[cntXYZ] = -value_rot;
				}
			}		
		}
	}

	re.x = tAngle[0];
	re.y = tAngle[1];
	re.z = tAngle[2];

	return re;
}
//*****************************************************************************
// ������̔���
//*****************************************************************************
bool HitCheckViewArea( D3DXVECTOR3 myPos, D3DXVECTOR3 tarPos, D3DXVECTOR3 myRot, float myView)
{
	D3DXVECTOR3 rotAnlge = GetRotationAngle( myPos, tarPos, myRot); 
	float fDiff = rotAnlge.y - myRot.y;

	if( fDiff > D3DX_PI)
	{
		fDiff = D3DX_PI * 2 - fDiff;
	}
	else if( fDiff < 0.0f - D3DX_PI)
	{
		fDiff = D3DX_PI * 2 + fDiff;
	}

	if( fabs(fDiff) <= myView)
	{
		return true;
	}
	return false;
}

//*****************************************************************************
// �Q�_�������擾
//*****************************************************************************
float GetDistanceXZ( D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	return sqrtf( (pos1.x - pos2.x)*(pos1.x - pos2.x)  + (pos1.z - pos2.z)*(pos1.z - pos2.z) );
}
