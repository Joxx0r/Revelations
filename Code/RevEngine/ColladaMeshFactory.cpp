#include "stdafx.h"
#include "ColladaMeshFactory.h"

#include "FCollada.h"
#include "RevTransformationCurveNode.h"
#include "RevTransformationHierachyNode.h"
#include "RevTransformationNode.h"
#include "RevAnimation.h"

#include "FCDocument\FCDocument.h"
#include "FCDocument\FCDLibrary.h"
#include "FCDocument\FCDGeometryMesh.h"
#include "FCDocument\FCDGeometry.h"
#include "FCDocument\FCDCamera.h"
#include "FCDocument\FCDMaterial.h"

#include "FCDocument\FCDGeometryPolygons.h"
#include "FCDocument\FCDGeometrySource.h"
#include "FCDocument\FCDSceneNode.h" 
#include "FCDocument\FCDLight.h" 
#include "FUtils\FUObject.h" 
#include "FCDocument\FCDControllerInstance.h"
#include "FCDocument\FCDController.h"
#include "FCDocument\FCDSkinController.h"

#include "FCDocument\FCDGeometryInstance.h"
#include "FCDocument\FCDMaterial.h" 
#include "FCDocument\FCDMaterialInstance.h"
#include "FCDocument\FCDLibrary.h"
#include "FCDocument\FCDAnimation.h"
#include "FCDocument\FCDAnimationChannel.h"
#include "FCDocument\FCDAnimationCurve.h"
#include "FCDocument\FCDGeometryPolygonsTools.h"
#include "FCDocument\FCDGeometryPolygonsInput.h"
#include "FCDocument\FCDMaterial.h"
#include "FCDocument\FCDEffect.h"
#include "FCDocument\FCDEffectProfile.h"
#include "FCDocument\FCDEffectStandard.h"
#include "FCDocument\FCDTexture.h"
#include "FCDocument\FCDImage.h"
#include "FCDocument\FCDAnimationCurve.h"
#include "FCDocument\FCDAnimated.h"
#include "FCDocument\FCDAnimation.h"
#include "FCDocument\FCDAnimationKey.h"

#include "RevModel.h"
#include "RevTextures.h"
#include "RevEngineMain.h"
#include "RevEffect.h"
#include "RevStructures.h"
#include "RevRoom.h"

#include <map>
#include <vector>
#include <assert.h>
#include <float.h>
#include <limits>

ColladaMeshFactory::ColladaMeshFactory(void)
{
	FCollada::Initialize();
}


ColladaMeshFactory::~ColladaMeshFactory(void)
{
}

RevModel* ColladaMeshFactory::LoadModel( const char* aFilePath )
{
	RevModel* model = new RevModel();
	model->SetModelFilePath(aFilePath);
	if( LoadModelWithFilePath( &model, aFilePath ) == false )
	{
		delete model;
		model = NULL;
	}
	return model;
}

void ColladaMeshFactory::LoadRooms( const char* aFilePath, std::vector< RevRoom* >& aRefContainer )
{
	LoadRoomFromFilePath( aFilePath, aRefContainer );
}

bool ColladaMeshFactory::LoadModelWithFilePath( RevModel** aModel, const char* aFilePath )
{

	FCDocument* colladDoc;

	colladDoc=FCollada::NewTopDocument();
	bool sucess=FCollada::LoadDocumentFromFile(colladDoc, aFilePath );	

	if(sucess==false) return(false);
	if(colladDoc==NULL) return(false);

	FCDGeometryLibrary* geometryLibrary=colladDoc->GetGeometryLibrary();

	unsigned int numberOfMeshes=0;
	for (unsigned int i=0; i< geometryLibrary->GetEntityCount(); i++)
	{
		if (geometryLibrary->GetEntity(i)->IsMesh()==true) 
		{
			numberOfMeshes++;
		}
	}

	BuildHierarchy(colladDoc,aModel, colladDoc->GetVisualSceneInstance());


	CU::Matrix44f mayaAdapter,temp;

	mayaAdapter.myMatrix[0][0]=-1;
	(*aModel)->SetBaseModelMatrix( mayaAdapter );
	//aModelNode->myTransformationNode->SetOrientation(temp);
	return(true);

}

void ColladaMeshFactory::BuildHierarchy(FCDocument* aColladaDocument,RevModel** aModelNode,  FCDSceneNode* aSceneNode)
{
	FCDSceneNode* sceneRoot=aSceneNode;
	assert(sceneRoot!=NULL); 

	assert(sceneRoot->GetTransformCount()==0);
	assert(sceneRoot->GetType()==FCDEntity::SCENE_NODE);

	assert(sceneRoot->GetInstanceCount()<2);

	if(sceneRoot->GetInstanceCount()==0)
	{
		//		aModelNode->myIsNULLObject=true;
		//		aModel->myOrientation.myMatrix[0][0]=-1;
		//		aModel->myOrientation.myMatrix[2][2]=-1;
	}
	else
	{

		FCDEntity::Type t = sceneRoot->GetInstance(0)->GetEntityType();

		bool isGeometry = t == FCDEntity::GEOMETRY || FCDEntity::CONTROLLER ? true : false;
		if( isGeometry )
		{
			bool haveBones = t == FCDEntity::CONTROLLER ? true : false;
			if( haveBones )
			{
				(*aModelNode)->SetModelType( RevModel::_ANIMATED );
			}
			else
			{
				(*aModelNode)->SetModelType( RevModel::_NORMAL );
			}

			FindAndDecodeInstance(aModelNode,sceneRoot->GetInstance(0),aColladaDocument,haveBones);
			if( haveBones )
			{
			
				FCDControllerLibrary* controllerLibrary=aColladaDocument->GetControllerLibrary();

				for (unsigned int i=0; i< controllerLibrary->GetEntityCount(); i++)
				{
					if(controllerLibrary->GetEntity(i)->GetDaeId()==aSceneNode->GetInstance(0)->GetEntity()->GetDaeId())
					{
						BuildBoneHierarchy(*aModelNode,aColladaDocument,aColladaDocument->GetVisualSceneInstance(),-1,dynamic_cast<FCDController*>(controllerLibrary->GetEntity(i)),dynamic_cast<FCDControllerInstance*>(aSceneNode->GetInstance(0)), false);
					}
				}
			}
		}
	}

	for(unsigned int childIndex=0;childIndex<sceneRoot->GetChildrenCount();childIndex++)
	{
		FCDSceneNode* childNode;
		RevModel * childModelNode= new RevModel();
		childNode=sceneRoot->GetChild(childIndex);
		childModelNode->SetModelFilePath((*aModelNode)->GetModelFilePath());
		//	childModelNode->SetTransformation(GetTransForm(childNode));
		(*aModelNode)->AddChild(childModelNode);
		//	(aColladaDocument,childNode,childModelNode);
		BuildHierarchy(aColladaDocument,&childModelNode, childNode);
	}
}


