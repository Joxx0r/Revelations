#pragma once

#include "CU_Matrix.h"
#include "CU_Vector.h"
#include "RevBaseInstance.h"

class RevSimpleInstance : public RevBaseInstance
{
public:
	RevSimpleInstance();
	virtual ~RevSimpleInstance();

	bool Init(RevModel* aModel);
	virtual bool Update(const float& delta) { delta; return false; }
	bool Render();

	void SetTransformation(const CU::Matrix44f& aTransformation);
	void SetPosition(const CU::Vector4f& aPosition);
	void SetRotation(const CU::Matrix33f& aRotation);

	RevModel* GetModel() { return m_model;  }
	const CU::Matrix44f& GetTransformation() { return m_transformation;  }

private:

	CU::Matrix44f	m_transformation;

	RevModel* m_model;
};

