#ifndef _LABB_STATE_APP_H
#define _LABB_STATE_APP_H

#include "GameCamera.h"

class RevScene;
class GameObject;
class InputHandler;

class LabStateApp
{
public:
	LabStateApp(void);
	~LabStateApp(void);
	void Init();
	void Activate( );
	bool Update( const float& aDeltaTime );
	bool Render( );
	
	void AddGameObject( GameObject* gameObject );

private:

	std::vector< GameObject* > m_gameObjects;

	GameCamera m_gameCamera;
};

#endif