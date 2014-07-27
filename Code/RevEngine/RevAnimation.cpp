#include "stdafx.h"
#include "RevAnimation.h"


RevAnimation::RevAnimation(void)
{
}
	

RevAnimation::~RevAnimation(void)
{
	for(auto transformationNode : m_transformationNodes )
	{
		delete transformationNode;
	}
}

void RevAnimation::Init(const int& nrOfExpectedBones)
{
	m_transformationNodes.reserve( nrOfExpectedBones );
}

void RevAnimation::AddTransformationNode(RevTransformationNode* node)
{
	m_transformationNodes.push_back( node );
}

std::vector< RevTransformationNode* >* RevAnimation::GetRevTransformationNodes()
{
	return &m_transformationNodes;
}