void ColladaMeshFactory::FindAndDecodeInstance(RevModel** aModel,FCDEntityInstance* aInstance,FCDocument* aColladaDocument,bool aHasBoneWeightsFlag)
{
	FCDGeometryLibrary* geometryLibrary=aColladaDocument->GetGeometryLibrary();
	FCDControllerLibrary* controllerLibrary=aColladaDocument->GetControllerLibrary();

	for (unsigned int i=0; i< geometryLibrary->GetEntityCount(); i++)
	{
		if (geometryLibrary->GetEntity(i)->IsMesh()==true) 
		{
			if(geometryLibrary->GetEntity(i)->GetDaeId()==aInstance->GetEntity()->GetDaeId())
			{
				DecodeInstance(aModel,geometryLibrary->GetEntity(i),dynamic_cast<FCDGeometryInstance*>(aInstance),aColladaDocument,false,NULL,NULL);
				// GET READY TO LOAD
				//SelectEffect(aModel);

			}
		}
	}
	for (unsigned int i=0; i< controllerLibrary->GetEntityCount(); i++)
	{
		if(controllerLibrary->GetEntity(i)->GetDaeId()==aInstance->GetEntity()->GetDaeId())
		{
			for (unsigned int j=0; j< geometryLibrary->GetEntityCount(); j++)
			{
				if(geometryLibrary->GetEntity(j)->GetDaeId()==controllerLibrary->GetEntity(i)->GetBaseTarget()->GetDaeId())
				{
					DecodeInstance(aModel,geometryLibrary->GetEntity(j),dynamic_cast<FCDGeometryInstance*>(aInstance),aColladaDocument,true,dynamic_cast<FCDController*>(controllerLibrary->GetEntity(i)),dynamic_cast<FCDControllerInstance*>(aInstance));
					// GET READY TO LOAD
					//SelectEffect(aModel);

				}
			}
		}
	}
}
bool operator<(const RevVertexIndexmapKey& aLeftKey,const RevVertexIndexmapKey& aRightKey)
{
	for(int i=0;i<8;i++)
	{
		if(aLeftKey.myKeys[i]<aRightKey.myKeys[i])
		{
			return(true);
		}
		else
		{
			if(aLeftKey.myKeys[i]>aRightKey.myKeys[i])
			{
				return(false);
			}
		}
	}
	return(false);
}

