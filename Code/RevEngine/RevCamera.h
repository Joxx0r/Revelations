#ifndef _REV_CAMERA_H
#define _REV_CAMERA_H

#include "CU_Matrix.h"

struct RevCameraData;

class RevCamera
{
public:
	RevCamera(void);
	~RevCamera(void);
	bool Init( const float& aFov, const float& aZnear, const float& aZFar, const float& aAspectRatio );

	void SetPosition( const CU::Vector4f& aPosition );
	void SetRotation( const CU::Matrix33f& aRotation );
	void PerformRotation( const CU::Matrix33f& aRotation );

	RevCameraData GetCameraData( );

	CU::Vector3f GetLocation();
	CU::Vector3f GetForwardVector();
	CU::Matrix33f GetRotation();

	void Forward(	const float& aMagnitude,
		const float& aDelta );
	void Left(		const float& aMagnitude,
		const float& aDelta );
	void RotateX(	const float& aMagnitude,
		const float& aDelta );

	void RotateY(	const float& aMagnitude,
		const float& aDelta );
	void RotateZ(	const float& aMagnitude,
		const float& aDelta );
	void Up(		const float& aMagnitude,
					 const float& aDelta );
	void ForwardNonRotated(		const float& aMagnitude,
					 const float& aDelta );

private:

	CU::Matrix44f myViewMatrix; 
	CU::Matrix44f myProjMatrix;

	float			myZFar;
	float			myZNear;
	float			myFov;
	float			myAspectRatio;

};

#endif