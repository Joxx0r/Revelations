#include "stdafx.h"
#include "RevTransformationCurveNode.h"

RevTransformationCurveNode::RevTransformationCurveNode(void)
{
	for(int i=0;i<4;i++) myRotationCurves[i]=NULL;
	for(int i=0;i<3;i++) myTranslationCurves[i]=NULL;
}


RevTransformationCurveNode::~RevTransformationCurveNode(void)
{
	for(int i=0;i<4;i++) delete(myRotationCurves[i]);
	for(int i=0;i<3;i++) delete(myTranslationCurves[i]);
}

void RevTransformationCurveNode::Init(
	RevAnimationType aType, 
	int aNumberOfKeyFrames,
	int aAnimationFPS,
	float aStartTime,
	float aEndTime)
{
	myAnimationFPS=aAnimationFPS;
	myStartTime=aStartTime;
	myEndTime=aEndTime;
	myAnimationType = aType;
}

CU::Matrix44f RevTransformationCurveNode::GetTransformationForTime(float aElapsedTime/* = 0.0f*/)
{
	RevCurveAnimationFrame keyFrame=GetFrameForTime(aElapsedTime);
	CU::Matrix44f temp;

	temp=m_transformation;
	temp*= CU::Matrix33f::RotationAroundVector(keyFrame.myDirection,keyFrame.myAngle);
	temp.myMatrix[3][0]=keyFrame.myTranslation.x;
	temp.myMatrix[3][1]=keyFrame.myTranslation.y;
	temp.myMatrix[3][2]=keyFrame.myTranslation.z;

	return(temp);
}

void RevTransformationCurveNode::SetBaseFrame(CU::Vector3f myDirection,CU::Vector3f aTranslation,float aAngle)
{
	myBaseFrame.myDirection=myDirection;
	myBaseFrame.myTranslation=aTranslation;
	myBaseFrame.myAngle=aAngle;
}

void RevTransformationCurveNode::SetRotationCurve(int i,RevAnimationCurve* aCurve)
{
	myRotationCurves[i]=aCurve;
}

void RevTransformationCurveNode::SetTranslationCurve(int i,RevAnimationCurve* aCurve)
{
	myTranslationCurves[i]=aCurve;
}

RevCurveAnimationFrame RevTransformationCurveNode::GetFrameForTime(float aElapsedTime)
{
	if(myRotationCurves[0]!=NULL)	myBaseFrame.myDirection.x=myRotationCurves[0]->GetValueFromTime(aElapsedTime);
	if(myRotationCurves[1]!=NULL)	myBaseFrame.myDirection.y=myRotationCurves[1]->GetValueFromTime(aElapsedTime);
	if(myRotationCurves[2]!=NULL)	myBaseFrame.myDirection.z=myRotationCurves[2]->GetValueFromTime(aElapsedTime);
	if(myRotationCurves[3]!=NULL)	myBaseFrame.myAngle=myRotationCurves[3]->GetValueFromTime(aElapsedTime);
	if(myTranslationCurves[0]!=NULL)	myBaseFrame.myTranslation.x=myTranslationCurves[0]->GetValueFromTime(aElapsedTime);
	if(myTranslationCurves[1]!=NULL)	myBaseFrame.myTranslation.y=myTranslationCurves[1]->GetValueFromTime(aElapsedTime);
	if(myTranslationCurves[2]!=NULL)	myBaseFrame.myTranslation.z=myTranslationCurves[2]->GetValueFromTime(aElapsedTime);

	return(myBaseFrame);
}

