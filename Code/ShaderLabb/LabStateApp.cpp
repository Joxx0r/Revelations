#include "StdAfx.h"
#include "LabStateApp.h"
#include "RevEngineMain.h"
#include "RevInstance.h"
#include "RevScene.h"
#include "GameObject.h"

#include <time.h>
#include <cstdlib>

LabStateApp::LabStateApp(void)	
{
	m_gameObjects.reserve( 100000 );
}

LabStateApp::~LabStateApp(void)
{
	GameRoot::Destroy();
}

void LabStateApp::Init()
{
	GameRoot::Create();

	m_gameCamera.Init( CU::Vector3f(0,0,-5) );
	RevEngineMain::ChangeCameraToScene( m_gameCamera.GetEngineCamera() );

//	GameRoot::CreateGameObject(CU::Vector3f(0, 0, 0), CU::Vector3f(0, 0, 0), CU::Vector4f(1, 1, 1, 1), this);
	GameRoot::CreateGameObjectTypes( "GameObjects.xml" );
	GameRoot::CreateGameObjects("Level1GameInstances.xml", this );
	GameRoot::CreateLights("Level1Lights.xml");
//	GameRoot::CreateTerrain( CU::Vector3f(0,0,0), "Level_Crosses_Height_output.r16", this );

	RevEngineMain::AllModelsLoaded();
	RevEngineMain::GetRenderer().OnAllItemsLoaded();
}

void LabStateApp::Activate( )
{
	for( auto gameObject : m_gameObjects )
	{
		assert( gameObject != nullptr );
		gameObject->Activate();
	}
}

bool LabStateApp::Update( const float& aDeltaTime )
{
	m_gameCamera.Update( aDeltaTime );
	for(auto gameObject : m_gameObjects )
	{
		gameObject->Update( aDeltaTime );
	}
	return true;
}

bool LabStateApp::Render( )
{
	return true;
}

void LabStateApp::AddGameObject( GameObject* gameObject )
{
	assert( gameObject != nullptr );
	if( gameObject )
	{
		m_gameObjects.push_back( gameObject );
	}
}