#include "stdafx.h"
#include "RevReflectionRenderHelper.h"
#include "RevRoomManager.h"
#include "RevRenderTargetCollector.h"
#include "RevEngineMain.h"
#include "RevEffectContainer.h"

RevReflectionRenderHelper::RevReflectionRenderHelper(void)
{
	myEnviromentHelpers.reserve( 100 );
}


RevReflectionRenderHelper::~RevReflectionRenderHelper(void)
{
}


bool RevReflectionRenderHelper::Render(	
			const int& aEnvMapSize,
			RevRenderTargetCollector& aRenderMesh, 
			RevRoomManager& aRoomManager )
{
	RevRoomManager& roomManager = aRoomManager;
	roomManager.BuildReflectionMapsOnCubeMaps( myEnviromentHelpers, aEnvMapSize );

	std::vector< RevRenderMesh >& meshes = aRenderMesh.GetRenderMeshes();

	for(int enviromentIndex = 0; enviromentIndex < myEnviromentHelpers.size(); enviromentIndex++)
	{
		RevRenderEnviromentHelper& structure = myEnviromentHelpers[ enviromentIndex ].GetRenderHelper();
		structure.Setup( );
		for(int meshIndex = 0; meshIndex < meshes.size(); meshIndex++ )
		{
			if( meshes[meshIndex].GetInstances().size() >  0 )
			{
				structure.Render( meshes[ meshIndex ] );
			}
		}
	}

	return true;
}

void RevReflectionRenderHelper::ActivateReflectionCubeMap( const int& aRoomIndex )
{
	if( aRoomIndex > -1 && aRoomIndex < myEnviromentHelpers.size() )
	{
		myEnviromentHelpers[ aRoomIndex ].Activate();
	}
}