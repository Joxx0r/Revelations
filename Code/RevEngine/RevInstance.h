#ifndef _INSTANCE_H
#define _INSTANCE_H

#include "CU_Matrix.h"
#include "CU_Vector.h"
#include "RevBaseInstance.h"

class RevModel;
class RevInstanceBaseNode;

class RevInstance : public RevBaseInstance
{
public:
	RevInstance(void);
	virtual ~RevInstance(void);
	virtual bool Init( RevModel* aModel );
	virtual bool Update(const float& delta);
	virtual bool Render();

	void SetRoomIndex(const int& aRoomIndex);

	const int& GetRoomIndex();

private:
	int				myRoomIndex;

	RevInstanceBaseNode* m_instanceNode;
};

#endif