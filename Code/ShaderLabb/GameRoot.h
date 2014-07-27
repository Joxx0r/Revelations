#ifndef _GAME_ROOT_H
#define _GAME_ROOT_H

#include "GameObjectFactory.h"
#include "InputHandler.h"
#include "PostMaster.h"
#include "LightFactory.h"

class GameObject;
class LabStateApp;

class GameRoot
{
public:

	static bool Create();
	static bool Destroy();

	static GameObject* CreateTerrain(  const CU::Vector3f& pos, const char* terrainPath, LabStateApp* world );
	static GameObject* CreateGameObject(  const CU::Vector3f& pos, const char* instancePath );
	static bool CreateGameObjects(  const char* filePath, LabStateApp* world );
	static bool CreateGameObjectTypes(  const char* filePath );
	static bool CreateLights(const char* filePath);
	static GameObject* CreateGameObject(const CU::Vector3f& pos,
		const CU::Vector3f& rot,
		const CU::Vector4f& color,
		LabStateApp* world);

	static void DestroyAllGameObjects();

	static void AddMessage( const Message& aMessage );
	static void Subscribe( const MessageListType aMessageType, Subscriber* aSubscriber,SubsciberPrioEnum aPrio = SUBSCRIBER_NORMAL );
	static void UnSubscribe(const MessageListType aMessageType, Subscriber* aSubscriber);
	static void DistributeMessages();

private:
	GameRoot();
	~GameRoot(void);

	static GameRoot* m_instance;

	GameObjectFactory m_gameObjectFactory;

	PostMaster m_postMaster;

	LightFactory m_lightFactory;
	
};

#endif