#ifndef _REV_REFLECTION_CUBE_MAP_STRUCTURE_H
#define _REV_REFLECTION_CUBE_MAP_STRUCTURE_H

#include "RevRenderEnviromentHelper.h"

class RevRoom;

class RevReflectionCubeMapStructure
{
public:
	RevReflectionCubeMapStructure();
	~RevReflectionCubeMapStructure();
	void Init( RevRoom* aRoom, const int& aReflectionSize );
	CU::Vector3f GetBiggestPoint();
	CU::Vector3f GetSmallestPoint();
	CU::Vector3f GetHalfExtents();
	RevRenderEnviromentHelper& GetRenderHelper();
	void Activate( );
private:
	RevRoom* myRoom;
	RevRenderEnviromentHelper myRenderHelper;
};

#endif