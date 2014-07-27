#include "StdAfx.h"
#include "RevCamera.h"
#include "RevStructures.h"
#include "D3dx9math.h"
#include "CU_VectorMath.h"

RevCamera::RevCamera(void)
{
}

RevCamera::~RevCamera(void)
{
}

bool RevCamera::Init( const float& aFov, const float& aZnear, const float& aZFar, const float& aAspectRatio )
{
	myZFar	= aZFar;
	myZNear = aZnear;
	myFov	= aFov;
	myAspectRatio = aAspectRatio;

	D3DXMATRIX m;

	D3DXMatrixPerspectiveFovLH( 
		&m,
		aFov *(PI/180.0f),
		aAspectRatio,
		myZNear,
		myZFar);

	myProjMatrix.ConvertD3D( reinterpret_cast<float*>( m.m ));

	return true;
}

void RevCamera::SetPosition( const CU::Vector4f& aPosition )
{
	myViewMatrix.SetPosition( aPosition );
}

void RevCamera::SetRotation( const CU::Matrix33f& aRotation )
{
	myViewMatrix.SetRotation( aRotation );
}


void RevCamera::PerformRotation( const CU::Matrix33f& aRotation )
{
	CU::Matrix33f rot = aRotation;
	rot = rot * myViewMatrix.Get33();
	myViewMatrix.SetRotation(rot );
}

RevCameraData RevCamera::GetCameraData( )
{
	RevCameraData data;
	data.myViewMatrix		= myViewMatrix.Real4X4Inverse();
	data.myProjectionMatrix = myProjMatrix;
	data.myZFar				= myZFar;
	data.myZNear			= myZNear;

	float halfHeight =myZFar * tanf( 0.5f*(myFov * DirectX::XM_PI / 180.0f)  );
	float halfWidth  = myAspectRatio * halfHeight;
	float farZ = myZFar;

	data.myFrustumCorners[0] = CU::Vector4f(-halfWidth, -halfHeight, farZ, 1.0f);
	data.myFrustumCorners[1] = CU::Vector4f(-halfWidth, +halfHeight, farZ, 1.0f);
	data.myFrustumCorners[2] = CU::Vector4f(+halfWidth, +halfHeight, farZ, 1.0f);
	data.myFrustumCorners[3] = CU::Vector4f(+halfWidth, -halfHeight, farZ, 1.0f);
	return data;
}

CU::Vector3f RevCamera::GetLocation()
{
	return myViewMatrix.GetPosition3();
}

CU::Vector3f RevCamera::GetForwardVector()
{
	return CU::Vector3f(myViewMatrix.myMatrix[2][0], myViewMatrix.myMatrix[2][1], myViewMatrix.myMatrix[2][2]);
}

CU::Matrix33f RevCamera::GetRotation()
{
	return myViewMatrix.Get33();
}

void RevCamera::Forward(	const float& aMagnitude,
						const float& aDelta )
{
	CU::Vector3f pos = myViewMatrix.GetPosition();
	CU::Vector3f forward(0,0,1);
	forward = forward * myViewMatrix.Get33();

	pos = (pos + forward * (aMagnitude * aDelta));
	myViewMatrix.SetPosition( pos );
}

void RevCamera::Left(		const float& aMagnitude,
					 const float& aDelta )
{
	CU::Vector3f pos = myViewMatrix.GetPosition();
	CU::Vector3f left = CU::Vector3f(1,0,0);
	left = left * myViewMatrix.Get33();

	pos = (pos + left * (aMagnitude * aDelta));
	myViewMatrix.SetPosition( pos );
}

void RevCamera::Up(		const float& aMagnitude,
					 const float& aDelta )
{
	CU::Vector3f pos = myViewMatrix.GetPosition();
	CU::Vector3f up = CU::Vector3f(0,1,0);
	up = up * myViewMatrix.Get33();

	pos = (pos + up * (aMagnitude * aDelta));
	myViewMatrix.SetPosition( pos );
}

void RevCamera::ForwardNonRotated(		const float& aMagnitude,
					 const float& aDelta )
{
	CU::Vector3f pos = myViewMatrix.GetPosition();
	CU::Vector3f forwardNonRotated = CU::Vector3f(0,0,1);
	pos = (pos + forwardNonRotated * (aMagnitude * aDelta));
	myViewMatrix.SetPosition( pos );
}
void RevCamera::RotateZ(	
	const float& aMagnitude,
	const float& aDelta )
{
	PerformRotation( CU::Matrix33f::RotateAroundZPI( aMagnitude * aDelta ) );
}

void RevCamera::RotateY(	const float& aMagnitude,
						const float& aDelta )
{
	PerformRotation( CU::Matrix33f::RotateAroundYPI( aMagnitude * aDelta ) );
}


void RevCamera::RotateX(	const float& aMagnitude,
						const float& aDelta )
{
	CU::Matrix33f rot =  CU::Matrix33f::RotateAroundXPI( aMagnitude * aDelta );
	PerformRotation( rot );

}