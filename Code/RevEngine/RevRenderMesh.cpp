#include "stdafx.h"
#include "RevRenderMesh.h"


RevRenderMesh::RevRenderMesh( )
{
	myInstances.reserve( 1000 );
	myModel = NULL;
}

RevRenderMesh::RevRenderMesh( RevModel* aModel )
:myModel( aModel )
{
}


RevRenderMesh::~RevRenderMesh(void)
{
}

void RevRenderMesh::SetModel( RevModel* aModel )
{
	myModel = aModel;
}

void RevRenderMesh::AddMesh( RevMesh&  aMesh )
{
	myInstances.push_back( aMesh );
}

RevModel* RevRenderMesh::GetModel( )
{
	return myModel;
}

void RevRenderMesh::Clear( )
{
	myInstances.clear();
}

std::vector< RevMesh >& RevRenderMesh::GetInstances( )
{
	return myInstances;
}
