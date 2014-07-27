#include "StdAfx.h"
#include "RevEffect.h"
#include "RevEngineMain.h"

#include <d3dx11effect.h>
#include <string>
#include <d3dx11.h>
#include <iostream>     // std::cout
#include <fstream>

#include <D3Dcompiler.h>
#include <assert.h>

RevEffect::RevEffect(void)
{
	for(int i = 0; i < _NUM_OF_TECHS; i++)				{ myTechs[ i ] = NULL; }
	for(int i = 0; i < _NUM_OF_MATIRX_VARIABLES; i++)	{ myMatrixes[ i ] = NULL; }
	for(int i = 0; i < _NUM_OF_VECTOR_VARIABLES; i++)	{ myVectors[ i ] = NULL; }

	myBaseEffect = NULL;
}

RevEffect::~RevEffect(void)
{
	for(int i = 0; i < _NUM_OF_TECHS; i++)				{ myTechs[ i ] = NULL; }
	for(int i = 0; i < _NUM_OF_MATIRX_VARIABLES; i++)	{ myMatrixes[ i ] = NULL; }
	for(int i = 0; i < _NUM_OF_VECTOR_VARIABLES; i++)	{ myVectors[ i ] = NULL; }
	if( myBaseEffect != NULL )
	{
		myBaseEffect->Release();
	}
}

bool RevEffect::Init( const char* aFilePath )
{
	HRESULT hr = E_FAIL;

	ID3D10Blob* compileShader = NULL;
	//Toggle this define if you want to compile shader files every startup
#ifdef COMPILE_EFFECT_FILES
	bool createdBinaryFile  = HandleAttemptToSaveShaderToBinary(  aFilePath, &compileShader );
	if( createdBinaryFile == false) return false; 
#else
	compileShader = LoadShaderFromBinaryFile( aFilePath );
	if( compileShader == NULL )
	{
		bool createdBinaryFile  = HandleAttemptToSaveShaderToBinary(  aFilePath, &compileShader );
		if( createdBinaryFile == false) return false; 
	}
#endif
	hr = D3DX11CreateEffectFromMemory(	compileShader->GetBufferPointer(), 
		compileShader->GetBufferSize(),
		NULL, 
		RevEngineMain::GetDevice(), 
		&myBaseEffect);

	if( hr != S_OK )
	{
		if( compileShader != NULL )
		{
			compileShader->Release();
		}
		assert( 0 && "FAILED TO INILIZE MEMORY FROM EFFECT FILE" );
		if( myBaseEffect != NULL )
		{
			myBaseEffect->Release();
			myBaseEffect = NULL;
		}
		return false;
	}

	HandleSavingEffectVariblesToEffect( );

	return true;
}

void RevEffect::SetVector2Varible( const int& aVectorIndex, const CU::Vector2f& aVector )
{
#if defined(DEBUG) || defined(_DEBUG)
	assert( aVectorIndex > 0 || aVectorIndex < _NUM_OF_VECTOR_VARIABLES );
	if( aVectorIndex < 0 || aVectorIndex > _NUM_OF_VECTOR_VARIABLES ) return;
	if( myVectors[ aVectorIndex ] == NULL ) return;
#endif

	myVectors[ aVectorIndex ]->SetRawValue( &aVector, 0, sizeof( CU::Vector4f ) );
}

void RevEffect::SetVector3Varible( const int& aVectorIndex, const CU::Vector3f& aVector )
{
#if defined(DEBUG) || defined(_DEBUG)
	assert( aVectorIndex > 0 || aVectorIndex < _NUM_OF_VECTOR_VARIABLES );
	if( aVectorIndex < 0 || aVectorIndex > _NUM_OF_VECTOR_VARIABLES ) return;
	if( myVectors[ aVectorIndex ] == NULL ) return;
#endif

	myVectors[ aVectorIndex ]->SetRawValue( &aVector, 0, sizeof( CU::Vector3f ) );
}

void RevEffect::SetVector4Varible( const int& aVectorIndex, const CU::Vector4f& aVector )
{
#if defined(DEBUG) || defined(_DEBUG)
	assert( aVectorIndex > 0 || aVectorIndex < _NUM_OF_VECTOR_VARIABLES );
	if( aVectorIndex < 0 || aVectorIndex >= _NUM_OF_VECTOR_VARIABLES ) return;
	if( myVectors[ aVectorIndex ] == NULL ) return;
#endif
	myVectors[ aVectorIndex ]->SetRawValue( &aVector, 0, sizeof( CU::Vector4f ) );
}

