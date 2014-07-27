#include "stdafx.h"
#include "RevSimpleInstance.h"

RevSimpleInstance::RevSimpleInstance(void)
{
	m_model = nullptr;
}

RevSimpleInstance::~RevSimpleInstance(void)
{
}

bool RevSimpleInstance::Init(RevModel* aModel)
{
	m_model = aModel;
	return true;
}

bool RevSimpleInstance::Render()
{
	RevEngineMain::GetRenderTargetCollector().AddInstance( this );
	return true;
}
