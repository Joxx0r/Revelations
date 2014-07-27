#ifndef _REV_ROOM_H
#define _REV_ROOM_H

#include "CU_Vector.h"

class RevRoom
{
public:
	RevRoom(void);
	~RevRoom(void);
	void Init(	const CU::Vector3f& aHalfExtents,
				const CU::Vector3f& aCenterPos );
	bool IsPointInside( const CU::Vector3f& aPoint );
	CU::Vector3f GetBiggestPoint();
	CU::Vector3f GetSmallestPoint();
	CU::Vector3f GetHalfExtents();
	CU::Vector3f GetCenterPos();
private:
	CU::Vector3f myHalfExtents;
	CU::Vector3f myCenterPos;
};

#endif