void ColladaMeshFactory::DecodeInstance(RevModel** aModel,FCDGeometry* aGeometry,FCDGeometryInstance* aGeometryInstance,FCDocument* aColladaDocument,bool aHasBoneWeightsFlag,FCDController* aController,FCDControllerInstance* aControllerInstance)
{
	FCDGeometryMesh* mesh=aGeometry->GetMesh();
	if (!mesh->IsTriangles()) FCDGeometryPolygonsTools::Triangulate(mesh);

	FCDGeometrySource* positionSource=mesh->FindSourceByType(FUDaeGeometryInput::POSITION);
	FCDGeometrySource* normalSource=mesh->FindSourceByType(FUDaeGeometryInput::NORMAL);
	FCDGeometrySource* geoTangentSource=mesh->FindSourceByType(FUDaeGeometryInput::GEOTANGENT);
	FCDGeometrySource* texTangentSource=mesh->FindSourceByType(FUDaeGeometryInput::TEXTANGENT);
	FCDGeometrySource* colorSource=mesh->FindSourceByType(FUDaeGeometryInput::COLOR);
	FCDGeometrySource* textureCordinatesSource=mesh->FindSourceByType(FUDaeGeometryInput::TEXCOORD);
	FCDGeometrySource* geoBiNormalSource=mesh->FindSourceByType(FUDaeGeometryInput::GEOBINORMAL);
	FCDGeometrySource* texBiNormalSource=mesh->FindSourceByType(FUDaeGeometryInput::TEXBINORMAL);

	assert(positionSource);


	int elementCount=0;
	int totalStride;
	int positionStride;
	int normalStride;
	int colorStride;
	int textureCordinatesStride;
	int tangentStride;


	RevInputData data;

	if(positionSource!=NULL)
	{
		D3D11_INPUT_ELEMENT_DESC element= { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		//(*aModel)->AddModelInputElement(element);
		positionStride=elementCount;
		data.myElements.push_back( element );

		elementCount+=12;
	}

	if(aHasBoneWeightsFlag)
	{
		D3D11_INPUT_ELEMENT_DESC element={ "WEIGHTS", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0,      elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element );
		elementCount+=4;
		D3D11_INPUT_ELEMENT_DESC element2={ "BONES", 0, DXGI_FORMAT_R8G8B8A8_UINT, 0,         elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element2 );
		elementCount+=4;
	}

	if(normalSource!=NULL)
	{
		D3D11_INPUT_ELEMENT_DESC element= { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element );
		normalStride=elementCount;
		elementCount+=12;
	}
	if(colorSource!=NULL)
	{
		D3D11_INPUT_ELEMENT_DESC element= { "COLOR",  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element );
		colorStride=elementCount;

		elementCount+=16;
	}
	if(textureCordinatesSource!=NULL)
	{
		D3D11_INPUT_ELEMENT_DESC element= { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT, 0, elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element );
		textureCordinatesStride=elementCount;
		elementCount+=8;
	}

	//assert(((geoTangentSource!=NULL) && (texTangentSource!=NULL))==false);

	if(geoTangentSource!=NULL)
	{
		D3D11_INPUT_ELEMENT_DESC element= { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element );
		tangentStride=elementCount;
		elementCount+=12;
	}
	if(texTangentSource!=NULL)
	{
		D3D11_INPUT_ELEMENT_DESC element= { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element );
		tangentStride=elementCount;
		elementCount+=12;
	}

	if(geoBiNormalSource!=NULL)
	{
		D3D11_INPUT_ELEMENT_DESC element= { "BINORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element );
		tangentStride=elementCount;
		elementCount+=12;
	}
	if(texBiNormalSource!=NULL)
	{
		D3D11_INPUT_ELEMENT_DESC element= { "BINORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, elementCount, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		data.myElements.push_back( element );
		tangentStride=elementCount;
		elementCount+=12;
	}

	for( auto element : data.myElements )
	{
		data.m_instancedElements.push_back( element );
	}

	D3D11_INPUT_ELEMENT_DESC elementOne		= { "WORLD",		0,	DXGI_FORMAT_R32G32B32A32_FLOAT,	1,		0,		D3D11_INPUT_PER_INSTANCE_DATA, 1	};
	D3D11_INPUT_ELEMENT_DESC elementTwo		= { "WORLD",		1,	DXGI_FORMAT_R32G32B32A32_FLOAT,	1,		16,		D3D11_INPUT_PER_INSTANCE_DATA, 1	};
	D3D11_INPUT_ELEMENT_DESC elementThree	= { "WORLD",		2,	DXGI_FORMAT_R32G32B32A32_FLOAT,	1,		32,		D3D11_INPUT_PER_INSTANCE_DATA, 1	};
	
	data.m_instancedElements.push_back( elementOne );
	data.m_instancedElements.push_back( elementTwo );
	data.m_instancedElements.push_back( elementThree );

	totalStride=elementCount;
		
	RevVertexDataNormal vbData( 0 );
	DetermineVertexType( vbData, data );

//	vbData.mySize = vbData.myStride * vbData.myNrOfVertexes;
//	vbData.myVertexData = new char[ vbData.mySize ];

	RevIndexData indexData;

	GenerateMeshDatafromPolygonsFull(aModel,indexData,vbData,data,aGeometryInstance,mesh, aColladaDocument, aHasBoneWeightsFlag, aController, aControllerInstance );
	
}

bool ColladaMeshFactory::DetermineVertexType(	RevVertexDataNormal& aVertexData,
											 RevInputData& aInputData )
{
	if(strcmp(aInputData.myElements[0].SemanticName,"POSITION")!=0)  return(false);
	if(aInputData.myElements[0].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);
	if(aInputData.myElements.size()==2)
	{
		if(strcmp(aInputData.myElements[1].SemanticName,"TEXCOORD")!=0)  return(false);
		if(aInputData.myElements[1].Format!=DXGI_FORMAT_R32G32_FLOAT) return(false);
		aVertexData.myStride=sizeof(RevVertexPosUV);;
		return(true);
	}
	if(aInputData.myElements.size()==3)
	{
		if(strcmp(aInputData.myElements[2].SemanticName,"TEXCOORD")!=0)  return(false);
		if(aInputData.myElements[2].Format!=DXGI_FORMAT_R32G32_FLOAT) return(false);


		if(strcmp(aInputData.myElements[1].SemanticName,"NORMAL")==0)
		{
			if(aInputData.myElements[1].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);
			aVertexData.myStride=sizeof(RevVertexPosUVNormal);;
			return(true);
		}
		else
			if(strcmp(aInputData.myElements[1].SemanticName,"COLOR")==0)
			{
				if(aInputData.myElements[1].Format!=DXGI_FORMAT_R32G32B32A32_FLOAT) return(false);
				aVertexData.myStride=sizeof(RevVertexPosColorUV);;
				return(true);
			}
	}
	if(aInputData.myElements.size()==4)
	{
		if(strcmp(aInputData.myElements[1].SemanticName,"NORMAL")!=0) return(false);
		if(aInputData.myElements[1].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

		if(strcmp(aInputData.myElements[2].SemanticName,"TEXCOORD")!=0)  return(false);
		if(aInputData.myElements[2].Format!=DXGI_FORMAT_R32G32_FLOAT) return(false);

		if(strcmp(aInputData.myElements[3].SemanticName,"TANGENT")!=0)  return(false);
		if(aInputData.myElements[3].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

		aVertexData.myStride=sizeof(RevVertexPosNormUVTan);
		return(true);

	}
	if(aInputData.myElements.size()==5)
	{
		if(strcmp(aInputData.myElements[1].SemanticName,"NORMAL")==0)
		{
			if(aInputData.myElements[1].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

			if(strcmp(aInputData.myElements[2].SemanticName,"TEXCOORD")!=0)  return(false);
			if(aInputData.myElements[2].Format!=DXGI_FORMAT_R32G32_FLOAT) return(false);

			if(strcmp(aInputData.myElements[3].SemanticName,"TANGENT")!=0)  return(false);
			if(aInputData.myElements[3].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

			if(strcmp(aInputData.myElements[4].SemanticName,"BINORMAL")!=0)  return(false);
			if(aInputData.myElements[4].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

			aVertexData.myStride=sizeof(RevVertexPosNormUVTanBi);
			return(true);
		}

		if(strcmp(aInputData.myElements[1].SemanticName,"WEIGHTS")!=0) return(false);
		if(aInputData.myElements[1].Format!=DXGI_FORMAT_R8G8B8A8_UNORM) return(false);
		if(strcmp(aInputData.myElements[2].SemanticName,"BONES")!=0) return(false);
		if(aInputData.myElements[2].Format!=DXGI_FORMAT_R8G8B8A8_UINT) return(false);
		if(strcmp(aInputData.myElements[3].SemanticName,"NORMAL")!=0) return(false);
		if(aInputData.myElements[3].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

		if(strcmp(aInputData.myElements[4].SemanticName,"TEXCOORD")!=0)  return(false);
		if(aInputData.myElements[4].Format!=DXGI_FORMAT_R32G32_FLOAT) return(false);

		aVertexData.myStride=sizeof( RevVertexPosWeightBonesNormUV ) ;
		return(true);
	}


	if(aInputData.myElements.size()==6)
	{

		if(strcmp(aInputData.myElements[1].SemanticName,"WEIGHTS")!=0) return(false);
		if(aInputData.myElements[1].Format!=DXGI_FORMAT_R8G8B8A8_UNORM) return(false);
		if(strcmp(aInputData.myElements[2].SemanticName,"BONES")!=0) return(false);
		if(aInputData.myElements[2].Format!=DXGI_FORMAT_R8G8B8A8_UINT) return(false);
		if(strcmp(aInputData.myElements[3].SemanticName,"NORMAL")!=0) return(false);
		if(aInputData.myElements[3].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

		if(strcmp(aInputData.myElements[4].SemanticName,"TEXCOORD")!=0)  return(false);
		if(aInputData.myElements[4].Format!=DXGI_FORMAT_R32G32_FLOAT) return(false);

		if(strcmp(aInputData.myElements[5].SemanticName,"TANGENT")!=0)  return(false);
		if(aInputData.myElements[5].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

		aVertexData.myStride=sizeof(RevVertexPosWeightBonesNormUVTan);
		return(true);

	}
	if(aInputData.myElements.size()==7)
	{

		if(strcmp(aInputData.myElements[1].SemanticName,"WEIGHTS")!=0) return(false);
		if(aInputData.myElements[1].Format!=DXGI_FORMAT_R8G8B8A8_UNORM) return(false);
		if(strcmp(aInputData.myElements[2].SemanticName,"BONES")!=0) return(false);
		if(aInputData.myElements[2].Format!=DXGI_FORMAT_R8G8B8A8_UINT) return(false);
		if(strcmp(aInputData.myElements[3].SemanticName,"NORMAL")!=0) return(false);
		if(aInputData.myElements[3].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

		if(strcmp(aInputData.myElements[4].SemanticName,"TEXCOORD")!=0)  return(false);
		if(aInputData.myElements[4].Format!=DXGI_FORMAT_R32G32_FLOAT) return(false);

		if(strcmp(aInputData.myElements[5].SemanticName,"TANGENT")!=0)  return(false);
		if(aInputData.myElements[5].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);

		if(strcmp(aInputData.myElements[6].SemanticName,"BINORMAL")!=0)  return(false);
		if(aInputData.myElements[6].Format!=DXGI_FORMAT_R32G32B32_FLOAT) return(false);
		aVertexData.myStride=sizeof(RevVertexPosWeightBonesNormUVTanBi);
		return(true);

	}

	return(false);
}

void ColladaMeshFactory::GenerateMeshDatafromPolygonsFull(	
	RevModel** aModel,
	RevIndexData& aIndexData,
	RevVertexDataNormal& aVertexData,
	RevInputData& aInputData,
	FCDGeometryInstance* aInstance,
	FCDGeometryMesh* aMesh,
	FCDocument* aColladaDocument,
	bool aHasBoneWeightsFlag,
	FCDController* aController,
	FCDControllerInstance* aControllerInstance)
{
	int polygonalMeshes= static_cast<int>(aMesh->GetPolygonsCount());

	//	assert(polygonalMeshes==1); // how to handle multiple meshes ? handle later

	// To do add support for multiple meshes ?

	FCDSkinController* skinControl=NULL;
	if(aHasBoneWeightsFlag==true)
	{
		skinControl=aController->GetSkinController();
		skinControl->ReduceInfluences(4);
	}

	int myVertexCount=0;
	int duplicatesCount=0;
	int currentIndex=0;

	std::map<RevVertexIndexmapKey,int> vertexList;
	std::map<RevVertexIndexmapKey,int>::iterator vertexIterator;
	std::vector<RevVertexIndexmapKey> vertexes;

	aIndexData.myNrOfIndexes =0;
	aIndexData.myFormat =DXGI_FORMAT_R32_UINT;
	for(int polygonMeshIndex=0;polygonMeshIndex<static_cast<int>(polygonalMeshes);polygonMeshIndex++)
	{
		FCDGeometryPolygons* somePolygons=aMesh->GetPolygons(polygonMeshIndex);
		FCDGeometryPolygonsInput* positionInput=somePolygons->FindInput(FUDaeGeometryInput::POSITION);
		aIndexData.myNrOfIndexes+=positionInput->GetIndexCount();

	}

	aIndexData.mySize=aIndexData.myNrOfIndexes*sizeof(unsigned int);

	aIndexData.myIndexData = new char [aIndexData.mySize] ();
	UINT* indexArray=reinterpret_cast<unsigned int*>(aIndexData.myIndexData);

	FCDGeometrySource* positionSource=aMesh->FindSourceByType(FUDaeGeometryInput::POSITION);
	FCDGeometrySource* normalSource=aMesh->FindSourceByType(FUDaeGeometryInput::NORMAL);
	FCDGeometrySource* colorSource=aMesh->FindSourceByType(FUDaeGeometryInput::COLOR);
	FCDGeometrySource* textureCordinatesSource=aMesh->FindSourceByType(FUDaeGeometryInput::TEXCOORD);
	FCDGeometrySource* geoTangentSource=aMesh->FindSourceByType(FUDaeGeometryInput::GEOTANGENT);
	FCDGeometrySource* texTangentSource=aMesh->FindSourceByType(FUDaeGeometryInput::TEXTANGENT);
	FCDGeometrySource* geoBiNormalSource=aMesh->FindSourceByType(FUDaeGeometryInput::GEOBINORMAL);
	FCDGeometrySource* texBiNormalSource=aMesh->FindSourceByType(FUDaeGeometryInput::TEXBINORMAL);


	RevModelRenderEssentials renderEssentials( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	SelectEffectOnModel(aModel, aInputData);

	RevTextures tex;

	for(int polygonMeshIndex=0;polygonMeshIndex<polygonalMeshes;polygonMeshIndex++)
	{	
		(*aModel)->SetIsNullObjectFlag( false );
		FCDGeometryPolygons* somePolygons=aMesh->GetPolygons(polygonMeshIndex);

		FCDGeometryPolygonsInput* positionInput=somePolygons->FindInput(FUDaeGeometryInput::POSITION);
		FCDGeometryPolygonsInput* normalInput=somePolygons->FindInput(FUDaeGeometryInput::NORMAL);
		FCDGeometryPolygonsInput* colorInput=somePolygons->FindInput(FUDaeGeometryInput::COLOR);
		FCDGeometryPolygonsInput* texCoordInput=somePolygons->FindInput(FUDaeGeometryInput::TEXCOORD);
		FCDGeometryPolygonsInput* geoTangentInput=somePolygons->FindInput(FUDaeGeometryInput::GEOTANGENT);
		FCDGeometryPolygonsInput* texTangentInput=somePolygons->FindInput(FUDaeGeometryInput::TEXTANGENT);
		FCDGeometryPolygonsInput* geoBiNormalInput=somePolygons->FindInput(FUDaeGeometryInput::GEOBINORMAL);
		FCDGeometryPolygonsInput* texBiNormalInput=somePolygons->FindInput(FUDaeGeometryInput::TEXBINORMAL);

		somePolygons->GetFaceVertexCount();


		renderEssentials.myVertexStart = 0;
		renderEssentials.myIndexStart = currentIndex;
		renderEssentials.myIndexCount = positionInput->GetIndexCount();

		for(int i=0;i<static_cast<int>(positionInput->GetIndexCount());i++)
		{
			RevVertexIndexmapKey key;
			if(positionInput!=NULL)
			{
				key.myKeys[0]=positionInput->GetIndices()[i];
			}
			if(normalInput!=NULL)
			{
				key.myKeys[1]=normalInput->GetIndices()[i];
			}
			if(colorInput!=NULL)
			{
				key.myKeys[2]=colorInput->GetIndices()[i];
			}
			if(texCoordInput!=NULL)
			{
				key.myKeys[3]=texCoordInput->GetIndices()[i];
			}
			if(geoTangentInput!=NULL)
			{
				key.myKeys[4]=geoTangentInput->GetIndices()[i];
			}
			if(texTangentInput!=NULL)
			{
				key.myKeys[5]=texTangentInput->GetIndices()[i];
			}

			if(geoBiNormalInput!=NULL)
			{
				key.myKeys[6]=geoBiNormalInput->GetIndices()[i];
			}

			if(texBiNormalInput!=NULL)
			{
				key.myKeys[7]=texBiNormalInput->GetIndices()[i];
			}


			vertexIterator=vertexList.find(key);
			if(vertexIterator==vertexList.end())
			{
				indexArray[currentIndex]=myVertexCount;
				std::pair<RevVertexIndexmapKey,int> tempPair(key,myVertexCount);
				vertexList.insert(tempPair);
				vertexes.push_back(key);
				myVertexCount++;
				// create indexes
			}
			else
			{
				indexArray[currentIndex]=(*vertexIterator).second;
				duplicatesCount++;
				// Read Created Vertex
			}
			currentIndex++;
		}

		for (unsigned int k=0; k<aInstance->GetMaterialInstanceCount(); k++) 
		{
			FCDMaterialInstance* materialInstance;
			materialInstance=aInstance->GetMaterialInstance(k);
			if(aMesh->GetPolygons(polygonMeshIndex)->GetMaterialSemantic()==materialInstance->GetSemantic())
			{
				LoadMaterialNew(polygonMeshIndex,materialInstance->GetMaterial(),aColladaDocument, tex);
			}
		}
	}

	(*aModel)->SetModelTextures( tex );


	aVertexData.myNrOfVertexes=myVertexCount;
	aVertexData.mySize=aVertexData.myStride*aVertexData.myNrOfVertexes;
	aVertexData.myVertexData =new char [aVertexData.mySize] ();

	for(int i=0;i<aIndexData.myNrOfIndexes;i+=3)
	{
		unsigned int temp;	
		temp=indexArray[i+0];
		indexArray[i+0]=indexArray[i+2];
		indexArray[i+2]=temp;
	}

	myVertexCount=0;
	for(int i=0;i<static_cast<int>(vertexes.size());i++)
	{
		RevVertexIndexmapKey key=vertexes[i];

		int offset=0;
		std::pair<RevVertexIndexmapKey,int> tempPair(key,myVertexCount);
		vertexList.insert(tempPair);

		// Create Vertex
		if(key.myKeys[0]!=-1)
		{
			CU::Vector3f pos;
			memcpy(&pos,
				&positionSource->GetData()[key.myKeys[0]*positionSource->GetStride()],12);

			//			pos.y=-pos.y;
			//			pos.z=-pos.z;
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],
				&pos,12);

			offset+=12;
		}
		if(aHasBoneWeightsFlag==true)
		{
			assert(key.myKeys[0]!=-1);
			FCDSkinControllerVertex* vertexsInfluence;

			vertexsInfluence=skinControl->GetVertexInfluence(key.myKeys[0]);

			int boneData=0;
			int weightData=0;

			assert(vertexsInfluence->GetPairCount()<=4);
			for(int j=0;j<static_cast<int>(vertexsInfluence->GetPairCount());j++)
			{
				assert(j<4);
				FCDJointWeightPair* pair=vertexsInfluence->GetPair(j);

				assert(pair->jointIndex<256);
				boneData |= pair->jointIndex << (j*8);

				int weight=static_cast<int>((pair->weight*255.0f+0.5));
				assert(weight>=0);
				assert(weight<=255);
				weightData |= weight << (j*8);
			}
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],&weightData,4);
			offset+=4;
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],&boneData,4);
			offset+=4;

		}
		if(key.myKeys[1]!=-1)
		{
			CU::Vector3f normal;
			memcpy(&normal,
				&normalSource->GetData()[key.myKeys[1]*normalSource->GetStride()],12);

			//			normal.y=-normal.y;
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],
				&normal,12);
			offset+=12;
		}
		if(key.myKeys[2]!=-1)
		{
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],
				&colorSource->GetData()[key.myKeys[2]*colorSource->GetStride()],16);
			offset+=16;
		}
		if(key.myKeys[3]!=-1)
		{
			CU::Vector2f UV;
			memcpy(&UV,
				&textureCordinatesSource->GetData()[key.myKeys[3]*textureCordinatesSource->GetStride()],8);

			UV.v=1.0f-UV.v;




			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],
				&UV,8);
			offset+=8;
		}
		if(key.myKeys[4]!=-1)
		{
			CU::Vector3f tangent;
			memcpy(&tangent,
				&geoTangentSource->GetData()[key.myKeys[4]*geoTangentSource->GetStride()],12);
			//			tangent.y=-tangent.y;
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],
				&tangent,12);
			offset+=12;
		}
		if(key.myKeys[5]!=-1)
		{
			CU::Vector3f tangent;
			memcpy(&tangent,
				&texTangentSource->GetData()[key.myKeys[5]*texTangentSource->GetStride()],12);
			//			tangent.y=-tangent.y;
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],
				&tangent,12);
			offset+=12;
		}

		if(key.myKeys[6]!=-1)
		{
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],
				&geoBiNormalSource->GetData()[key.myKeys[6]*geoBiNormalSource->GetStride()],12);
			offset+=12;
		}
		if(key.myKeys[7]!=-1)
		{
			memcpy(&aVertexData.myVertexData[myVertexCount*aVertexData.myStride+offset],
				&texBiNormalSource->GetData()[key.myKeys[7]*texBiNormalSource->GetStride()],12);
			offset+=12;
		}

		myVertexCount++;
	}
	if( (*aModel)->Init( aVertexData, aIndexData, aInputData, renderEssentials ) ==false)
	{
		assert(0 && "Something went wrong in trying to init the current model");
	}
}