void RevEffect::SetVector4ArrayVarible( const int& aVectorIndex, CU::Vector4f* aVectors, const int& aNVectors )
{
	if(myVectors[ aVectorIndex ] == NULL) return;
#if defined(DEBUG) || defined(_DEBUG)
	assert( aVectorIndex > 0 || aVectorIndex < _NUM_OF_VECTOR_VARIABLES );
	if( aVectorIndex < 0 || aVectorIndex > _NUM_OF_VECTOR_VARIABLES ) return;
	if( myVectors[ aVectorIndex ] == NULL ) return;
#endif
	myVectors[ aVectorIndex ]->SetFloatVectorArray( reinterpret_cast<float*>(aVectors), 0, aNVectors );
}

void RevEffect::SetMatrixVarible( const int& aMatrixIndex, CU::Matrix44f& aMatrix )
{
#if defined(DEBUG) || defined(_DEBUG)
	assert( aMatrixIndex > 0 || aMatrixIndex < _NUM_OF_MATIRX_VARIABLES );
	if( aMatrixIndex < 0 || aMatrixIndex > _NUM_OF_MATIRX_VARIABLES ) return;
	if( myMatrixes[ aMatrixIndex ] == NULL ) return;
#endif
	myMatrixes[ aMatrixIndex ]->SetMatrix( reinterpret_cast<float*>( &aMatrix.myMatrix[0][0] ) );
}

void RevEffect::SetMatrixArrayVarible( const int& aMatrixIndex, CU::Matrix44f* aMatrix, const int& n )
{
	#if defined(DEBUG) || defined(_DEBUG)
	assert( aMatrixIndex > 0 || aMatrixIndex < _NUM_OF_MATIRX_VARIABLES );
	if( aMatrixIndex < 0 || aMatrixIndex > _NUM_OF_MATIRX_VARIABLES ) return;
	if( myMatrixes[ aMatrixIndex ] == NULL ) return;
#endif
	myMatrixes[ aMatrixIndex ]->SetMatrixArray(  reinterpret_cast<float*>(aMatrix), 0, n );
}

void RevEffect::SetTextureVarible(  const char* aResourceName, ID3D11ShaderResourceView* aResource )
{
#if defined(DEBUG) || defined(_DEBUG)
	assert( myBaseEffect != NULL );
	assert( myBaseEffect->GetVariableByName( aResourceName )->IsValid() );
#endif
	myBaseEffect->GetVariableByName( aResourceName )->AsShaderResource()->SetResource( aResource );
}

void RevEffect::SetFloatVarible( const int& aScalarIndex, const float& aValue )
{
#if defined(DEBUG) || defined(_DEBUG)
	assert( myBaseEffect != NULL );
#endif
	if( myScalars[ aScalarIndex ] == NULL ) return;
	myScalars[ aScalarIndex ]->SetFloat( aValue );
}

void RevEffect::SetIntVarible(const int& aScalarIndex, const int& value)
{
#if defined(DEBUG) || defined(_DEBUG)
	assert(myBaseEffect != NULL);
#endif
	if (myScalars[aScalarIndex] == NULL) return;
	myScalars[aScalarIndex]->SetInt(value);
}

void RevEffect::SetFloatArray(const int& scalarIndex, float* floatArray, const int& n)
{
#if defined(DEBUG) || defined(_DEBUG)
	assert(myBaseEffect != NULL);
	assert(myScalars[scalarIndex] == NULL);
#endif
	if (myScalars[scalarIndex] == NULL) return;
	myScalars[scalarIndex]->SetFloatArray(floatArray, 0, n);
}

bool RevEffect::IsTechniqueValid( const int& index )
{
	return myTechs[ index ]->IsValid();
}

ID3DX11EffectTechnique* RevEffect::GetTechnique( const int& aIndex )
{
#if defined(DEBUG) || defined(_DEBUG)
	assert( aIndex > 0 || aIndex < _NUM_OF_TECHS );
	if( aIndex < 0 || aIndex > _NUM_OF_TECHS ) return NULL;
	assert( myTechs[ aIndex ]->IsValid() == true );
#endif
	return myTechs[ aIndex ];
}

ID3DX11EffectTechnique* RevEffect::GetTechniqueByName( const char* aName )
{
	assert( myBaseEffect != NULL );
	ID3DX11EffectTechnique* tech = myBaseEffect->GetTechniqueByName(  aName );
#if defined(DEBUG) || defined(_DEBUG)
	assert( tech->IsValid() );
#endif
	return tech;
}

ID3DX11Effect*	RevEffect::GetEffect( )
{
#if defined(DEBUG) || defined(_DEBUG)
	assert( myBaseEffect != NULL );
#endif
	return myBaseEffect;
}

