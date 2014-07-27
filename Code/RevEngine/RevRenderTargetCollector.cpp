#include "stdafx.h"
#include "RevRenderTargetCollector.h"
#include "RevModel.h"
#include "RevInstance.h"
#include "RevInstanceBaseNode.h"
#include "RevRenderBaseHelper.h"
#include "RevBoneInstanceNode.h"
#include "RevSimpleInstance.h"

RevRenderTargetCollector::RevRenderTargetCollector(void)
{
	myRenderMeshes.reserve( 50000 );
}


RevRenderTargetCollector::~RevRenderTargetCollector(void)
{
}

bool RevRenderTargetCollector::Init( ) //Call this after all the models have been loaded
{
	for(int i = 0; i < 50000; i++)
	{
		myRenderMeshes.push_back( RevRenderMesh() );
	}
	return true;
}

#pragma optimize("", off )

bool RevRenderTargetCollector::AddInstance( RevInstanceBaseNode* instance )
{
	if (!instance)
	{
		return false;
	}
	const int modelID = instance->GetModelID();

	assert( myRenderMeshes.size() > modelID && -1 < modelID );
	
	if( myRenderMeshes[ modelID ].GetModel() == nullptr )
	{
		myRenderMeshes[ modelID ].SetModel( instance->GetModel() );
	}
	RevMesh mesh = RevMesh( instance->GetBaseMatrix(), 0) ;
	if( instance->GetType() == RevModel::_ANIMATED )
	{
		std::vector< CU::Matrix44f > boneMatrixes;
		const std::vector< RevBoneInstanceNode* >& boneInstances
			= instance->GetBoneInstances();
		for(auto boneInstance : boneInstances )
		{
			boneMatrixes.push_back( 
				boneInstance->GetObjectSpaceTransformation() );

		}
		mesh.m_bones = boneMatrixes;
	}

	myRenderMeshes[ modelID ].AddMesh( 
		mesh);

	return true;
}

bool RevRenderTargetCollector::AddInstance(RevSimpleInstance* instance)
{
	if (!instance)
	{
		return false;
	}
	RevModel* selectedModel = instance->GetModel();
	if (!selectedModel)
	{ 
		return false;
	}

	const int modelID = selectedModel->GetModelID();

	assert(myRenderMeshes.size() > modelID && -1 < modelID);
	if (myRenderMeshes[modelID].GetModel() == nullptr)
	{
		myRenderMeshes[modelID].SetModel(selectedModel);
	}

	RevMesh mesh = RevMesh(instance->GetTransformation(), 0);
	myRenderMeshes[modelID].AddMesh(
		mesh);

	return true;
}

#pragma optimize("", on )

bool RevRenderTargetCollector::Render( RevRenderBaseHelper* aRenderHelper )
{
	assert( aRenderHelper != NULL );
	aRenderHelper->Setup( );
	for(unsigned int renderMeshIndex = 0; renderMeshIndex < myRenderMeshes.size(); renderMeshIndex++)
	{
		aRenderHelper->Render( myRenderMeshes[ renderMeshIndex ] );
		myRenderMeshes[ renderMeshIndex ].Clear();
	}

	return true;
}

std::vector< RevRenderMesh >& RevRenderTargetCollector::GetRenderMeshes( )
{
	return myRenderMeshes;
}