void ColladaMeshFactory::LoadMaterialNew(
	int aSurfaceIndex,
	FCDMaterial* aMaterial,
	FCDocument* aDocument,
	RevTextures& aTextures)
{
	FCDEffect* fx = aMaterial->GetEffect();
	FCDEffectProfile* profile = fx->FindProfile(FUDaeProfileType::COMMON);
	FCDEffectStandard* standardProfile=dynamic_cast<FCDEffectStandard*>(profile);

	if (standardProfile->GetTextureCount(FUDaeTextureChannel::DIFFUSE)>0) 
	{
		FCDTexture* texture=standardProfile->GetTexture(FUDaeTextureChannel::DIFFUSE,0);
		FCDImage* image=texture->GetImage();
		std::string fileName=image->GetFilename();
		aTextures.AddShaderResource( "g_DiffuseTex", fileName.c_str(), true ); 

	}

	if (standardProfile->GetTextureCount(FUDaeTextureChannel::BUMP)>0) 
	{
		FCDTexture* texture=standardProfile->GetTexture(FUDaeTextureChannel::BUMP,0);	
		FCDImage* image=texture->GetImage();
		std::string fileName=image->GetFilename();
		aTextures.AddShaderResource( "g_NormalTex", fileName.c_str(), false ); 

	}
	if (standardProfile->GetTextureCount(FUDaeTextureChannel::AMBIENT)>0) 
	{
		FCDTexture* texture=standardProfile->GetTexture(FUDaeTextureChannel::AMBIENT,0);
		FCDImage* image=texture->GetImage();
		std::string fileName=image->GetFilename();
		aTextures.AddShaderResource("g_PixelRoughnessAOEmissive", fileName.c_str(), true);
	}


	if (standardProfile->GetTextureCount(FUDaeTextureChannel::SPECULAR) > 0)
	{
		FCDTexture* texture = standardProfile->GetTexture(FUDaeTextureChannel::SPECULAR, 0);
		FCDImage* image = texture->GetImage();
		std::string fileName = image->GetFilename();
		aTextures.AddShaderResource("g_SubstanceTexture", fileName.c_str(), false);
	}
}


