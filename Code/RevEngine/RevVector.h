#ifndef _REV_VECTOR_H
#define _REV_VECTOR_H

#include <stdio.h>

class RevVector
{
public:
	enum RevVecCord
	{
		_TX,
		_TY,
		_TZ,
		_TW
	}

	float m_v[4];
	
	RevVector()
	{
		memset( &m_v, 0.0f, sizeof( float ) * 4 );
	};

	~RevVector()
	{

	}

};
#endif