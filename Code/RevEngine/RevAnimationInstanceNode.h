#ifndef _REV_ANIMATION_INSTANCE_NODE_H
#define _REV_ANIMATION_INSTANCE_NODE_H

#include "CU_Matrix.h"

class RevTransformationNode;
class RevAnimation;

class RevAnimationInstanceNode
{
public:
	RevAnimationInstanceNode(void);
	virtual ~RevAnimationInstanceNode(void);
	void SetAnimation( RevAnimation* animation );
	void Update( const float& deltaTime );

	std::vector< CU::Matrix44f >& GetAnimationTransformedNodes();
	const CU::Matrix44f& GetBoneTransformation( const int& aBoneIndex );
private:
	std::vector< CU::Matrix44f > m_transformedInstances;

	float m_elapsedTime;

	RevAnimation* m_sharedAnimation;
	std::vector< RevTransformationNode* >* m_transformationNodes;
};

#endif