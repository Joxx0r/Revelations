#ifndef _REV_TRANSFORMATION_CURVE_NODE_H
#define _REV_TRANSFORMATION_CURVE_NODE_H

#include "RevTransformationNode.h"
#include "CU_Vector.h"
#include "CU_Matrix.h"
#include "RevAnimationCurve.h"

struct RevCurveAnimationFrame
{
	CU::Vector3f myDirection;
	CU::Vector3f myTranslation;
	float myAngle;
};

class RevTransformationCurveNode : public RevTransformationNode
{
public:
	RevTransformationCurveNode(void);
	virtual ~RevTransformationCurveNode(void);
	void Init(
		RevAnimationType aType, 
		int aNumberOfKeyFrames,
		int aAnimationFPS,
		float aStartTime,
		float aEndTime);
		
	virtual CU::Matrix44f GetTransformationForTime(float aElapsedTime = 0.0f);
		
	void SetBaseFrame(CU::Vector3f myDirection,CU::Vector3f aTranslation,float aAngle);
	void SetRotationCurve(int i,RevAnimationCurve* aCurve);
	void SetTranslationCurve(int i,RevAnimationCurve* aCurve);

private:
	RevCurveAnimationFrame GetFrameForTime(float aElapsedTime);

	int myAnimationFPS;
	float myStartTime;
	float myEndTime;

	RevAnimationType myAnimationType;
	RevAnimationCurve* myRotationCurves[4];
	RevAnimationCurve* myTranslationCurves[3];
	RevCurveAnimationFrame myBaseFrame;
};

#endif