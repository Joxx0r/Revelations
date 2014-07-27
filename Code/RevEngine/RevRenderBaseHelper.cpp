#include "stdafx.h"
#include "RevRenderBaseHelper.h"
#include "RevEffect.h"
#include "RevModel.h"
#include "RevRenderMesh.h"

RevRenderBaseHelper::RevRenderBaseHelper(void)
{
	myRenderTech = -1;
}


RevRenderBaseHelper::~RevRenderBaseHelper(void)
{
}

void RevRenderBaseHelper::SetCameraData( const RevCameraData& aCameraData )
{
	myCameraData = aCameraData;
}

void RevRenderBaseHelper::SetRenderTech( const int& aRenderTech )
{
	myRenderTech = aRenderTech;
}

void RevRenderBaseHelper::RenderInstancedInstances( RevRenderMesh& instanceRendered )
{
	RevModel* model = instanceRendered.GetModel();
	if( model == nullptr )
	{
		return;
	}
	RenderModel( model, instanceRendered );
}

void RevRenderBaseHelper::RenderModel( RevModel* model,  RevRenderMesh& instanceRendered )
{
	const RevModel::ModelType modelType = model->GetModelType();
	switch( modelType )
	{
		case RevModel::_NORMAL:
			RenderNormalInstances( model, instanceRendered );
			break;
		case RevModel::_TERRAIN:
			RenderTerrainInstance( model, instanceRendered );
			break;
		case RevModel::_ANIMATED:
			RenderSkinnedInstance( model, instanceRendered );
			break;
		default:
			assert(0 && "INVALID MODEL TYPE" );
	};
}

void RevRenderBaseHelper::RenderNormalInstances(
	RevModel* model, 
	RevRenderMesh& instanceRendered )
{
	if(model->GetIsNullObjectFlag() == false )
	{
		std::vector< RevMesh >& instances = instanceRendered.GetInstances();
		CU::Matrix44f m;
		RevInstanceData* instanceData = model->GetInstanceData();
		if (instanceData != nullptr && instances.size() > 0)
		{
			const int n = instances.size() >= NR_OF_MAX_INSTANCED_INSTANCES_PER_MODEL ? NR_OF_MAX_INSTANCED_INSTANCES_PER_MODEL : instances.size();
			for(int i = 0; i < n; i++ )
			{
				m = instances[ i ].myWorldMatrix;

				RevInstanceData data;
				instanceData[ i ].m_worldOne.x = m.myMatrix[0][0];
				instanceData[ i ].m_worldOne.y = m.myMatrix[0][1];
				instanceData[ i ].m_worldOne.z = m.myMatrix[0][2];
				instanceData[ i ].m_worldOne.w = m.myMatrix[3][0];

				instanceData[ i ].m_worldTwo.x = m.myMatrix[1][0];
				instanceData[ i ].m_worldTwo.y = m.myMatrix[1][1];
				instanceData[ i ].m_worldTwo.z = m.myMatrix[1][2];
				instanceData[ i ].m_worldTwo.w = m.myMatrix[3][1];

				instanceData[ i ].m_worldThree.x = m.myMatrix[2][0];
				instanceData[ i ].m_worldThree.y = m.myMatrix[2][1];
				instanceData[ i ].m_worldThree.z = m.myMatrix[2][2];
				instanceData[ i ].m_worldThree.w = m.myMatrix[3][2];
			}

			const unsigned int byteWidth = n * sizeof( RevInstanceData );

			model->UpdateInstancedVBBuffer( n, byteWidth );
			model->RenderInstanced( n );
		}
	}

}

void RevRenderBaseHelper::RenderTerrainInstance( 
	RevModel* model, 
	RevRenderMesh& instanceRendered  )
{
	std::vector< RevMesh >& instances = instanceRendered.GetInstances();
	for( auto instance : instances )
	{
		model->Render( instance.myWorldMatrix, RevEffect::_NORMAL );
	}
}

void RevRenderBaseHelper::RenderSkinnedInstance( 
	RevModel* model,  
	RevRenderMesh& instanceRendered  )
{
	RevEffect* effect = model->GetEffect();
	assert( effect != nullptr );
	std::vector< RevMesh >& instances = instanceRendered.GetInstances();
	for( auto instance : instances )
	{
		if(instance.m_bones.size()  > 0 )
		{
			effect->SetMatrixArrayVarible( RevEffect::_BONE_LIST, &instance.m_bones[0],instance.m_bones.size()  );
			model->Render( instance.myWorldMatrix, RevEffect::_NORMAL );
		}
	}
}