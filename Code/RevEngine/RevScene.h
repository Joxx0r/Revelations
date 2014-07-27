#ifndef _SCENE_H
#define _SCENE_H

#include "RevCamera.h"
#include "RevRoomManager.h"
#include <vector>

class RevModel;
class RevBaseInstance;

class RevScene
{
public:
	RevScene(void);
	~RevScene(void);
	bool Init( );
	bool Update( const float& aDeltaTime );
	bool Render( );
	void AddInstance(RevBaseInstance* aInstance);
	void RemoveInstance(RevBaseInstance* instance);

	void SetCamera( RevCamera* newCamera );
	RevCameraData& GetSceneCameraData( );

	std::vector< RevBaseInstance* >& GetInstances();
	RevRoomManager& GetRoomManager( );

private:
	std::vector< RevBaseInstance* > myInstances;
	RevCamera*					myCamera;
	RevRoomManager				myRoomManager;
};

#endif