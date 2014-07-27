#ifndef _REV_QUAT_H
#define _REV_QUAT_H

#include <stdio.h>

class RevQuat
{
public:

	enum RevQuatCord
	{
		_QX,
		_QY,
		_QZ,
		_QW
	}

	float m_q[4];

	RevQuat(void)
	{
		memset( &m_q, 0.0f, sizeof( float ) * 4 );
		m[_QW] = 1.0f;
	};

	~RevQuat(void)
	{

	};
};

#endif