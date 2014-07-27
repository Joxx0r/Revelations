#include "stdafx.h"
#include "RevTransformationHierachyNode.h"


RevTransformationHierachyNode::RevTransformationHierachyNode(void)
{
	m_childs.reserve( 500 );
}

RevTransformationHierachyNode::~RevTransformationHierachyNode(void)
{
}

void RevTransformationHierachyNode::AddTransformation(RevTransformationNode* transformationNode )
{
	m_childs.push_back( transformationNode );
}

CU::Matrix44f RevTransformationHierachyNode::GetTransformationForTime(float aElapsedTime/*=0*/)
{
	CU::Matrix44f orientation= CU::Matrix44f::Identity();

	for(int childIndex=0;childIndex<m_childs.size();childIndex++)
	{
		CU::Matrix44f ori=m_childs[ childIndex ]->GetTransformationForTime(aElapsedTime);
		ori*=orientation;
		orientation=ori;
	}
	return(orientation);
}