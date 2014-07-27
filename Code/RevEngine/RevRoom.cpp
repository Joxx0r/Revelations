#include "stdafx.h"
#include "RevRoom.h"


RevRoom::RevRoom(void)
{
}


RevRoom::~RevRoom(void)
{
}

void RevRoom::Init(	const CU::Vector3f& aHalfExtents,
				   const CU::Vector3f& aCenterPos )
{
	myHalfExtents	= aHalfExtents;
	myCenterPos		= aCenterPos;
}

bool RevRoom::IsPointInside( const CU::Vector3f& aPoint )
{
	CU::Vector3f dist = aPoint - myCenterPos;
	if( abs(dist.x) <= myHalfExtents.x && 
		abs(dist.y) <= myHalfExtents.y  && 
		abs(dist.z) <= myHalfExtents.z )
	{
		return true;
	}

	return false;
}

CU::Vector3f RevRoom::GetBiggestPoint()
{
	return myCenterPos + myHalfExtents;
}

CU::Vector3f RevRoom::GetSmallestPoint()
{
	return myCenterPos - myHalfExtents;
}

CU::Vector3f RevRoom::GetHalfExtents()
{
	return myHalfExtents;
}

CU::Vector3f RevRoom::GetCenterPos()
{
	return myCenterPos;
}