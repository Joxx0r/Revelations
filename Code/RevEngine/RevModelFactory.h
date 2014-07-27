#ifndef _MODEL_FACTORY_H
#define _MODEL_FACTORY_H

#include "windows.h"
#include "ColladaMeshFactory.h"

class RevModel;
class RevTerrain;
struct ID3D11ShaderResourceView;

class RevModelFactory
{
public:
	RevModelFactory(void);
	~RevModelFactory(void);

	/*RevModel* GetSphereModel(	const float& aRadius,
								const float& aSlices,
								const float& aR, 
								const CreateTerrainfloat& aG,
								const float& aB );*/
	RevModel* GetSphereSliceDefered( );
	RevModel* GetSphereModel(float radius, UINT sliceCount, UINT stackCount, const float& aR, const float& aG, const float& aB);
	RevModel* CreateBox(float width, float height, float depth, const CU::Vector4f& color );
	RevModel* CreateModel( const char* aFilePath );
	RevTerrain* CreateTerrain( const char* terrainPath );
	void CreateRooms( const char* aFilePath, std::vector< RevRoom* >& aRooms );
private:

	ID3D11ShaderResourceView* CreateQuadPatchR16Texture( RevTerrain* terrain );

	ColladaMeshFactory myMeshFactory;

	std::vector< RevModel* > m_createdModels;
};

#endif