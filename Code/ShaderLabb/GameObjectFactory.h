#ifndef _GAME_OBJECT_FACTORY_H
#define _GAME_OBJECT_FACTORY_H

#include <vector>
#include <string>

class GameObject;
class LabStateApp;

struct GameType
{
	GameType(const char* modelFilePath, const char* objectType)
		:m_modelFilePath( modelFilePath )
		, m_gameType( objectType ) 
	{

	}

	std::string m_modelFilePath;
	std::string m_gameType;
};

class GameObjectFactory
{
public:
	GameObjectFactory(void);
	~GameObjectFactory(void);

	GameObject* CreateGameObject( 
		const CU::Vector3f& pos, 
		const CU::Vector3f& rot,
		const char* instancePath );

	GameObject* CreateGameObject(
		const CU::Vector3f& pos,
		const CU::Vector3f& rot,
		const CU::Vector4f& color,
		LabStateApp* world);


	GameObject* CreateTerrainGameObject( 
		const CU::Vector3f& pos, 
		const CU::Vector3f& rot,
		const char* terrainPath,
		LabStateApp* world  );

	bool CreateGameObjects( const char* filePath, LabStateApp* world );
	bool CreateGameTypes( const char* filePath );

	void DestroyAll();

private:

	CU::Matrix33f GetRotFromPitchYawRoll( const float& pitch, const float& yaw, const float& roll );

	const GameType* GetGameType( const char* gameType );

	std::vector< GameObject* > m_gameObjects;
	std::vector< GameType > m_gameTypes;
};

#endif