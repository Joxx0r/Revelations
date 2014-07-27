#include "stdafx.h"
#include "GameObject.h"
#include "RevInstance.h"

GameObject::GameObject(void)
{
	m_instance = nullptr;
}

GameObject::~GameObject(void)
{
}

bool GameObject::Update( const float& deltaTime )
{
	return true;
}

void GameObject::Die()
{
	if( m_instance != nullptr )
	{
		RevEngineMain::RemoveInstanceFromScene( m_instance );
	}
}

void GameObject::Activate()
{
	if( m_instance != nullptr )
	{
		RevEngineMain::AddInstanceToScene( m_instance );
	}
}

const CU::Vector3f& GameObject::GetPosition()
{
	if(m_instance != nullptr)
	{
		return m_instance->GetPosition();
	}
	return ZEROV;
}

void GameObject::SetPosition(const CU::Vector3f& position)
{
	if(m_instance != nullptr)
	{
		m_instance->SetPosition( position );
	}
}

void GameObject::SetRotation(const CU::Matrix33f& rot )
{
	if(m_instance != nullptr)
	{
		m_instance->SetRotation( rot );
	}
}

void GameObject::SetTransformation(const CU::Matrix44f& transformation )
{
	if(m_instance != nullptr)
	{
		m_instance->SetTransformation( transformation );
	}
}

void GameObject::SetInstance(RevBaseInstance* instance)
{
	m_instance = instance;
}