void ColladaMeshFactory::SelectEffectOnModel(	RevModel** aModel,
						 RevInputData& aInputData )
{
	if( (*aModel)->GetModelType() == RevModel::_NORMAL )
	{
		(*aModel)->InitShader( "Shaders\\Object_PosNormUvTanBi.txt" );
	}
	else
	{
		(*aModel)->InitShader( "Shaders\\Object_PosNormUvTanBiBones.txt" );
	}
}

void ColladaMeshFactory::LoadRoomFromFilePath( const char* aFilePath, std::vector< RevRoom* >& aRefContainer )
{
	FCDocument* colladDoc;

	colladDoc=FCollada::NewTopDocument();
	bool sucess=FCollada::LoadDocumentFromFile(colladDoc, aFilePath );	

	if(sucess==false) return;
	if(colladDoc==NULL) return;

	FCDGeometryLibrary* geometryLibrary=colladDoc->GetGeometryLibrary();

	unsigned int numberOfMeshes=0;
	for (unsigned int i=0; i< geometryLibrary->GetEntityCount(); i++)
	{
		if (geometryLibrary->GetEntity(i)->IsMesh()==true) 
		{
			numberOfMeshes++;
		}
	}


	
	
	BuildHierachyRoom(colladDoc,aRefContainer, colladDoc->GetVisualSceneInstance());

	//aModelNode->myTransformationNode->SetOrientation(temp);

}

