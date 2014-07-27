#ifndef _REV_ROOM_MANAGER_H
#define _REV_ROOM_MANAGER_H

#include "CU_Vector.h"
#include "RevRoom.h"

#include <vector>

class RevReflectionCubeMapStructure;
class RevInstance;

class RevRoomManager
{
public:
	RevRoomManager(void);
	~RevRoomManager(void);
	void Init( const char* aFilePath );
	bool IsPointInsideARoom( const CU::Vector3f& aPoint );
	void BuildReflectionMapsOnCubeMaps( 
		std::vector< RevReflectionCubeMapStructure >& aEnviroments,
		const int& aEnvMapSize );

	void PlaceInstanceInRoom( RevInstance* aInstance );

	CU::Vector3f GetRoomsFullExtents();
	CU::Vector3f GetMin( );
	CU::Vector3f GetMax( );

	std::vector< RevRoom* >& GetRooms() { return myRooms; }

private:
	void CalcRoomSize();
	void BuildEnvMapOnRoom( std::vector< RevReflectionCubeMapStructure >& aVector,
							RevRoom* aRoom, 
							const int& aReflectionSize );

	std::vector< RevRoom* > myRooms;

	CU::Vector3f myRoomsExtents;
	CU::Vector3f myMaxRoomPosition;
	CU::Vector3f myMinRoomPosition;
};

#endif
