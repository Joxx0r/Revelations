#include "stdafx.h"
#include "RevLightContainer.h"
#include "RevEffect.h"

RevLightContainer::RevLightContainer()
{
	m_pointLightModel = nullptr;
}


RevLightContainer::~RevLightContainer()
{
}

void RevLightContainer::Init()
{
	m_pointLightModel = RevEngineMain::GetModelFactory().GetSphereSliceDefered();
}

void RevLightContainer::RegisterPointLight(const RevPointLight& revPointLight)
{
	m_pointLights.push_back(revPointLight);
}

void RevLightContainer::Clear()
{
	m_pointLights.clear();
}

void RevLightContainer::UpdateLightsToShader(RevEffect* effect, const RevCameraData& cameraData)
{
	if (!effect)
	{
		return;
	}

	const int n = m_pointLights.size();

	if (n > 0)
	{
		CU::Vector4f colors[NR_OF_LIGHTS];
		CU::Vector4f pos[NR_OF_LIGHTS];
		float radius[NR_OF_LIGHTS];

		for (int lightIndex = 0; lightIndex < n; lightIndex++)
		{
			RevPointLight& pLight = m_pointLights[lightIndex];
			colors[lightIndex] = pLight.myColor;
			pos[lightIndex] = pLight.myPos * cameraData.myViewMatrix;
			radius[lightIndex] = pLight.myRadius;
		}

		effect->SetVector4ArrayVarible(RevEffect::_LIGHT_POS_ARRAY, &pos[0], n);
		effect->SetVector4ArrayVarible(RevEffect::_LIGHT_POS_COLOR, &colors[0], n);
		effect->SetFloatArray(RevEffect::_LIGHT_RADIUS, &radius[0], n);
	}
}