void ColladaMeshFactory::BuildHierachyRoom( FCDocument* aColladaDocument, std::vector< RevRoom* >& aRefContainer, FCDSceneNode* aSceneNode )
{
	FCDSceneNode* sceneRoot=aSceneNode;//aColladaDocument->GetVisualSceneInstance();
	assert(sceneRoot!=NULL);
	assert(sceneRoot->GetType()==FCDEntity::SCENE_NODE);
	assert(sceneRoot->GetInstanceCount()<2);

	if(sceneRoot->GetInstanceCount()==0)
	{
	}
	else
	{
		if(sceneRoot->GetInstance(0)->GetEntityType()==FCDEntity::LIGHT)
		{
		}
		else
			if(sceneRoot->GetInstance(0)->GetEntityType()==FCDEntity::CAMERA)
			{
			}
			else
				if(sceneRoot->GetInstance(0)->GetEntityType()==FCDEntity::GEOMETRY)
				{
					DecodeRoom( aRefContainer, sceneRoot->GetInstance(0),aColladaDocument,false );
				}
				else
				{
					assert(0 && "UNSUPPORTED INSTANCE TYPE");
				}

	}

	for(unsigned int childIndex=0;childIndex<sceneRoot->GetChildrenCount();childIndex++)
	{
		FCDSceneNode* childNode;
		childNode=sceneRoot->GetChild(childIndex);
		BuildHierachyRoom(aColladaDocument,aRefContainer, childNode);
	}


}

void ColladaMeshFactory::DecodeRoom(std::vector< RevRoom* >& aRefContainer,FCDEntityInstance* aInstance,FCDocument* aColladaDocument,bool)
{
	FCDGeometryLibrary* geometryLibrary=aColladaDocument->GetGeometryLibrary();
	FCDControllerLibrary* controllerLibrary=aColladaDocument->GetControllerLibrary();

	for (unsigned int i=0; i< geometryLibrary->GetEntityCount(); i++)
	{
		if (geometryLibrary->GetEntity(i)->IsMesh()==true) 
		{
			if(geometryLibrary->GetEntity(i)->GetDaeId()==aInstance->GetEntity()->GetDaeId())
			{
				DecodeRoomInstance(aRefContainer,geometryLibrary->GetEntity(i),dynamic_cast<FCDGeometryInstance*>(aInstance),aColladaDocument,false,NULL,NULL);


			}
		}
	}
	for (unsigned int i=0; i< controllerLibrary->GetEntityCount(); i++)
	{
		if(controllerLibrary->GetEntity(i)->GetDaeId()==aInstance->GetEntity()->GetDaeId())
		{
			for (unsigned int j=0; j< geometryLibrary->GetEntityCount(); j++)
			{
				if(geometryLibrary->GetEntity(j)->GetDaeId()==controllerLibrary->GetEntity(i)->GetBaseTarget()->GetDaeId())
				{
					DecodeRoomInstance(aRefContainer,geometryLibrary->GetEntity(j),dynamic_cast<FCDGeometryInstance*>(aInstance),aColladaDocument,true,dynamic_cast<FCDController*>(controllerLibrary->GetEntity(i)),dynamic_cast<FCDControllerInstance*>(aInstance));
	
				}
			}
		}
	}
}

void ColladaMeshFactory::DecodeRoomInstance( std::vector< RevRoom* >& aRefContainer,FCDGeometry* aGeometry,FCDGeometryInstance* aGeometryInstance,FCDocument* aColladaDocument,bool aHasBoneWeightsFlag,FCDController* aController,FCDControllerInstance* aControllerInstance)
{
	FCDGeometryMesh* mesh=aGeometry->GetMesh();
	if (!mesh->IsTriangles()) FCDGeometryPolygonsTools::Triangulate(mesh);

	FCDGeometrySource* positionSource=mesh->FindSourceByType(FUDaeGeometryInput::POSITION);
	FCDGeometrySource* normalSource=mesh->FindSourceByType(FUDaeGeometryInput::NORMAL);
	FCDGeometrySource* geoTangentSource=mesh->FindSourceByType(FUDaeGeometryInput::GEOTANGENT);
	FCDGeometrySource* texTangentSource=mesh->FindSourceByType(FUDaeGeometryInput::TEXTANGENT);
	FCDGeometrySource* colorSource=mesh->FindSourceByType(FUDaeGeometryInput::COLOR);
	FCDGeometrySource* textureCordinatesSource=mesh->FindSourceByType(FUDaeGeometryInput::TEXCOORD);
	FCDGeometrySource* geoBiNormalSource=mesh->FindSourceByType(FUDaeGeometryInput::GEOBINORMAL);
	FCDGeometrySource* texBiNormalSource=mesh->FindSourceByType(FUDaeGeometryInput::TEXBINORMAL);

	assert(positionSource);


	int elementCount=0;
	int totalStride;
	int positionStride;
	int normalStride;
	int colorStride;
	int textureCordinatesStride;
	int tangentStride;


	int myVertexCount=0;
	int polygonalMeshes= static_cast<int>(mesh->GetPolygonsCount());

	
	for(int polygonMeshIndex=0;polygonMeshIndex<polygonalMeshes;polygonMeshIndex++)
	{

		const float minValue = std::numeric_limits< float >::min();
		const float maxValue = std::numeric_limits< float >::max();

		CU::Vector3f min( maxValue, maxValue, maxValue);
		CU::Vector3f max( minValue, minValue, minValue );

		FCDGeometryPolygons* somePolygons=mesh->GetPolygons(polygonMeshIndex);
		FCDGeometryPolygonsInput* positionInput=somePolygons->FindInput(FUDaeGeometryInput::POSITION);
		const int nVertex = somePolygons->GetFaceVertexCount();
		const size_t n =  positionSource->GetDataCount() / 3; //Data count is represetnted by a individual byte. The position soruce uses 3 bytes for the position so therefore the divide to know the N.

		for(int i = 0; i < n; i++)
		{
			CU::Vector3f pos;
			memcpy(&pos,
				&positionSource->GetData()[ i *positionSource->GetStride() ],12);
			assert( !_isnan( pos.x ) ); 
			assert( !_isnan( pos.y ) ); 
			assert( !_isnan( pos.z ) ); 
			for(int elementIndex = 0; elementIndex < 3; elementIndex ++)
			{
				if( min[ elementIndex ] > pos[ elementIndex ] )
				{
					min[ elementIndex ] = pos[ elementIndex ];
				}
			
				if( max[ elementIndex ] < pos[ elementIndex ] )
				{
					max[ elementIndex ] = pos[ elementIndex ];
				}
			}
		}
		CU::Vector3f halfExtents = (max - min) * 0.5f;
		CU::Vector3f center = min + halfExtents;
		RevRoom* room = new RevRoom();
		room->Init(halfExtents, center );
		aRefContainer.push_back( room );
	}


}

