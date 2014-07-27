#ifndef _REV_TRANSFORMATION_HIERACHY_NODE_H
#define _REV_TRANSFORMATION_HIERACHY_NODE_H
 
#include "RevTransformationNode.h"
#include <vector>

class RevTransformationHierachyNode  : public RevTransformationNode
{
public:
	RevTransformationHierachyNode(void);
	virtual ~RevTransformationHierachyNode(void);
	void AddTransformation(RevTransformationNode* transformationNode);
	virtual CU::Matrix44f GetTransformationForTime(float aElapsedTime = 0);

private:
	std::vector< RevTransformationNode* > m_childs;
};


#endif