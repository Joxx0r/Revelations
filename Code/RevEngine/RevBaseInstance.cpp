#include "stdafx.h"
#include "RevBaseInstance.h"

void RevBaseInstance::SetTransformation(const CU::Matrix44f& aTransformation)
{
	m_transformation = aTransformation;
}

void RevBaseInstance::SetPosition(const CU::Vector4f& aPosition)
{
	m_transformation.SetPosition(aPosition);
}

void RevBaseInstance::SetRotation(const CU::Matrix33f& aRotation)
{
	m_transformation.SetRotation(aRotation);
}

const CU::Matrix44f& RevBaseInstance::GetTransformation()
{
	return m_transformation;
}

CU::Vector4f RevBaseInstance::GetPosition()
{
	return m_transformation.GetPosition();
}
