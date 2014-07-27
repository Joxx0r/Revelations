#include "stdafx.h"
#include "RevTransformationNode.h"

RevTransformationNode::RevTransformationNode(void)
{
}

RevTransformationNode::~RevTransformationNode(void)
{
}

CU::Matrix44f RevTransformationNode::GetTransformationForTime(float aElapsedTime/*=0*/)
{
	return m_transformation;
}

void RevTransformationNode::SetTransformation( const CU::Matrix44f& transformation )
{
	m_transformation = transformation;
}