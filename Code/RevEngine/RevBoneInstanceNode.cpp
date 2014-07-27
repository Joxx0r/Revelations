#include "stdafx.h"
#include "RevBoneInstanceNode.h"
#include "RevAnimationInstanceNode.h"

#pragma optimize("", off )

RevBoneInstanceNode::RevBoneInstanceNode(void)
{
	m_boneID = -1;
	m_revAnimationInstance = nullptr;
	m_bone = nullptr;
}

RevBoneInstanceNode::~RevBoneInstanceNode(void)
{
	m_revAnimationInstance = nullptr;
}

void RevBoneInstanceNode::Update( const CU::Matrix44f& parent )
{
	if( m_revAnimationInstance )
	{
		m_transformation = m_revAnimationInstance->GetBoneTransformation( m_boneID );
		m_transformation *= parent;
	}
}

void RevBoneInstanceNode::UpdateBindPose()
{
	if( m_bone )
	{
		CU::Matrix44f test1=m_bone->GetInverseTransformedBindPoseOrientation();
		test1*=m_transformation;
		m_objectSpaceTransformation=test1;
	}
}

void RevBoneInstanceNode::SetBoneID( const int& boneID )
{
	m_boneID = boneID;
}

void RevBoneInstanceNode::SetAnimationInstanceNode( RevAnimationInstanceNode* node )
{
	m_revAnimationInstance = node;
}

void RevBoneInstanceNode::SetBone( RevBone* bone )
{
	m_bone = bone;
}

CU::Matrix44f RevBoneInstanceNode::GetTransformation()
{
	return m_transformation;
}

CU::Matrix44f RevBoneInstanceNode::GetObjectSpaceTransformation()
{
	return m_objectSpaceTransformation;
}

#pragma optimize("", on )