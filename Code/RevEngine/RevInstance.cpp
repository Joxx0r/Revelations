#include "StdAfx.h"
#include "RevInstance.h"
#include "RevModel.h"
#include "RevEffect.h"
#include "RevInstanceBaseNode.h"
#include "RevEngineMain.h"

RevInstance::RevInstance(void)
{
	myRoomIndex = -1;
	m_instanceNode = nullptr;
}

RevInstance::~RevInstance(void)
{
	delete m_instanceNode; m_instanceNode = nullptr;
}

bool RevInstance::Init( RevModel* aModel )
{
	assert( m_instanceNode == nullptr );
	m_instanceNode = new RevInstanceBaseNode();
	m_instanceNode->Init( aModel ); 
	return true;
}

bool RevInstance::Update( const float& delta )
{
	assert( m_instanceNode != nullptr );
	m_instanceNode->Update( delta );
	return true;
}

bool RevInstance::Render( )
{
	assert( m_instanceNode != NULL );
	m_instanceNode->Render( m_transformation );
	return true;
}

void RevInstance::SetRoomIndex( const int& aRoomIndex )
{
	myRoomIndex = aRoomIndex;
}

const int& RevInstance::GetRoomIndex()
{
	return myRoomIndex;
}