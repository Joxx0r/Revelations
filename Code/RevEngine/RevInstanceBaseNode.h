#ifndef _REV_INSTANCE_BASE_NODE_H
#define _REV_INSTANCE_BASE_NODE_H

#include "RevModel.h"

#include <vector>

class RevAnimationInstanceNode;
class RevBoneInstanceNode;

class RevInstanceBaseNode
{
public:
	RevInstanceBaseNode(void);
	virtual ~RevInstanceBaseNode(void);
	
	void Init( RevModel* model );

	void Update( const float& delta );

	void Render( CU::Matrix44f& parent );

	const CU::Matrix44f& GetBaseMatrix();
	const std::vector< RevBoneInstanceNode* >& GetBoneInstances();
	const RevModel::ModelType& GetType();
	const int& GetModelID();
	RevModel* GetModel();

private:

	void TransformFrame(int boneID ,const CU::Matrix44f& parentWorld );

	RevModel* m_model;

	RevAnimationInstanceNode* m_animInstance;

	std::vector< RevBoneInstanceNode* > m_boneInstances;
	std::vector< RevInstanceBaseNode* > m_childs;

	CU::Matrix44f		m_baseMatrix;
	RevModel::ModelType m_modelType;

};

#endif