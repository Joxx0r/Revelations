#include "StdAfx.h"
#include "RevScene.h"
#include "RevInstance.h"
#include "RevEngineMain.h"
#include "RevStructures.h"

RevScene::RevScene(void)
{
	myInstances.reserve(1000000);
	myCamera = nullptr;
}

RevScene::~RevScene(void)
{
	myInstances.clear();
}

bool RevScene::Init( )	
{
	myRoomManager.Init( "niklasRum\\niklasRum_portals.dae" );

	return true;
}

bool RevScene::Update( const float& aDeltaTime )
{
	for( auto instance : myInstances )
	{
		instance->Update( aDeltaTime );
	}
	return true;
}

bool RevScene::Render( )
{
	for( int i = 0; i < static_cast<int>( myInstances.size() ); i++ )
	{
		myInstances[ i ]->Render( );
	}
	return true;
}

void RevScene::SetCamera( RevCamera* newCamera )
{
	myCamera = newCamera;
}

void RevScene::AddInstance(RevBaseInstance* aInstance)
{
	myInstances.push_back( aInstance );
}

void RevScene::RemoveInstance(RevBaseInstance* instance)
{
	std::vector< RevBaseInstance* >::iterator iter = myInstances.begin();
	for(; iter != myInstances.end(); iter++ )
	{
		if( *iter == instance )
		{
			myInstances.erase( iter ); 
		}
	}
}

RevCameraData& RevScene::GetSceneCameraData( )
{
	assert( myCamera != nullptr );
	return myCamera->GetCameraData();
}

std::vector< RevBaseInstance* >& RevScene::GetInstances()
{
	return myInstances;
}

RevRoomManager& RevScene::GetRoomManager( )
{
	return myRoomManager;
}