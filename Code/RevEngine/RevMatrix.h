#ifndef _REV_MATRIX_H
#define _REV_MATRIX_H

#include "RevQuat.h"
#include "RevVector.h"

class RevMatrix
{

public:
	enum RevMatrixCord
	{
		_AX,
		_AY,
		_AZ,
		_AW,
		_BX,
		_BY,
		_BZ,
		_BW,
		_CA,
		_CB,
		_CZ,
		_CW,
		_DX, 
		_DY,
		_DZ,
		_DW
	}

	RevQuat m_rotation;
	RevVector m_translation;
	
	RevMatrix()
	{
		Identity();
	};

	~RevMatrix()
	{

	};

	void Identity()
	{
		
	}
	
	

};

#endif