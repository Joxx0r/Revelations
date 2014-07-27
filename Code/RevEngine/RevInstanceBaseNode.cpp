#include "stdafx.h"
#include "RevInstanceBaseNode.h"
#include "RevModel.h"
#include "RevAnimationInstanceNode.h"
#include "RevBoneInstanceNode.h"

#pragma optimize("", off )

RevInstanceBaseNode::RevInstanceBaseNode(void)
{
	m_animInstance = nullptr;
	m_model = nullptr;
	m_modelType = RevModel::_INVALID;
}

RevInstanceBaseNode::~RevInstanceBaseNode(void)
{
	delete m_animInstance; m_animInstance = nullptr;
	for( auto boneInstance : m_boneInstances )
	{
		delete boneInstance; 
	}

	m_boneInstances.clear();

	for( auto child : m_childs )
	{
		delete child;
	}

	m_childs.clear();

	delete m_animInstance; m_animInstance = nullptr;
	m_model = nullptr;
}

void RevInstanceBaseNode::Init( RevModel* model )
{
	if( !model )
	{
		return;
	}
	std::vector<RevModel*>& modelChilds = model->GetChilds();
	for(auto childModel : modelChilds)
	{
		RevInstanceBaseNode* childInstance = new RevInstanceBaseNode();
		childInstance->Init( childModel );
		m_childs.push_back( childInstance );
	}

	m_model = model;
	m_modelType = model->GetModelType();

	if (m_modelType == RevModel::_ANIMATED)
	{
		RevAnimation* animation = model->GetAnimation();
		assert( animation );
		if( animation )
		{
			m_animInstance = new RevAnimationInstanceNode();
			m_animInstance->SetAnimation( animation );
		
			std::vector< RevBone >& bone = model->GetBones();
			for( int boneIndex = 0; boneIndex < bone.size(); boneIndex++)
			{
				RevBoneInstanceNode* boneInstance = new RevBoneInstanceNode();
				boneInstance->SetBoneID( boneIndex );
				boneInstance->SetAnimationInstanceNode( m_animInstance );
				boneInstance->SetBone( &bone[boneIndex] );
				m_boneInstances.push_back( boneInstance );
			}
		}
	}
}

void RevInstanceBaseNode::Update( const float& delta )
{
	if( m_animInstance )
	{
		m_animInstance->Update( delta );	
		CU::Matrix44f m;
		m.SetIdentity();
		TransformFrame( 0, m	);
		for( auto boneInstance : m_boneInstances )
		{
			boneInstance->UpdateBindPose();
		}
	}
	for(auto child : m_childs )
	{
		child->Update( delta );
	}

}

void RevInstanceBaseNode::TransformFrame(int boneID ,const CU::Matrix44f& parentWorld)
{
	m_boneInstances[boneID]->Update(parentWorld);
	for(int i=0;i<m_model->GetBones()[boneID].GetChilds().size();i++)
	{
		TransformFrame( 
			m_model->GetBones()[boneID].GetChilds()[i], 
			m_boneInstances[boneID]->GetTransformation());
	}
}

void RevInstanceBaseNode::Render( CU::Matrix44f& parent )
{
	m_baseMatrix = m_model->GetModelMatrix() * parent;
	RevEngineMain::GetRenderTargetCollector().AddInstance( this );
	for(auto child : m_childs )
	{
		child->Render( m_baseMatrix );
	}
}

const CU::Matrix44f& RevInstanceBaseNode::GetBaseMatrix()
{
	return m_baseMatrix;
}

const std::vector< RevBoneInstanceNode* >& RevInstanceBaseNode::GetBoneInstances()
{
	return m_boneInstances;
}

const RevModel::ModelType& RevInstanceBaseNode::GetType()
{
	return m_modelType;
}

const int& RevInstanceBaseNode::GetModelID()
{
	return m_model->GetModelID();	
}

RevModel* RevInstanceBaseNode::GetModel()
{
	return m_model;
}


#pragma optimize("", on )