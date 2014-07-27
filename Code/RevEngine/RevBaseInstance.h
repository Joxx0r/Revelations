#pragma once

#include "CU_Matrix.h"
#include "CU_Vector.h"

class RevBaseInstance
{
public:

	enum Type
	{
		_SLIM,
		_HIEARCHY,
		_NUM_OF_TYPES
	};

	RevBaseInstance() { };
	virtual ~RevBaseInstance() { };


	virtual bool Init(RevModel* aModel) = 0;
	virtual bool Update(const float& delta) = 0;
	virtual bool Render() = 0;

	void SetTransformation(const CU::Matrix44f& aTransformation);
	void SetPosition(const CU::Vector4f& aPosition);
	void SetRotation(const CU::Matrix33f& aRotation);

	const CU::Matrix44f& GetTransformation();
	CU::Vector4f GetPosition();
	CU::Matrix33f& GetRotation();

protected:

	CU::Matrix44f m_transformation;

};