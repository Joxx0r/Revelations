#include "stdafx.h"
#include "RevInstanceFactory.h"
#include "RevInstance.h"
#include "RevSimpleInstance.h"
#include "RevBaseInstance.h"

RevInstanceFactory::RevInstanceFactory(void)
{
	m_instances.reserve( NR_OF_INSTANCES );
}

RevInstanceFactory::~RevInstanceFactory(void)
{
	DestroyAll();
}

RevBaseInstance* RevInstanceFactory::GetNewInstance(const int& newInstance)
{
	RevBaseInstance* instance = nullptr;
	switch (newInstance)
	{
	case RevBaseInstance::_SLIM:
	{
		instance = new RevSimpleInstance();
		break;
	}
	case RevBaseInstance::_HIEARCHY:
	{
		instance = new RevInstance();
		break;
	}
	default:
		assert(0);
		break;
	}

	if (instance)
	{
		m_instances.push_back(instance);
	}

	return instance;
}

void RevInstanceFactory::DestroyAll()
{
	for( auto instance : m_instances )
	{
		delete instance; 
	}

	m_instances.clear();
}