bool ColladaMeshFactory::IsRealTransform(FCDTransform* aTransform)
{
	if(aTransform->IsAnimated())
	{
		return(true);
	}
	if(aTransform->GetType()==FCDTransform::ROTATION)
	{
		FCDTRotation* rotation=dynamic_cast<FCDTRotation*>(aTransform);
		if(rotation->GetAngle()!=0) 		return(true);
	}

	if(aTransform->GetType()==FCDTransform::TRANSLATION)
	{
		FCDTTranslation* translation=dynamic_cast<FCDTTranslation*>(aTransform);
		FMVector3 vec=translation->GetTranslation();
		if(vec.LengthSquared()!=0)
		{
			return(true);
		}
	}
	return(false);
}


RevTransformationNode* ColladaMeshFactory::GetTransForm(
		FCDSceneNode* aSceneNode)
{
	CU::Matrix44f transformation=CU::Matrix44f::Identity();

	RevTransformationNode* returnNode=NULL;
	RevTransformationNode* transformationNode=NULL;
	RevTransformationHierachyNode* transformationNodeHierarachy=NULL;
	int transformationFrames=0;
	for(unsigned int i=0;i<aSceneNode->GetTransformCount();i++)
	{
		FCDTransform* transform=aSceneNode->GetTransform(i);

		if(IsRealTransform(transform)==true)
		{
			if(transform->IsAnimated())
			{
				if((transformationFrames>0) || (returnNode!=NULL))
				{
					if(transformationNodeHierarachy==NULL)
					{
						transformationNodeHierarachy= new RevTransformationHierachyNode();
						if(returnNode!=NULL)
						{
							transformationNodeHierarachy->AddTransformation(returnNode);
						}
					}
					returnNode=transformationNodeHierarachy;
					if(transformationFrames>0)
					{
						transformationNode= new RevTransformationNode();
						transformationNode->SetTransformation(transformation);
						transformationNodeHierarachy->AddTransformation(transformationNode);
					}
				}

				if(transform->GetType()==FCDTransform::ROTATION)
				{
					if(transformationNodeHierarachy!=NULL)
					{
						transformationNodeHierarachy->AddTransformation(GetRotationTransformation(transform));
					}
					else
					{
						returnNode=GetRotationTransformation(transform);
					}
				}
				if(transform->GetType()==FCDTransform::TRANSLATION)
				{
					if(transformationNodeHierarachy!=NULL)
					{
						transformationNodeHierarachy->AddTransformation(GetTranslationTransform(transform));
					}
					else
					{
						returnNode=GetTranslationTransform(transform);
					}
				}
				
				transformation=CU::Matrix44f::Identity();
				transformationFrames=0;
			}
			else
			{
				if(transform->GetType()==FCDTransform::ROTATION)
				{
					ApplyTransformationRotation(transform,transformation);
					transformationFrames++;
				}

				if(transform->GetType()==FCDTransform::TRANSLATION)
				{
					ApplyTransformationTranslation(transform,transformation);
					transformationFrames++;
				}

			}
			if(transform->GetType()==FCDTransform::SCALE)
			{
				int i = 3;
				i;
				// TO DO LATER RESOLVE SCALING
				//	assert(0 && "SCALING NOT IMPLEMENTED");
			}
		}
	}
	if(transformationFrames>0)
	{
		if(returnNode==NULL)
		{
			transformationNode= new RevTransformationNode();
			transformationNode->SetTransformation(transformation);
			return(transformationNode);
		}
		if(transformationNodeHierarachy==NULL)
		{
			transformationNodeHierarachy= new RevTransformationHierachyNode();
			if(returnNode!=NULL)
			{
				transformationNodeHierarachy->AddTransformation(returnNode);
			}
		}
		returnNode=transformationNodeHierarachy;
		transformationNode= new RevTransformationNode();
		transformationNode->SetTransformation(transformation);
		transformationNodeHierarachy->AddTransformation(transformationNode);
	}
	if(returnNode==0)
	{
		transformationNode= new RevTransformationNode();
		transformationNode->SetTransformation(transformation);
		returnNode=transformationNode;
	}
	return(returnNode);
}

void ColladaMeshFactory::ApplyTransformationRotation(FCDTransform* aTransform,CU::Matrix44f& aOrientation)
{
	FCDTRotation* rotation=dynamic_cast<FCDTRotation*>(aTransform);
	CU::Matrix44f rotMat;
	rotMat.ConvertD3D(&rotation->ToMatrix().m[0][0]);

	rotMat*=aOrientation;
	aOrientation=rotMat;
	//	aOrientation*=rotMat;
}

void ColladaMeshFactory::ApplyTransformationTranslation(FCDTransform* aTransform,CU::Matrix44f& aOrientation)
{
	FCDTTranslation* rotation=dynamic_cast<FCDTTranslation*>(aTransform);
	CU::Matrix44f rotMat;
	rotMat.ConvertD3D(&rotation->ToMatrix().m[0][0]);


	rotMat*=aOrientation;
	aOrientation=rotMat;
	//aOrientation*=rotMat;
}

RevAnimationCurve* ColladaMeshFactory::CreateAnimationCurve(FCDAnimationCurve* aCurve)
{
	RevAnimationCurve* animCurve;
	int nrOfKeys= aCurve->GetKeyCount();

	float startTime=aCurve->GetKey(0)->input;
	float endTime=aCurve->GetKey(nrOfKeys-1)->input;

	float fps=120; // frecuency is 1/fps
	float currTime;
	currTime=startTime;

	animCurve = new RevAnimationCurve();
	while (currTime<endTime) 
	{
		RevKeyFrame temp;
		temp.m_time=currTime;
		temp.m_val=(aCurve->Evaluate(currTime));
		currTime+=1.0f/fps;
		animCurve->AddKeyFrame(temp);
	}
	RevKeyFrame temp;
	temp.m_time=endTime;
	temp.m_val=aCurve->Evaluate(endTime);
	animCurve->AddKeyFrame(temp);
	animCurve->FinalizeCurve();
	return(animCurve);
}


