#include "stdafx.h"
#include "GameCamera.h"
#include "GameObject.h"

#pragma optimize("", off )

GameCamera::GameCamera(void)
{
	m_gunObject = nullptr;
	m_rotationSpeed = 0.0f;
	m_movementSpeed = 0.0f;
}

GameCamera::~GameCamera(void)
{
	GameRoot::UnSubscribe( INPUT_MOUSE_POSITION, this );
}

bool GameCamera::Init( const CU::Vector3f& startPos )
{
	m_camera.Init( 90, 0.1f, 10000.0f, RevEngineMain::GetAspectRatio() );
	m_camera.SetPosition( startPos );

	m_rotationSpeed = 9.0f;
	m_movementSpeed = 100.0f;


	m_gunObject = GameRoot::CreateGameObject( startPos, "Data\\GRA_Graphics\\Models\\Weapon_DualBlasters\\DualBlasters_Idle.dae" );
	m_gunObject->Activate();

	//updates the mouse the current current cursor loc.
	m_mouseCurrentFrame = RevEngineMain::GetCursorLoc();
	m_mouseLastFrame = RevEngineMain::GetCursorLoc();

	GameRoot::Subscribe( INPUT_MOUSE_POSITION, this );

	return true;
}


bool GameCamera::Update( const float& deltaTime )
{
	UpdateInput( deltaTime );
	UpdateGraphics( );
	return true;
}

void GameCamera::UpdateInput( const float& deltaTime )
{
	if( GetAsyncKeyState( 0x41 ) ) //A
	{
		m_camera.Left( -m_movementSpeed,deltaTime );
	}
	if( GetAsyncKeyState( 0x44 ) ) //D
	{
		m_camera.Left( m_movementSpeed,deltaTime );
	}
	if( GetAsyncKeyState( 0x57 ) ) //W
	{
		m_camera.Forward(  m_movementSpeed, deltaTime );
	}
	if( GetAsyncKeyState( 0x53 ) ) //S
	{
		m_camera.Forward( -m_movementSpeed, deltaTime );
	}
	
	CU::Vector2f delta = m_mouseLastFrame - m_mouseCurrentFrame;
	if(!m_mouseCurrentFrame.IsZero() )
	{
		m_camera.RotateY( -m_mouseCurrentFrame.x * m_rotationSpeed, deltaTime );
		m_camera.RotateX( m_mouseCurrentFrame.y * m_rotationSpeed, deltaTime );
	}

	m_mouseCurrentFrame.Zero();
	m_mouseLastFrame = RevEngineMain::GetCursorLoc();

	if (!m_gunObject)
	{
		return;
	}
	m_gunObject->SetPosition( m_camera.GetLocation() );
	m_gunObject->SetRotation( m_camera.GetRotation() );

}

void GameCamera::UpdateGraphics( )
{

}

RevCamera* GameCamera::GetEngineCamera()
{
	return &m_camera;
}

void GameCamera::SetPosition( const CU::Vector3f& aPosition )
{
	m_camera.SetPosition( aPosition );
}

bool GameCamera::ReceiveMessage(Message aMessage)
{
	if( aMessage.GetMessageListType() == INPUT_MOUSE_POSITION )
	{
		aMessage.TryGetValue( "PositionRelative", m_mouseCurrentFrame );
	}
	return true;
}

#pragma optimize("", on )