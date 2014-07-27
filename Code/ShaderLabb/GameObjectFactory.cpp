#include "stdafx.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "RevInstance.h"
#include "LabStateApp.h"
#include "RevTerrain.h"
#include "tinyxml.h"

GameObjectFactory::GameObjectFactory(void)
{
	m_gameTypes.reserve(10000);
	m_gameObjects.reserve(10000);
}

GameObjectFactory::~GameObjectFactory(void)
{
	DestroyAll();
}

GameObject* GameObjectFactory::CreateGameObject( 
		const CU::Vector3f& pos, 
		const CU::Vector3f& rotation,
		const char* instancePath )
{
	RevModel* model = RevEngineMain::GetModelFactory().CreateModel( instancePath ) ;
	if( model == nullptr )
	{
		return false;
	}

	GameObject* gameObject = new GameObject();
	RevBaseInstance* instance = RevEngineMain::GenerateNewInstance(RevBaseInstance::_HIEARCHY);
	instance->Init( model );

	CU::Matrix44f transformation;
	transformation.SetRotation( GetRotFromPitchYawRoll( rotation.x, rotation.y , rotation.z ) );
	transformation.SetPosition( pos );

	gameObject->SetInstance( instance );
	gameObject->SetTransformation( transformation );

	m_gameObjects.push_back( gameObject );

	return gameObject;
}

GameObject* GameObjectFactory::CreateGameObject(
	const CU::Vector3f& pos,
	const CU::Vector3f& rot,
	const CU::Vector4f& color,
	LabStateApp* world)
{
	RevModel* model = RevEngineMain::GetModelFactory().CreateBox( 1,1,1, color );
	if (model == nullptr)
	{
		return false;
	}

	GameObject* gameObject = new GameObject();
	RevBaseInstance* instance = RevEngineMain::GenerateNewInstance(RevBaseInstance::_SLIM);
	instance->Init(model);

	CU::Matrix44f transformation;
	if (!rot.IsZero())
	{
		transformation.SetRotation(GetRotFromPitchYawRoll(rot.x, rot.y, rot.z));
	}

	transformation.SetPosition(pos);

	gameObject->SetInstance(instance);
	gameObject->SetTransformation(transformation);

	world->AddGameObject(gameObject);
	m_gameObjects.push_back(gameObject);

	return gameObject;
}

GameObject* GameObjectFactory::CreateTerrainGameObject( 
	const CU::Vector3f& pos, 
	const CU::Vector3f& rot,
	const char* terrainPath,
	LabStateApp* world )
{
	RevModelFactory& modelFactory = RevEngineMain::GetModelFactory();
	RevTerrain* terrain = modelFactory.CreateTerrain( terrainPath );

	GameObject* gameObject = new GameObject();
	RevBaseInstance* instance = RevEngineMain::GenerateNewInstance(RevBaseInstance::_SLIM);
	instance->Init( terrain );

	CU::Matrix44f transformation;
	transformation.SetRotation( GetRotFromPitchYawRoll( rot.x, rot.y , rot.z ) );
	transformation.SetPosition( pos );

	gameObject->SetInstance( instance );
	gameObject->SetTransformation( transformation );

	m_gameObjects.push_back( gameObject );

	if( world != nullptr )
	{
		world->AddGameObject( gameObject );
	}

	return gameObject;
}

CU::Matrix33f GameObjectFactory::GetRotFromPitchYawRoll( const float& pitch, const float& yaw, const float& roll )
{
	CU::Matrix33f r;
	r *= CU::Matrix33f::RotateAroundXPI(pitch);
	r *= CU::Matrix33f::RotateAroundYPI(yaw);
	r *= CU::Matrix33f::RotateAroundZPI(roll);
	return r;
}

bool GameObjectFactory::CreateGameObjects( const char* filePath, LabStateApp* world )
{
	if( world == nullptr )	
	{
		assert( 0 && "FAILED LOADING GAME INSTACES" );
		return false;
	}
	
	TiXmlDocument doc;
	if(!doc.LoadFile( filePath ) )
	{
		assert( 0 && "FAILED LOADING GAME INSTACES" );
		return false;
	}

	TiXmlNode* node = doc.FirstChild( "object-instances" );

	TiXmlElement* element = node->FirstChildElement();

	for(; element; element = element->NextSiblingElement() )
	{
		CU::Vector3f objectPos;
		CU::Vector3f rot;
		std::string gameType;
		
		element->QueryFloatAttribute( "pos-x" ,&objectPos.x );
		element->QueryFloatAttribute( "pos-y" ,&objectPos.y );
		element->QueryFloatAttribute( "pos-z" ,&objectPos.z );
		element->QueryFloatAttribute( "rot-x" ,&rot.x );
		element->QueryFloatAttribute( "rot-y", &rot.y );
		element->QueryFloatAttribute( "rot-z", &rot.z );
		gameType = element->Attribute( "type" );

		const GameType* type = GetGameType( gameType.c_str() );
		if( type == nullptr )
		{
			continue;
		}

		GameObject* gObj = CreateGameObject( objectPos, rot, type->m_modelFilePath.c_str() );
		if( gObj != nullptr )
		{
			world->AddGameObject( gObj );
		}
	}

	return true;
}

bool GameObjectFactory::CreateGameTypes( const char* filePath )
{
	TiXmlDocument doc;
	if(!doc.LoadFile( filePath ) )
	{
		assert( 0 && "FAILED LOADING GAME TYPES" );
		return false;
	}	
	
	TiXmlNode* node = doc.FirstChild( "game-objects-types" );

	TiXmlElement* element = node->FirstChildElement();
	for(; element; element = element->NextSiblingElement() )
	{
		std::string modelFilePath;
		std::string gameType;

		modelFilePath = element->Attribute( "instance-path" );
		gameType = element->Attribute( "id" );
		m_gameTypes.push_back( GameType( modelFilePath.c_str(), gameType.c_str() ) );
	}
	return true;
}

void GameObjectFactory::DestroyAll()
{
	for(auto gameObject : m_gameObjects )
	{
		delete gameObject;
	}
}

const GameType* GameObjectFactory::GetGameType( const char* gameType )
{
	for( unsigned int typeIndex = 0;
		typeIndex < m_gameTypes.size();
		typeIndex++)
	{
		const GameType* type = &m_gameTypes[ typeIndex ];
		if( type != nullptr && type->m_gameType == gameType )
		{
			return type;
		}
	}

	return nullptr;
}