RevAnimationCurve* ColladaMeshFactory::CreateAnimationCurveRot(FCDAnimationCurve* aCurve)
{
	RevAnimationCurve* animCurve;
	int nrOfKeys= aCurve->GetKeyCount();

	float startTime=aCurve->GetKey(0)->input;
	float endTime=aCurve->GetKey(nrOfKeys-1)->input;

	float fps=20; // frecuency is 1/fps
	float currTime;
	currTime=startTime;

	animCurve = new RevAnimationCurve();
	while (currTime<endTime) 
	{
		RevKeyFrame temp;
		temp.m_time=currTime;
		temp.m_val=(aCurve->Evaluate(currTime))*3.149f/180.0f;
		currTime+=1.0f/fps;
		animCurve->AddKeyFrame(temp);
	}
	RevKeyFrame temp;
	temp.m_time=endTime;
	temp.m_val=aCurve->Evaluate(endTime);
	animCurve->AddKeyFrame(temp);
	animCurve->FinalizeCurve();
	return(animCurve);
}

void ColladaMeshFactory::BuildBoneHierarchy(RevModel* aModel,FCDocument* aColladaDocument, FCDSceneNode* aSceneNode,int aParentIndex,FCDController* aController, FCDControllerInstance* aControllerInstance, bool aHaveCreatedBonesFlag)
{
	RevBone tempBone;
	std::vector< RevBone >& bones = aModel->GetBones();
	std::vector< int >& boneList = aModel->GetBoneList();

	int boneIndex = bones.size();
	assert(aSceneNode!=NULL);
	assert(aSceneNode->GetType()==FCDEntity::SCENE_NODE);

	assert(aSceneNode->GetInstanceCount()<2);
	bool validBone=false;

	RevTransformationNode* testNode=NULL;

	if(aSceneNode->GetJointFlag()==true)
	{
		if(aModel->GetAnimation()==nullptr)
		{
			RevAnimation* animation = new RevAnimation();
			aModel->SetAnimation( animation );
			animation->Init(bones.size());
		}
		testNode=GetTransForm(aSceneNode);

		if(aHaveCreatedBonesFlag==false)
		{
			if(aParentIndex!=-1)
			{
				bones[aParentIndex].AddChild(boneIndex);
			}
			FCDSkinController* skinControl=NULL;
			skinControl=aController->GetSkinController();

			for(int i=0;i<static_cast<int>(skinControl->GetJointCount());i++)
			{
				if(skinControl->GetJoint(i)->GetId()==aSceneNode->GetDaeId())
				{
					assert(i==boneIndex);
					bones.push_back(tempBone);
					boneList.push_back(boneIndex);
					validBone=true;
					FMMatrix44 invBindPos= skinControl->GetJoint(boneIndex)->GetBindPoseInverse();
					FMMatrix44 bindPos= invBindPos.Inverted();
					CU::Matrix44f matrix;
					matrix.ConvertD3D(&bindPos.m[0][0]);
					bones[boneIndex].SetBindPose(matrix);
					FCDAnimationLibrary*  animationLibrary = aColladaDocument->GetAnimationLibrary();
					bool isAnimated=false;
					for(int animationIndex=0;
							animationIndex<static_cast<int>(animationLibrary->GetEntityCount());
							animationIndex++)
					{
						FCDEntity* entity=animationLibrary->GetEntity(animationIndex);
						fm::string name=entity->GetDaeId();
						name.erase(skinControl->GetJoint(i)->GetId().length(),name.length());
						if(name==skinControl->GetJoint(i)->GetId())
						{
							isAnimated=true;

						}
					}
					if(isAnimated==false)
					{
						if(aModel->GetAnimation()==NULL)
						{
							RevAnimation* anim = new RevAnimation();
							aModel->SetAnimation( anim );
							anim->Init( bones.size() );
						}
					}
				}
			}
			aModel->GetAnimation()->AddTransformationNode(testNode);
		}
	}
	if(bones.size()==0) boneIndex=-1;
	for(unsigned int childIndex=0;childIndex<aSceneNode->GetChildrenCount();childIndex++)
	{
		FCDSceneNode* childNode;
		childNode=aSceneNode->GetChild(childIndex);
		BuildBoneHierarchy(aModel,aColladaDocument,childNode,boneIndex,aController,aControllerInstance, false);
	}
}


RevTransformationCurveNode* ColladaMeshFactory::GetRotationTransformation(FCDTransform* aTransform)
{
	CU::Vector3f baseAxis(0,0,1);
	float baseAngle=0;

	assert(aTransform->IsAnimated());
	RevTransformationCurveNode* node=new RevTransformationCurveNode();

	FCDTRotation* rotation=dynamic_cast<FCDTRotation*>(aTransform);
	FMVector3 point=rotation->GetAxis();
	baseAngle=rotation->GetAngle();

	baseAxis.x=point.x;
	baseAxis.y=point.y;
	baseAxis.z=point.z;
	FCDAnimationCurve* rotXCurve;
	FCDAnimationCurve* rotYCurve;
	FCDAnimationCurve* rotZCurve;
	FCDAnimationCurve* rotAngleCurve;

	// look for x animation
	rotXCurve=rotation->GetAnimated()->FindCurve(".X");

	// look for y animation
	rotYCurve=rotation->GetAnimated()->FindCurve(".Y");

	// look for z animation
	rotZCurve=rotation->GetAnimated()->FindCurve(".Z");

	// look for angle animation
	rotAngleCurve=rotation->GetAnimated()->FindCurve(".ANGLE");

	if(rotXCurve!=NULL)			
	{
		node->SetRotationCurve(0,CreateAnimationCurve(rotXCurve));
	}
	if(rotYCurve!=NULL)			
	{
		node->SetRotationCurve(1,CreateAnimationCurve(rotYCurve));
	}
	if(rotZCurve!=NULL)			
	{
		node->SetRotationCurve(2,CreateAnimationCurve(rotZCurve));
	}
	if(rotAngleCurve!=NULL)			
	{
		node->SetRotationCurve(3,CreateAnimationCurveRot(rotAngleCurve));
	}
	node->SetBaseFrame(baseAxis,CU::Vector3f(0,0,0),baseAngle);
	return(node);
}

RevTransformationCurveNode* ColladaMeshFactory::GetTranslationTransform(FCDTransform* aTransform)
{
	CU::Vector3f baseTranslation(0,0,0);

	assert(aTransform->IsAnimated());
	RevTransformationCurveNode* node=new RevTransformationCurveNode();

	FCDTTranslation* translatation=dynamic_cast<FCDTTranslation*>(aTransform);

	FMVector3 point=translatation->GetTranslation();

	baseTranslation.x=point.x;
	baseTranslation.y=point.y;
	baseTranslation.z=point.z;
	FCDAnimationCurve* curve;

	// look for x animation
	curve=translatation->GetAnimated()->FindCurve(".X");
	if(curve!=NULL)			
	{
		node->SetTranslationCurve(0,CreateAnimationCurve(curve));
	}

	// look for y animation
	curve=translatation->GetAnimated()->FindCurve(".Y");
	if(curve!=NULL)			
	{
		node->SetTranslationCurve(1,CreateAnimationCurve(curve));
	}

	// look for z animation
	curve=translatation->GetAnimated()->FindCurve(".Z");
	if(curve!=NULL)			
	{
		node->SetTranslationCurve(2,CreateAnimationCurve(curve));
	}
	node->SetBaseFrame(CU::Vector3f(1,0,0),baseTranslation,0);
	return(node);
}