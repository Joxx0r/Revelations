#ifndef _MODEL_H
#define _MODEL_H

#include "RevStructures.h"
#include "CU_Matrix.h"
#include "RevTextures.h"
#include "RevDefines.h"
#include "RevBone.h"

#include <d3dx11effect.h>

#include <vector>
#include <string>

struct RevInstanceData
{
	CU::Vector4f m_worldOne;
	CU::Vector4f m_worldTwo;
	CU::Vector4f m_worldThree;
};

class RevEffect;
class RevAnimation;

class RevModel
{
public:
	enum ModelType
	{
		_INVALID,
		_NORMAL,
		_TERRAIN,
		_ANIMATED,
		_NUM_OF_MODEL_TYPES
	};

	static int ModelIDCounter; //Counts the amount of models created so that an estimate of the rendering container can be made

	RevModel(void);
	virtual ~RevModel(void);
	
	bool InitShader(  const char* aFilePath );
	bool Init(	RevVertexDataNormal& aVBData,
				RevIndexData& aIBData,
				RevInputData& aInputData,
				RevModelRenderEssentials& aEssentials );

	bool SetupInstanced();
	bool UpdateInstancedVBBuffer(const int& nInstances, const unsigned int& size );
	
	void SetupIBVB();
	bool Render(	CU::Matrix44f& aWorldMatrix,
					const int& aRenderTech );
	bool RenderNormalNoSetupOrChild( const int& renderTech );
	bool RenderInstanced( const int& n );


	const int& GetModelID();
	
	void SetIsNullObjectFlag( const bool& aState );
	const bool& GetIsNullObjectFlag();

	const std::string& GetModelFilePath();
	void SetModelFilePath( const std::string& filePath );

	void AddChild( RevModel* aModel );

	void SetModelTransformation( const CU::Matrix44f& aTransformation );
	void AddTexture( const char* resourceName, ID3D11ShaderResourceView* srv );
	void SetModelTextures( const RevTextures& aTexture );
	void SetBaseModelMatrix( CU::Matrix44f& aMatrix );
	void SetModelType( const ModelType& modelType );
	void SetAnimation( RevAnimation* animation );

	std::vector<RevModel*>& GetChilds();
	RevEffect* GetEffect();
	RevInstanceData* GetInstanceData();
	CU::Matrix44f& GetModelMatrix();
	const ModelType& GetModelType();
	std::vector< int >& GetBoneList();
	std::vector< RevBone >& GetBones();
	RevAnimation* GetAnimation();

private:
	bool InitVB( RevVertexDataNormal& aVBData );
	bool InitIB( RevIndexData& aIBData );
	bool InitInputLayout( RevInputData& aInputData );
	
	RevVertexBuffer							myVB;
	RevIndexBuffer							myIB;
	RevInputLayout							myInputLayout;

	RevVertexBuffer							m_instancedVB;
	RevInputLayout							m_instancedLayout;

	RevEffect*								myEffect;
	RevModelRenderEssentials				myRenderEssentials;

	int										myModelID; //Binded to the static int intergral

	CU::Matrix44f							myMatrix;
	std::vector<RevModel*>					myChilds;

	bool									myIsNullObjectFlag;
	RevTextures								myTextures;

	std::string								m_modelFilePath;

	RevInstanceData							m_instanceData[ NR_OF_MAX_INSTANCED_INSTANCES_PER_MODEL ];
	ModelType								m_modelType;

	std::vector< RevBone >					m_bones;
	std::vector< int >						m_boneList;

	RevAnimation*							m_animation;
};	

#endif