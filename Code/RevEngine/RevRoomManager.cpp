#include "stdafx.h"
#include "RevRoomManager.h"
#include "RevEngineMain.h"
#include "RevReflectionCubeMapStructure.h"
#include "RevInstance.h"
#include <limits>

RevRoomManager::RevRoomManager(void)
{
	myRooms.reserve( 100 );
}


RevRoomManager::~RevRoomManager(void)
{
	for(unsigned int roomIndex = 0; roomIndex < myRooms.size(); roomIndex++)
	{
		delete myRooms[ roomIndex ];
	}
	myRooms.clear();
}

void RevRoomManager::Init( const char* aFilePath )
{
	RevEngineMain::GetModelFactory().CreateRooms( aFilePath, myRooms );
	CalcRoomSize( );
}

bool RevRoomManager::IsPointInsideARoom( const CU::Vector3f& aPoint )
{
	for(unsigned int roomIndex = 0; roomIndex < myRooms.size(); roomIndex++)
	{
		if( myRooms[ roomIndex ]->IsPointInside( aPoint ) == true )
		{
			return true;
		}
	}
	return false;
}

void RevRoomManager::BuildReflectionMapsOnCubeMaps( 
		std::vector< RevReflectionCubeMapStructure >& aEnviroments,
		const int& aEnvMapSize)
{
	for(int roomIndex = 0; roomIndex < myRooms.size(); roomIndex++)
	{
		BuildEnvMapOnRoom( aEnviroments, myRooms[ roomIndex ], aEnvMapSize );
	}
}

void RevRoomManager::PlaceInstanceInRoom( RevInstance* aInstance )
{
	CU::Vector3f pos = aInstance->GetPosition();

	for(int roomIndex = 0; roomIndex < myRooms.size(); roomIndex++)
	{
		if( myRooms[ roomIndex ]->IsPointInside( pos ) == true )
		{
			return;
		}
	}
}

CU::Vector3f RevRoomManager::GetRoomsFullExtents()
{
	return myRoomsExtents;
}

CU::Vector3f RevRoomManager::GetMin( )
{
	return myMinRoomPosition;
}

CU::Vector3f RevRoomManager::GetMax( )
{
	return myMaxRoomPosition;
}

void RevRoomManager::CalcRoomSize()
{
	const float minValue = (std::numeric_limits< float >::min)();
	const float maxValue = (std::numeric_limits< float >::max)();

	CU::Vector3f minVal(maxValue,maxValue,maxValue);
	CU::Vector3f maxVal(minValue,minValue,minValue);
	for(int roomIndex = 0; roomIndex < myRooms.size(); roomIndex++)
	{
		RevRoom* room = myRooms[ roomIndex ];
		CU::Vector3f points[2] = {
						room->GetBiggestPoint(), 
						room->GetSmallestPoint() };

		for(int pointIndex = 0; pointIndex < 2; pointIndex++ )
		{
			for(int elementIndex = 0; elementIndex < 3; elementIndex++)
			{
				if( minVal[ elementIndex ] >  points[pointIndex][ elementIndex ] )
				{
					minVal[ elementIndex ] = points[pointIndex][ elementIndex ];
				}

				if( maxVal[ elementIndex ] <  points[pointIndex][ elementIndex ] )
				{
					maxVal[ elementIndex ] = points[pointIndex][ elementIndex ];
				}
			}
		}
	}

	myMaxRoomPosition = maxVal;
	myMinRoomPosition = minVal;
	myRoomsExtents = maxVal - minVal;
}

void RevRoomManager::BuildEnvMapOnRoom( 
				std::vector< RevReflectionCubeMapStructure >& aVector,
				RevRoom* aRoom, 
				const int& aReflectionSize )
{
	aVector.push_back( RevReflectionCubeMapStructure() );
	aVector[ aVector.size() - 1 ].Init( aRoom, aReflectionSize );
}