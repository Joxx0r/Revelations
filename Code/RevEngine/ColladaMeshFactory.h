#ifndef _COLLDA_MESH_FACTORY_H
#define _COLLDA_MESH_FACTORY_H

#include <vector>
#include "CU_Matrix.h"

class RevModel;
class RevRoom;
class RevTextures;

class FCDocument;
class FCDEntityInstance;
class FCDGeometry;
class FCDGeometryInstance;
class FCDController;
class FCDControllerInstance;
class FCDMaterial;
class FCDGeometryMesh;
class FCDSceneNode;
class FCDTransform;
class FCDAnimationCurve;

struct RevVertexDataNormal;
struct RevIndexData;
struct RevInputData;

class RevBone;
class RevAnimationCurve;
class RevTransformationCurveNode;
class RevTransformationNode;

class ColladaMeshFactory
{
public:
	ColladaMeshFactory(void);
	~ColladaMeshFactory(void);

	RevModel* LoadModel( const char* aFilePath );
	void LoadRooms( const char* aFilePath, std::vector< RevRoom* >& aRefContainer );

private:

	bool LoadModelWithFilePath( RevModel** aModel, const char* aFilePath );
	void BuildHierarchy(FCDocument* aColladaDocument,RevModel** aModelNode, FCDSceneNode* aSceneNode);
	void FindAndDecodeInstance(RevModel** aModel,FCDEntityInstance* aInstance,FCDocument* aColladaDocument,bool aHasBoneWeightsFlag);
	void DecodeInstance(RevModel** aModel,FCDGeometry* aGeometry,FCDGeometryInstance* aGeometryInstance,FCDocument* aColladaDocument,bool aHasBoneWeightsFlag,FCDController* aController,FCDControllerInstance* aControllerInstance);
	void BuildBoneHierarchy(RevModel** aModel,FCDocument* aColladaDocument, FCDSceneNode* aSceneNode,int aParentIndex,FCDController* aController, FCDControllerInstance* aControllerInstance, bool aHaveCreatedBonesFlag);
	bool DetermineVertexType(	RevVertexDataNormal& aVertexData,
								RevInputData& aInputData );
	bool IsRealTransform(FCDTransform* aTransform);

	void GenerateMeshDatafromPolygonsFull(	RevModel** aModel,
											RevIndexData& aIndexData,
											RevVertexDataNormal	& aVertexData,
											RevInputData& aInputData,
											FCDGeometryInstance* aInstance,
											FCDGeometryMesh* aMesh,
											FCDocument* aColladaDocument,
											bool aHasBoneWeightsFlag,
											FCDController* aController,
											FCDControllerInstance* aControllerInstance);
	void LoadMaterialNew(
					int aSurfaceIndex,
					FCDMaterial* aMaterial,
					FCDocument* aDocument,
					RevTextures& aTextures);

	void SelectEffectOnModel(	RevModel** aModel,
								RevInputData& aInputData );

	void LoadRoomFromFilePath( const char* aFilePath, std::vector< RevRoom* >& aRefContainer );
	void BuildHierachyRoom( FCDocument* aColladaDocument, std::vector< RevRoom* >& aRefContainer,  FCDSceneNode* aSceneNode );
	void DecodeRoom(std::vector< RevRoom* >& aRefContainer,FCDEntityInstance* aInstance,FCDocument* aColladaDocument,bool);
	void DecodeRoomInstance( 
		std::vector< RevRoom* >& aRefContainer,
		FCDGeometry* aGeometry,
		FCDGeometryInstance* aGeometryInstance,
		FCDocument* aColladaDocument,
		bool aHasBoneWeightsFlag,
		FCDController* aController,
		FCDControllerInstance* aControllerInstance);

	void ApplyTransformationRotation(FCDTransform* aTransform,CU::Matrix44f& aOrientation);
	void ApplyTransformationTranslation(FCDTransform* aTransform,CU::Matrix44f& aOrientation);

	RevTransformationNode* GetTransForm(FCDSceneNode* aSceneNode);

	RevAnimationCurve* CreateAnimationCurve(FCDAnimationCurve* aCurve);
	RevAnimationCurve* CreateAnimationCurveRot(FCDAnimationCurve* aCurve);
	
	void BuildBoneHierarchy(RevModel* aModel,FCDocument* aColladaDocument, FCDSceneNode* aSceneNode,int aParentIndex,FCDController* aController, FCDControllerInstance* aControllerInstance, bool aHaveCreatedBonesFlag);

	RevTransformationCurveNode* GetRotationTransformation(FCDTransform* aTransform);
	RevTransformationCurveNode* GetTranslationTransform(FCDTransform* aTransform);

};

#endif