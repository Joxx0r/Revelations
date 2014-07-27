#ifndef _REV_TRANSFORMATION_NODE_H
#define _REV_TRANSFORMATION_NODE_H

#include "CU_Matrix.h"

class RevTransformationNode
{
public:
	RevTransformationNode(void);
	virtual ~RevTransformationNode(void);	

	virtual CU::Matrix44f GetTransformationForTime(float aElapsedTime = 0);
	void SetTransformation( const CU::Matrix44f& transformation );

protected:
	CU::Matrix44f m_transformation;
};

#endif //_REV_TRANSFORMATION_NODE_H