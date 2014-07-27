#ifndef _REV_TERRAIN_H
#define _REV_TERRAIN_H

#include "RevModel.h"
#include "RevStructures.h"
#include <vector>

class RevTerrain : public RevModel
{
public:
	RevTerrain(void);
	~RevTerrain(void);

	float m_tileSize;

	float m_terrainHeight;

	std::vector< float > m_heightData;
	std::vector< RevVertexPosUV > m_vertexes;

};

#endif //_REV_TERRAIN_H