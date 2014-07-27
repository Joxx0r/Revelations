#ifndef _REV_BONE_H
#define _REV_BONE_H

#include "CU_Matrix.h"
#include <vector>

class RevBone
{
public:
	RevBone(void);
	virtual ~RevBone(void);

	void AddChild(const int& aChild); 
	CU::Matrix44f GetInverseTransformedBindPoseOrientation();
	CU::Matrix44f GetBindPose();
	void SetBindPose(CU::Matrix44f aMatrix);
	void TransformBindPose(const CU::Matrix44f& aMatrix);
	std::vector<int>& GetChilds();

private:

	CU::Matrix44f m_orgBindPoseTransformation;
	CU::Matrix44f m_inverseTransformedBindPoseTransformation;
	CU::Matrix44f m_transformedBindPoseTransformation;
	std::vector<int> m_childObjects;

};

#endif