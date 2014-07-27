#include "stdafx.h"
#include "GameRoot.h"

GameRoot* GameRoot::m_instance = nullptr;

bool GameRoot::Create()
{
	m_instance = new GameRoot();
	return true;
}

bool GameRoot::Destroy()
{
	delete m_instance; m_instance = nullptr;
	return true;
}

GameObject* GameRoot::CreateTerrain( 
	const CU::Vector3f& pos, const char* terrainPath, LabStateApp* world )
{
	return m_instance->m_gameObjectFactory.CreateTerrainGameObject( pos, CU::Vector3f(0,0,0), terrainPath, world );
}

GameObject* GameRoot::CreateGameObject(  
	const CU::Vector3f& pos,
	const char* instancePath )
{
	return m_instance->m_gameObjectFactory.CreateGameObject( pos, CU::Vector3f(0,0,0), instancePath );
}

bool GameRoot::CreateGameObjects(  const char* filePath, LabStateApp* world )
{
	return m_instance->m_gameObjectFactory.CreateGameObjects( filePath, world );
}

bool GameRoot::CreateGameObjectTypes(  const char* filePath )
{
	return m_instance->m_gameObjectFactory.CreateGameTypes( filePath );
}

bool GameRoot::CreateLights(const char* filePath)
{
	return m_instance ? m_instance->m_lightFactory.GenerateLights(filePath) : false;
}

GameObject* GameRoot::CreateGameObject(const CU::Vector3f& pos,
	const CU::Vector3f& rot,
	const CU::Vector4f& color,
	LabStateApp* world)
{
	return m_instance ? m_instance->m_gameObjectFactory.CreateGameObject(pos, rot, color, world) : false;
}

void GameRoot::DestroyAllGameObjects()
{
	m_instance->m_gameObjectFactory.DestroyAll();
}

void GameRoot::AddMessage( const Message& aMessage )
{
	if( m_instance )
	{
		m_instance->m_postMaster.AddMessage( aMessage );
	}
}

void GameRoot::Subscribe( const MessageListType aMessageType, Subscriber* aSubscriber,SubsciberPrioEnum aPrio/* = SUBSCRIBER_NORMAL*/ )
{
	if( m_instance )
	{
		m_instance->m_postMaster.Subscribe( aMessageType, aSubscriber, aPrio );
	}
}
	
void GameRoot::UnSubscribe(const MessageListType aMessageType, Subscriber* aSubscriber)
{
	if( m_instance )
	{
		m_instance->m_postMaster.UnSubscribe( aMessageType, aSubscriber );
	}
}
	
void GameRoot::DistributeMessages()
{
	if( m_instance )
	{
		m_instance->m_postMaster.DistributeMessages();
	}
}

GameRoot::GameRoot()
{
}

GameRoot::~GameRoot(void)
{
}