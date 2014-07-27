#ifndef _GAME_CAMERA_H
#define _GAME_CAMERA_H

#include "CU_Vector.h"
#include "RevCamera.h"
#include "Subscriber.h"

class RevInstance;

class GameCamera : public Subscriber
{
public:
	GameCamera(void);
	~GameCamera(void);

	bool Init( const CU::Vector3f& startPos );

	bool Update( const float& deltaTime );

	RevCamera* GetEngineCamera();
	void SetPosition( const CU::Vector3f& aPosition );

	virtual bool ReceiveMessage(Message aMessage);

private:
	void UpdateInput( const float& deltaTime );

	void UpdateGraphics( );

	CU::Vector2f m_lastFrameLoc;

	RevCamera	m_camera;

	GameObject*	m_gunObject;

	CU::Vector2f m_mouseCurrentFrame;

	CU::Vector2f m_mouseLastFrame;

	float m_movementSpeed;

	float m_rotationSpeed;
};

#endif