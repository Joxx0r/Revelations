#ifndef _REV_RENDER_TARGET_COLLECTOR_H
#define _REV_RENDER_TARGET_COLLECTOR_H

#include <vector>
#include "RevRenderMesh.h"

class RevInstanceBaseNode;
class RevRenderBaseHelper;
class RevSimpleInstance;

class RevRenderTargetCollector
{
public:
	RevRenderTargetCollector(void);
	~RevRenderTargetCollector(void);
	bool Init( ); //Call this after all the models have been loaded

	bool AddInstance( RevInstanceBaseNode* instance );
	bool AddInstance(RevSimpleInstance* instance);
	bool Render( RevRenderBaseHelper* aRenderHelper );

	std::vector< RevRenderMesh >& GetRenderMeshes( );

private:
	std::vector< RevRenderMesh > myRenderMeshes;
};

#endif
