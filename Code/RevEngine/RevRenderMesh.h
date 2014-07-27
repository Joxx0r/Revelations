#ifndef _REV_RENDER_MESH_H
#define _REV_RENDER_MESH_H

#include "CU_Matrix.h"
#include "RevBone.h"
#include <vector>

class RevModel;
class RevInstance;

struct RevMesh 
{
	RevMesh( const CU::Matrix44f& aWorldMatrix, const int& aReflectionIndex)
	:myWorldMatrix( aWorldMatrix), 
	myReflectionIndex( aReflectionIndex )
	{  
	}

	CU::Matrix44f	myWorldMatrix;
	int				myReflectionIndex;
	std::vector< CU::Matrix44f > m_bones;
};

class RevRenderMesh
{
public:
	RevRenderMesh( );
	RevRenderMesh( RevModel* aModel );
	~RevRenderMesh(void);

	void SetModel( RevModel* aModel );
	void AddMesh( RevMesh&  aMesh );
	void Clear( );

	RevModel* GetModel( );
	std::vector< RevMesh >& GetInstances( );
private:
	std::vector< RevMesh >		myInstances;
	RevModel*					myModel;

};

#endif