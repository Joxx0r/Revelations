#pragma once

#include <vector>
#include "RevStructures.h"

class RevEffect;
class RevModel;

struct RevCameraData;
struct ID3D11BlendState;

class RevLightContainer
{
public:
	RevLightContainer();
	~RevLightContainer();

	void Init();

	void RegisterPointLight(const RevPointLight& revPointLight );

	void Clear();

	void UpdateLightsToShader(RevEffect* effect, const RevCameraData& cameraData );

	RevModel* GetPointLightModel() { return m_pointLightModel;  }
	std::vector< RevPointLight >& GetPointLights() { return m_pointLights;  }

private:

	std::vector< RevPointLight > m_pointLights;

	RevModel* m_pointLightModel;

};

