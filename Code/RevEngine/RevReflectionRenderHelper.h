#ifndef _REV_REFLECTION_RENDER_HELPER_H
#define _REV_REFLECTION_RENDER_HELPER_H

#include "RevReflectionCubeMapStructure.h"
#include <vector>

class RevRenderTargetCollector;
class RevRoomManager;

class RevReflectionRenderHelper
{
public:
	RevReflectionRenderHelper(void);
	~RevReflectionRenderHelper(void);
	bool Render(	const int& aEnvMapSize,
					RevRenderTargetCollector& aRenderMesh, 
					RevRoomManager& aRoomManager );
	void ActivateReflectionCubeMap( const int& aRoomIndex );
					 
private:
	std::vector< RevReflectionCubeMapStructure > myEnviromentHelpers;

};

#endif