#ifndef _REV_EFFECT_H
#define _REV_EFFECT_H

#include "CU_Matrix.h"
#include "CU_Vector.h"

struct ID3DX11EffectTechnique;
struct ID3D10Blob;
struct ID3DX11Effect;
struct ID3DX11EffectMatrixVariable;
struct ID3DX11EffectVectorVariable;
struct ID3DX11EffectScalarVariable;
struct ID3D11ShaderResourceView;

class RevEffect
{

public:

	enum EffectTech
	{
		_NORMAL,
		_CUBE_MAP_ON_OBJECT,
		_NORMAL_DEFERED,
		_RENDER_REFLECTION,
		_RESET,
		_RENDER_LIGHT_RENDERING,
		_RENDER_INSTANCED,
		_PBL_POINT_LIGHT,
		_NUM_OF_TECHS
	};

	enum EffectMatrixVaribles
	{	
		_WORLD,
		_VIEW,
		_PROJ,
		_VIEW_PROJ,
		_WORLD_VIEW_PROJ,
		_INVERSE_VIEW,
		_INVERSE_VIEW_PROJ,
		_INVERSE_WORLD_VIEW_PROJ,
		_INVERSE_WORLD,
		_SCALE_MATRIX,
		_INVERSE_PROJ,
		_BONE_LIST,
		_NUM_OF_MATIRX_VARIABLES
	};

	enum EffectVectorVaribles
	{
		_CAMERA_POS,
		_PIXEL_SIZE,
		_PIXEL_DOWN_SAMPLE_SIZE,
		_DOF_PARAMS,
		_WINDOW_SIZE,
		_FRUSTUM_CORNERS,
		_C_AR_,
		_C_AG_,
		_C_AB_,
		_C_BR_,
		_C_BG_,
		_C_BB_,
		_C_CC_,
		_MIN_WORLD,
		_MAX_WORLD,
		_WORLD_EXTENTS,
		_ENV_BOX_MAX,
		_ENV_BOX_MIN,
		_ENV_BOX_POS,
		_VECTOR_SKYBOXPOS,
		_LIGHT_POS_ARRAY,
		_LIGHT_POS_COLOR,
		_NUM_OF_VECTOR_VARIABLES
	};
	
	enum EffectScalarVaribles
	{
		_Z_FAR_VARIBLE,
		_Z_NEAR_VARIBLE,
		_TEXTILEZEROAMOUNT,
		_TEXTILEONEAMOUNT,
		_TEXTILETWOAMOUNT,
		_TEXTILETHREEAMOUNT,
		_LIGHT_RADIUS,
		_LIGHT_INDEX,
		_NUM_OF_SCALAR_VARIABLES
	};

	RevEffect(void);
	~RevEffect(void);
	bool Init( const char* aFilePath );
	void SetVector2Varible( const int& aVectorIndex, const CU::Vector2f& aVector );
	void SetVector3Varible( const int& aVectorIndex, const CU::Vector3f& aVector );
	void SetVector4Varible( const int& aVectorIndex, const CU::Vector4f& aVector );
	void SetVector4ArrayVarible( const int& aVectorIndex, CU::Vector4f* aVectors, const int& aNVectors );
	void SetMatrixVarible( const int& aMatrixIndex, CU::Matrix44f& aMatrix );
	void SetMatrixArrayVarible( const int& aMatrixIndex, CU::Matrix44f* aMatrix, const int& n );
	void SetTextureVarible(  const char* aResourceName, ID3D11ShaderResourceView* aResource );
	void SetFloatVarible( const int& aScalarIndex, const float& aValue );
	void SetIntVarible(const int& aScalarIndex, const int& value);
	void SetFloatArray(const int& scalarIndex, float* floatArray, const int& n);

	bool IsTechniqueValid( const int& index );
	ID3DX11EffectTechnique* GetTechnique( const int& aIndex );
	ID3DX11EffectTechnique* GetTechniqueByName( const char* aName );
	ID3DX11Effect*	GetEffect( );

private:
	void HandleSavingEffectVariblesToEffect( );

	bool HandleAttemptToSaveShaderToBinary( const char* aFilePath,
											ID3D10Blob** aDataPointer );

	ID3D10Blob* LoadShaderFromBinaryFile(const char* aFilePath);
	bool		SaveShaderToBinaryFile(	const char* aFilePath,
									ID3D10Blob* aShaderBlob );

	ID3DX11Effect*					myBaseEffect;

	ID3DX11EffectTechnique*			myTechs[ _NUM_OF_TECHS ];
	ID3DX11EffectMatrixVariable*	myMatrixes[ _NUM_OF_MATIRX_VARIABLES ];
	ID3DX11EffectVectorVariable*	myVectors[ _NUM_OF_VECTOR_VARIABLES ];
	ID3DX11EffectScalarVariable*	myScalars[ _NUM_OF_SCALAR_VARIABLES ];


};

#endif