#include "stdafx.h"
#include "RevBone.h"
#include <DirectXMath.h>
#include <D3dx9math.h>

RevBone::RevBone(void)
{
}

RevBone::~RevBone(void)
{
}

void RevBone::AddChild(const int& aChild) 
{
	m_childObjects.push_back( aChild );
}

CU::Matrix44f RevBone::GetInverseTransformedBindPoseOrientation()
{
	return m_inverseTransformedBindPoseTransformation;
}

CU::Matrix44f RevBone::GetBindPose()
{
	return m_transformedBindPoseTransformation;
}

void RevBone::SetBindPose(CU::Matrix44f aMatrix)
{
	m_orgBindPoseTransformation=aMatrix;
	m_transformedBindPoseTransformation=aMatrix;
	D3DXMATRIX* orgMatrix=reinterpret_cast<D3DXMATRIX*>(&m_transformedBindPoseTransformation);
	D3DXMATRIX* invMatrix=reinterpret_cast<D3DXMATRIX*>(&m_inverseTransformedBindPoseTransformation);
	D3DXMatrixInverse(invMatrix,NULL,orgMatrix);
}

void RevBone::TransformBindPose(const CU::Matrix44f& aMatrix)
{
	m_transformedBindPoseTransformation=m_orgBindPoseTransformation*aMatrix;
	D3DXMATRIX* orgMatrix=reinterpret_cast<D3DXMATRIX*>(&m_transformedBindPoseTransformation);
	D3DXMATRIX* invMatrix=reinterpret_cast<D3DXMATRIX*>(&m_inverseTransformedBindPoseTransformation);
	D3DXMatrixInverse(invMatrix,NULL,orgMatrix);
}

std::vector<int>& RevBone::GetChilds()
{
	return m_childObjects;
}
