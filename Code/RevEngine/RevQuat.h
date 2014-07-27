#ifndef _REV_QUAT_H
#define _REV_QUAT_H

#include <stdio.h>

class RevQuat
{
public:

	float m_cords[4];

	RevQuat(void)
	{
		memset( &m_cords, 0.0f, sizeof( float ) * 4 );
	}

	~RevQuat(void)
	{

	}
};

#endif