void RevEffect::HandleSavingEffectVariblesToEffect( )
{
	//Method of saving the names of the specific varibles, should probably be data driven but not currently
	const char* techNames[ _NUM_OF_TECHS ] = {	
			"Render",
			"RenderCubeMapOnObject",
			"RenderDefered",
			"RenderReflection",
			"Reset",
			"RenderComplete",
			"RenderInstanced",
			"RenderPBLPointLight" };

	const char* matrixNames[ _NUM_OF_MATIRX_VARIABLES ] 
	= { "g_World",
		"g_View",
		"g_Proj",
		"g_ViewProj",
		"g_WorldViewProj",
		"g_InverseView",
		"g_InverseViewProj",
		"g_InverseWorldViewProj",
		"g_InverseWorld",
		"g_ScaleMatrix",
		"g_InverseProj",
		"g_BoneList"
	};

	const char* vectorNames[ _NUM_OF_VECTOR_VARIABLES ] 
	= { "g_EyePos",
		"g_PixelSizeHigh",
		"g_PixelSizeLow",
		"vDofParams",
		"WindowSize",
		"FrustumCorners",
		"cAr",
		"cAg",
		"cAb",
		"cBr",
		"cBg",
		"cBb",
		"cC",
		"g_MinWorld",
		"g_MaxWorld",
		"g_Extents",
		"g_EnvBoxMax",
		"g_EnvBoxMin",
		"g_EnvBoxPos",
		"g_SkyboxPos",
		"g_LightPos",
		"g_LightColors"
	};

	const char* floatNames[_NUM_OF_SCALAR_VARIABLES]
	= { "g_ZFar",
		"g_ZNear",
		"TexTile0Amount",
		"TexTile1Amount",
		"TexTile2Amount",
		"TexTile3Amount",
		"g_Radius",
		"g_ActiveIndex"
		};

	for(int i = 0; i < _NUM_OF_TECHS; i++)
	{
		myTechs[ i ] = myBaseEffect->GetTechniqueByName( techNames[ i ] );
	}

	for(int i = 0; i < _NUM_OF_MATIRX_VARIABLES; i++)
	{
		myMatrixes[ i ] = myBaseEffect->GetVariableByName( matrixNames[ i ] )->AsMatrix();
	}

	for(int i = 0; i < _NUM_OF_VECTOR_VARIABLES; i++)
	{
		myVectors[ i ] = myBaseEffect->GetVariableByName( vectorNames[ i ] )->AsVector();
	}
	for (int i = 0; i < _NUM_OF_SCALAR_VARIABLES; i++)
	{
		myScalars[ i ] = myBaseEffect->GetVariableByName( floatNames[ i ] )->AsScalar();
	}
}

bool RevEffect::HandleAttemptToSaveShaderToBinary(	const char* aFilePath,
												  ID3D10Blob** aDataPointer  )
{
	DWORD shaderFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shaderFlags |= D3D10_SHADER_DEBUG;
	shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
		
	//Replaces the initial .txt to .fx so it can compile the shader file to binary
	std::string filePath(aFilePath);
	filePath.erase( filePath.find_last_of( '.' ), filePath.size()-1);
	filePath.append( ".fx" );

	ID3D10Blob* compileErrors = NULL;

	HRESULT hr = D3DX11CompileFromFile(	
		filePath.c_str(),
		0,
		0,
		NULL,
		"fx_5_0",
		shaderFlags,	
		0,
		0,
		aDataPointer,
		&compileErrors,
		0);

	if(FAILED(hr))
	{
		if(compileErrors!=0)
		{
			MessageBox( NULL,(char*)compileErrors->GetBufferPointer(),"Error", MB_OK);
		}
		return false;
	}

	return SaveShaderToBinaryFile( aFilePath, *aDataPointer );
}


ID3D10Blob* RevEffect::LoadShaderFromBinaryFile(const char* aFilePath)
{

	ID3D10Blob*	shaderBlob=NULL;
	std::ifstream fin(aFilePath,std::ios::binary);
	if(fin.bad()||fin.fail())
	{
		return NULL;
	}
	int size;
	fin.read((char*)&size,sizeof(int));
	void* buf=malloc(size);
	fin.read((char*)buf,size);

	D3DCreateBlob(size,&shaderBlob);
	memcpy(shaderBlob->GetBufferPointer(),buf,size);

	free(buf);
	fin.close();
	return shaderBlob;   
}

bool RevEffect::SaveShaderToBinaryFile(	
									   const char* aFilePath,
									   ID3D10Blob* aShaderBlob )
{
	if(aShaderBlob == NULL)
	{
		assert( 0 && "Send invalid d3d blob to binary file");
		return false;
	}
	int size;
	std::ofstream fout;			
	fout.open(aFilePath,std::ios::binary);
	size=aShaderBlob->GetBufferSize();
	fout.write((char*)&size,sizeof(int));
	fout.write((char*)aShaderBlob->GetBufferPointer(),size);
	fout.close();	
	return true;
}

