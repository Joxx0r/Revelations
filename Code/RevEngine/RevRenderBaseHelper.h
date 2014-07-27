#ifndef _REV_RENDER_BASE_HELPER_H
#define _REV_RENDER_BASE_HELPER_H

#include "RevStructures.h"

class RevRenderMesh;

class RevRenderBaseHelper
{
public:
	RevRenderBaseHelper(void);
	virtual ~RevRenderBaseHelper(void);

	virtual bool Init() = 0;
	virtual void Render(RevRenderMesh& aRenderMesh) { assert(0 && "SHOULD NOT CALL BASECLASS"); };
	virtual void RenderNoMesh() { assert(0 && "SHOULD NOT CALL BASECLASS"); };
	virtual bool Setup( ) = 0;

	void SetCameraData(const RevCameraData& aCameraData);
	void SetRenderTech(const int& aRenderTech);
protected:
	void RenderModel( RevModel* model,  RevRenderMesh& instanceRendered );
	void RenderInstancedInstances( RevRenderMesh& instanceRendered );

	void RenderNormalInstances( RevModel* model,  RevRenderMesh& instanceRendered  );
	void RenderTerrainInstance( RevModel* model,  RevRenderMesh& instanceRendered  );
	void RenderSkinnedInstance( RevModel* model,  RevRenderMesh& instanceRendered  );

	RevCameraData	myCameraData;
	int				myRenderTech;
};

#endif