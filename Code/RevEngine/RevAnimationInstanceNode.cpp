#include "stdafx.h"
#include "RevAnimationInstanceNode.h"
#include "RevTransformationNode.h"
#include "RevAnimation.h"

RevAnimationInstanceNode::RevAnimationInstanceNode(void)
{
	m_sharedAnimation = nullptr;
	m_transformationNodes = nullptr;
	m_elapsedTime= 0.0f;
}

RevAnimationInstanceNode::~RevAnimationInstanceNode(void)
{
}

void RevAnimationInstanceNode::SetAnimation( RevAnimation* animation )
{
	if( !animation )
	{
		assert( 0 && "INVALID ANIMATION SENT TO ANIMATION INSTANCE NODE");
	}

	m_sharedAnimation = animation;
	m_transformationNodes = m_sharedAnimation->GetRevTransformationNodes();
}

void RevAnimationInstanceNode::Update( const float& deltaTime )
{
	m_elapsedTime += deltaTime;
	m_transformedInstances.clear();
	for( int transformNodeIndex = 0;
		transformNodeIndex < (*m_transformationNodes).size();
		transformNodeIndex++) 
	{
		RevTransformationNode* transNode = (*m_transformationNodes)[transformNodeIndex];
		m_transformedInstances.push_back(transNode->GetTransformationForTime( m_elapsedTime ) );
	}
}

std::vector< CU::Matrix44f >& RevAnimationInstanceNode::GetAnimationTransformedNodes()
{
	return m_transformedInstances;
}

const CU::Matrix44f& RevAnimationInstanceNode::GetBoneTransformation( const int& aBoneIndex )
{
	assert( aBoneIndex > -1 && aBoneIndex < m_transformedInstances.size() );
	return m_transformedInstances[aBoneIndex];
}