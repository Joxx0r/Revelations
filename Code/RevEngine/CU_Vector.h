#ifndef COMMONUTILITIES_VECTOR_HEADER
#define COMMONUTILITIES_VECTOR_HEADER

#include <math.h>

namespace CommonUtilities
{
	template <class TYPE>
	class Vector3;

	template <class TYPE>
	class Vector2
	{
	public:
		Vector2();
		Vector2(TYPE aX,TYPE aY);
		Vector2(Vector3<TYPE> aVector);
		~Vector2();

		void Set(TYPE aX,TYPE aY);

		Vector2 operator+(const Vector2& a2DVector);
		Vector2& operator+=(const Vector2& aVector);
		Vector2& operator-=(const Vector2& aVector);
		Vector2& operator*=(const Vector2& aVector);
		Vector2& operator/=(const Vector2& aVector);
		
		bool IsDifferentFrom(Vector2<TYPE> aVector);
		bool operator!=(const Vector2<TYPE>& aVector);

		Vector2& operator*=(const TYPE& aType);
		Vector2& operator/=(const TYPE& aType);

		Vector2 operator-(const TYPE& aType);
		void Rotate(float aAngle);
		
		Vector2 operator-();
		TYPE Dot(Vector2 a2DVector);
		Vector2& operator=(const Vector2& aVector);

		bool operator==(const Vector2& aVector) const;
		inline void Zero();
		inline bool IsZero();
		TYPE Length() const;
		TYPE Length2() const;
		Vector2<TYPE>& Normalize();

		union
		{
			TYPE myX;
			TYPE x;
			TYPE u;
		};
		union
		{
			TYPE myY;
			TYPE y;
			TYPE v;
		};
	};
	template <class TYPE>
	TYPE Vector2<TYPE>::Dot(Vector2<TYPE> a2DVector)
	{
		return (myX*a2DVector.myX + myY * a2DVector.myY);
	}
	template <class TYPE>
	bool Vector2<TYPE>::operator!=(const Vector2<TYPE>& aVector) 
	{
		if(x==aVector.x && y==aVector.y)
		{ 
			return(false);
		}
		return(true);
	}
	template <class TYPE>
	bool Vector2<TYPE>::IsDifferentFrom(Vector2<TYPE> aVector)
	{
		if(aVector.x != x)
		{
			return true;
		}
		if(aVector.y != y)
		{
			return true;
		}
		return false;
	}
	template <class TYPE>
	Vector2<TYPE> Vector2<TYPE>::operator+(const Vector2<TYPE>& a2DVector)
	{
		Vector2<TYPE> myTempVector = *this;
		myTempVector.myX += a2DVector.myX;
		myTempVector.myY += a2DVector.myY;
		return myTempVector;
	}
	template <class TYPE>
	Vector2<TYPE> operator-(const Vector2<TYPE>& aLeftSideVector,const Vector2<TYPE>& aRightSideVector)
	{
		Vector2<TYPE> returnValue(aLeftSideVector);
		returnValue-=aRightSideVector;
		return(returnValue);
	}

	template <class TYPE>
	Vector2<TYPE> operator*(const Vector2<TYPE>& aLeftSideVector,const Vector2<TYPE>& aRightSideVector)
	{
		Vector2<TYPE> returnValue(aLeftSideVector);
		returnValue*=aRightSideVector;
		return(returnValue);
	}
	template <class TYPE>
	Vector2<TYPE> operator/(const Vector2<TYPE>& aLeftSideVector,const Vector2<TYPE>& aRightSideVector)
	{
		Vector2<TYPE> returnValue(aLeftSideVector);
		returnValue/=aRightSideVector;
		return(returnValue);
	}
	template <class TYPE>
	Vector2<TYPE> operator*(const Vector2<TYPE>& aLeftSideVector,const TYPE& aRightSideType)
	{
		Vector2<TYPE> returnValue(aLeftSideVector);
		returnValue*=aRightSideType;
		return(returnValue);
	}
	template <class TYPE>
	Vector2<TYPE> operator/(const Vector2<TYPE>& aLeftSideVector,const TYPE& aRightSideType)
	{
		Vector2<TYPE> returnValue(aLeftSideVector);
		returnValue/=aRightSideType;
		return(returnValue);
	}
	template <class TYPE>
	void Vector2<TYPE>::Rotate(float aAngle)
	{
		float Sin,Cos;

		Sin=(float)sin(aAngle);
		Cos=(float)cos(aAngle);
		float tempX,tempY;
		tempX=myX*Cos+myY*Sin;
		tempY=myX*-Sin+myY*Cos;
		myX=tempX;
		myY=tempY;
	}
	template <class TYPE>
	Vector2<TYPE>::Vector2()
	{
		myX=0;
		myY=0;
	}

	template <class TYPE>
	Vector2<TYPE>::Vector2(const TYPE aX,const TYPE aY)
	{
		myX=aX;
		myY=aY;
	}

		template <class TYPE>
	void Vector2<TYPE>::Zero()
	{
		myX = 0;
		myY = 0;
	}

	template <class TYPE>
	bool Vector2<TYPE>::IsZero()
	{
		return myX == 0.0f && myY == 0.0f;
	}

	template <class TYPE>
	Vector2<TYPE>::~Vector2()
	{
	}

	template <class TYPE>
	Vector2<TYPE> Vector2<TYPE>::operator-()
	{
		Vector2<TYPE> v;
		v.x = -myX;
		v.y = -myY;
		return v;
	}

	template <class TYPE>
	void Vector2<TYPE>::Set(const TYPE aX,const TYPE aY)
	{
		myX=aX;
		myY=aY;
	}

	template <class TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator+=(const Vector2<TYPE>& aVector)
	{
		myX+=aVector.myX;
		myY+=aVector.myY;
		return(*this);
	}
	template <class TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator-=(const Vector2<TYPE>& aVector)
	{
		myX-=aVector.myX;
		myY-=aVector.myY;
		return(*this);
	}

	template <class TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator*=(const Vector2<TYPE>& aVector)
	{
		myX*=aVector.myX;
		myY*=aVector.myY;
		return(*this);
	}
	template <class TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator/=(const Vector2<TYPE>& aVector)
	{
		myX/=aVector.myX;
		myY/=aVector.myY;
		return(*this);
	}

	template <class TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator*=(const TYPE& aType)
	{
		myX*=aType;
		myY*=aType;
		return(*this);
	}

	template <class TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator/=(const TYPE& aType)
	{
		myX/=aType;
		myY/=aType;
		return(*this);
	}

	template <class TYPE>
	Vector2<TYPE> Vector2<TYPE>::operator-(const TYPE& aType)
	{
		return( CU::Vector2f(myX - aType,myY - aType) );
	}


	template <class TYPE>
	Vector2<TYPE>& Vector2<TYPE>::operator=(const Vector2<TYPE>& aVector)
	{
		myX=aVector.myX;
		myY=aVector.myY;
		return(*this);
	}

	template <class TYPE>
	bool Vector2<TYPE>::operator==(const Vector2<TYPE>& aRightSideVector) const 
	{
		if(myX!=aRightSideVector.myX) return(false);
		if(myY!=aRightSideVector.myY) return(false);
		return(true);
	}


	template <class TYPE>
	Vector2<TYPE>::Vector2(Vector3<TYPE> aVector)
	{
		myX = aVector.x;
		myY = aVector.y;
	}


	template <class TYPE>
	Vector2<TYPE>& Vector2<TYPE>::Normalize()
	{
		float length=Length();
		if(length > 0)
		{
			myX/=length;
			myY/=length;
		}
		return *this;
	}

	template <class TYPE>
	TYPE Vector2<TYPE>::Length() const 
	{
		return(sqrt(myX*myX+myY*myY));
	}

	template <class TYPE>
	TYPE Vector2<TYPE>::Length2() const 
	{
		return(myX*myX+myY*myY);
	}

	typedef Vector2<float> Vector2f;
	typedef Vector2<double> Vector2d;
	typedef Vector2<int> Vector2i;


	template <class TYPE>
	class Vector4;



	template <class TYPE>
	class Vector3
	{
	public:
		//		Vector3(const D3DXVECTOR3& aVec);
		Vector3();
		Vector3(Vector4<TYPE> aVector);
		Vector3(TYPE aX,TYPE aY,TYPE aZ);
		~Vector3();

		void Set(TYPE aX,TYPE aY,TYPE aZ);

		Vector3 operator-(const Vector3& aVector);
		//		Vector3& operator=(const D3DXVECTOR3& aVec);

		Vector3& operator+=(const Vector3& aVector);
		Vector3& operator-=(const Vector3& aVector);
		Vector3& operator*=(const Vector3& aVector);
		Vector3& operator/=(const Vector3& aVector);
		Vector3 operator-();

		Vector3 operator*(const TYPE& aType);
		
		Vector3 operator/(const TYPE& aType);
		Vector3 operator/(const Vector3& aVector);
		Vector3 operator+(const Vector3& aVector);
		Vector3 operator+(const TYPE& aScalar);
		Vector3& operator*=(const TYPE& aType);
		Vector3& operator/=(const TYPE& aType);
		Vector3 LerpRandom(Vector3<TYPE> aVector);

		float Dot(const Vector3& aVector);
		Vector3 Cross(const Vector3& aVector);

		void PackIntoShortData( unsigned short* shortData );
		Vector3& operator=(const Vector3& aVector);

		TYPE& operator[](const int& anIndex);
		bool operator==(const Vector3& aVector) const;
		bool IsZero() const;
		TYPE Length() const;
		TYPE Length2() const;
		TYPE LengthXY() const;
		TYPE LengthXZ() const;
		Vector3<TYPE>& Normalize();
		Vector3<TYPE> NonSelfNormalize();

		union
		{
			TYPE myX;
			TYPE x;
			TYPE r;
		};
		union
		{
			TYPE myY;
			TYPE y;
			TYPE g;

		};
		union
		{
			TYPE myZ;
			TYPE z;
			TYPE b;

		};
	};


	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator*(const TYPE& aType)
	{
		Vector3<TYPE> returnV;
		returnV.x = myX;
		returnV.y = myY,
		returnV.z = myZ;
		returnV *= aType;
		return returnV;
	}


	template <class TYPE>
	void Vector3<TYPE>::PackIntoShortData( unsigned short* shortData )
	{
		CU::Vector3f copy = *this;
		copy.myX = copy.myX / 2.0f;
		copy.myY = copy.myY / 2.0f;
		copy.myZ = copy.myZ / 2.0f;
		copy += FLT_MAX * 0.5f;
		unsigned short data[3];
		data[0] = static_cast< unsigned short >( copy.myX );
		data[1] = static_cast< unsigned short >( copy.myY );
		data[2] = static_cast< unsigned short >( copy.myZ );

		memcpy( &shortData[0], &data, sizeof( unsigned short ) * 3 );
	}

	template <class TYPE>
	float Vector3<TYPE>::Dot(const Vector3& aVector)
	{
		return(aVector.x*x+aVector.y*y+aVector.z*z);
	}

	template <class TYPE>
	Vector3<TYPE> operator+(const Vector3<TYPE>& aLeftSideVector,const Vector3<TYPE>& aRightSideVector)
	{
		Vector3<TYPE> returnValue(aLeftSideVector);
		returnValue+=aRightSideVector;
		return(returnValue);
	}

	template <class TYPE>
	Vector3<TYPE> operator-(const Vector3<TYPE>& aLeftSideVector,const Vector3<TYPE>& aRightSideVector)
	{
		Vector3<TYPE> returnValue(aLeftSideVector);
		returnValue-=aRightSideVector;
		return(returnValue);
	}

	template <class TYPE>
	Vector3<TYPE> operator*(const Vector3<TYPE>& aLeftSideVector,const Vector3<TYPE>& aRightSideVector)
	{
		Vector3<TYPE> returnValue(aLeftSideVector);
		returnValue*=aRightSideVector;
		return(returnValue);
	}
	template <class TYPE>
	Vector3<TYPE> operator/(const Vector3<TYPE>& aLeftSideVector,const Vector3<TYPE>& aRightSideVector)
	{
		Vector3<TYPE> returnValue(aLeftSideVector);
		returnValue/=aRightSideVector;
		return(returnValue);
	}
	template <class TYPE>
	Vector3<TYPE> operator*(const Vector3<TYPE>& aLeftSideVector,const TYPE& aRightSideType)
	{
		Vector3<TYPE> returnValue(aLeftSideVector);
		returnValue*=aRightSideType;
		return(returnValue);
	}

	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::LerpRandom(Vector3<TYPE> aVector)
	{
		CU::Vector3f returnV;
		CU::Vector3f v = *this;
		for(int i = 0; i < 3; i++)
		{
			float r = (float)rand() / (float)RAND_MAX;
			returnV[i] = v[i] + r * (aVector[i] - v[i]);

		}
		return returnV;
	}

	template <class TYPE>
	Vector3<TYPE> operator/(const Vector3<TYPE>& aLeftSideVector,const TYPE& aRightSideType)
	{
		Vector3<TYPE> returnValue(aLeftSideVector);
		returnValue/=aRightSideType;
		return(returnValue);
	}

	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator+(const TYPE& aScalar)
	{
		Vector3<TYPE> returnV;
		returnV.x = myX + aScalar;
		returnV.y = myY + aScalar;
		returnV.z = myZ + aScalar;
		return returnV;
	}

	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator/(const TYPE& aType)
	{
		Vector3<TYPE> v;
		v = *this;
		v /= aType;
		return v;
	}


	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator/(const Vector3& aVector)
	{
		Vector3<TYPE> v;
		v = *this;
		v.x /= aVector.x;
		v.y /= aVector.y;
		v.z /= aVector.z;
		return v;
	}

	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator-()
	{
		Vector3<TYPE> returnVector;
		returnVector.x = -myX;
		returnVector.y = -myY;
		returnVector.z = -myZ;
		return returnVector;
	}

	template <class TYPE>
	Vector3<TYPE>  Vector3<TYPE>::Cross(const Vector3<TYPE>& aVector)
	{
		return(Vector3<TYPE>(aVector.y*myZ-aVector.z*myY,
			aVector.z*myX-aVector.x*myZ,
			aVector.x*myY-aVector.y*myX));

	}

	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator+(const Vector3<TYPE>& aVector)
	{
		Vector3<TYPE> rV;
		rV = *this;
		rV += aVector;
		return rV;
	}

	template <class TYPE>
	Vector3<TYPE>::Vector3()
	{
		myX = 0;
		myY = 0;
		myZ = 0;
	}
	
	template <class TYPE>
	bool Vector3<TYPE>::IsZero() const
	{
		if(myX == 0 && myY == 0  && myZ == 0) return true;
		return false;
	}
	/*	template <class TYPE>
	Vector3<TYPE>::Vector3(const D3DXVECTOR3& aVec)
	{
	myX=aVec.x;
	myY=aVec.y;
	myZ=aVec.z;
	}
	*/
	template <class TYPE>
	Vector3<TYPE>::Vector3(const TYPE aX,const TYPE aY,const TYPE aZ)
	{
		myX=aX;
		myY=aY;
		myZ=aZ;
	}
	template <class TYPE>
	Vector3<TYPE>::Vector3(Vector4<TYPE> aVector)
	{
		myX=aVector.x;
		myY=aVector.y;
		myZ=aVector.z;
	}


	template <class TYPE>
	Vector3<TYPE>::~Vector3()
	{
	}

	template <class TYPE>
	void Vector3<TYPE>::Set(const TYPE aX,const TYPE aY,const TYPE aZ)
	{
		myX=aX;
		myY=aY;
		myZ=aZ;
	}

	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::operator-(const Vector3<TYPE>& aVector)
	{
		Vector3<TYPE> vec;
		vec.x = myX - aVector.myX;
		vec.y = myY - aVector.myY;
		vec.z = myZ - aVector.myZ;
		return(vec);
	}


	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator+=(const Vector3<TYPE>& aVector)
	{
		myX+=aVector.myX;
		myY+=aVector.myY;
		myZ+=aVector.myZ;
		return(*this);
	}

	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator-=(const Vector3<TYPE>& aVector)
	{
		myX-=aVector.myX;
		myY-=aVector.myY;
		myZ-=aVector.myZ;
		return(*this);
	}


	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator*=(const Vector3<TYPE>& aVector)
	{
		myX*=aVector.myX;
		myY*=aVector.myY;
		myZ*=aVector.myZ;

		return(*this);
	}
	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator/=(const Vector3<TYPE>& aVector)
	{
		myX/=aVector.myX;
		myY/=aVector.myY;
		myZ/=aVector.myZ;
		return(*this);
	}

	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator*=(const TYPE& aType)
	{
		myX*=aType;
		myY*=aType;
		myZ*=aType;
		return(*this);
	}

	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator/=(const TYPE& aType)
	{
		myX/=aType;
		myY/=aType;
		myZ/=aType;
		return(*this);
	}

	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator=(const Vector3<TYPE>& aVector)
	{
		myX=aVector.myX;
		myY=aVector.myY;
		myZ=aVector.myZ;

		return(*this);
	}
	/*
	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::operator=(const D3DXVECTOR3& aVec)
	{
	myX=aVec.x;
	myY=aVec.y;
	myZ=aVec.z;
	return(*this);
	}*/


	template <class TYPE>
	bool Vector3<TYPE>::operator==(const Vector3<TYPE>& aRightSideVector) const 
	{
		if(myX!=aRightSideVector.myX) return(false);
		if(myY!=aRightSideVector.myY) return(false);
		if(myZ!=aRightSideVector.myZ) return(false);
		return(true);
	}

	template <class TYPE>
	TYPE& Vector3<TYPE>::operator[](const int& anIndex)
	{
		if(anIndex==0) return myX;
		if(anIndex==1) return myY;
		if(anIndex==2) return myZ;
		return myX;
	}

	template <class TYPE>
	Vector3<TYPE>& Vector3<TYPE>::Normalize()
	{
		float length=Length();
		if(length!=0)
		{
			myX/=length;
			myY/=length;
			myZ/=length;
		}
		return *this;
	}

	template <class TYPE>
	Vector3<TYPE> Vector3<TYPE>::NonSelfNormalize()
	{
		Vector3<TYPE> v = *this;
		float length=Length();
		if(length!=0)
		{
			v.x/=length;
			v.y/=length;
			v.z/=length;
		}
		return v;
	}


	template <class TYPE>
	TYPE Vector3<TYPE>::Length() const 
	{
		return(sqrt(myX*myX+myY*myY+myZ*myZ));
	}

	template <class TYPE>
	TYPE Vector3<TYPE>::Length2() const 
	{
		return(myX*myX+myY*myY+myZ*myZ);
	}

	template <class TYPE>
	TYPE Vector3<TYPE>::LengthXY() const 
	{
		return(myX*myX+myY*myY);
	}

	template <class TYPE>
	TYPE Vector3<TYPE>::LengthXZ() const 
	{
		return( sqrt(myX*myX+myZ*myZ) );
	}

	typedef Vector3<float> Vector3f;
	typedef Vector3<double> Vector3d;
	typedef Vector3<int> Vector3i;


	template <class TYPE>
	class Vector4
	{
	public:
		Vector4();
		Vector4(TYPE aX,TYPE aY,TYPE aZ,TYPE aAlpha);
		Vector4(const Vector3<TYPE>& aVector);
		~Vector4();

		void Set(TYPE aX,TYPE aY,TYPE aZ,TYPE aAlpha);

		Vector4& operator+=(const Vector4& aVector);
		Vector4& operator-=(const Vector4& aVector);
		Vector4& operator*=(const Vector4& aVector);
		Vector4& operator/=(const Vector4& aVector);

		Vector4& operator*=(const TYPE& aType);
		Vector4& operator/=(const TYPE& aType);

		void PackIntoShortData( unsigned short* shortData );

		Vector4 Invert();
		float Dot(const Vector4& aVector);


		Vector4& operator=(const Vector4& aVector);
		Vector4& operator=(const Vector3<TYPE>& aVector);

		bool operator==(const Vector4& aVector) const;

		TYPE Length() const;
		TYPE Length2() const;
		Vector4<TYPE>& Normalize();


		TYPE& operator[](const int& anIndex);

		union
		{
			TYPE myX;
			TYPE x;
			TYPE r;
		};
		union
		{
			TYPE myY;
			TYPE y;
			TYPE g;

		};
		union
		{
			TYPE myZ;
			TYPE z;
			TYPE b;

		};
		union
		{
			TYPE myW;
			TYPE w;
			TYPE myAlpha;
			TYPE a;

		};
	};


	template <class TYPE>
	Vector4<TYPE>::Vector4(const Vector3<TYPE>& aVector)
	{
		x=aVector.x;
		y=aVector.y;
		z=aVector.z;
		w=1;
	}

	template <class TYPE>
	float Vector4<TYPE>::Dot(const Vector4& aVector)
	{
		return(aVector.x*x+aVector.y*y+aVector.z*z+aVector.w*w);
	}

	template <class TYPE>
	Vector4<TYPE> operator+(const Vector4<TYPE>& aLeftSideVector,const Vector4<TYPE>& aRightSideVector)
	{
		Vector4<TYPE> returnValue(aLeftSideVector);
		returnValue+=aRightSideVector;
		return(returnValue);
	}
	template <class TYPE>
	Vector4<TYPE> operator-(const Vector4<TYPE>& aLeftSideVector,const Vector4<TYPE>& aRightSideVector)
	{
		Vector4<TYPE> returnValue(aLeftSideVector);
		returnValue-=aRightSideVector;
		return(returnValue);
	}

	template <class TYPE>
	Vector4<TYPE> operator*(const Vector4<TYPE>& aLeftSideVector,const Vector4<TYPE>& aRightSideVector)
	{
		Vector4<TYPE> returnValue(aLeftSideVector);
		returnValue*=aRightSideVector;
		return(returnValue);
	}
	template <class TYPE>
	Vector4<TYPE> operator/(const Vector4<TYPE>& aLeftSideVector,const Vector4<TYPE>& aRightSideVector)
	{
		Vector4<TYPE> returnValue(aLeftSideVector);
		returnValue/=aRightSideVector;
		return(returnValue);
	}

	template <class TYPE>
	Vector4<TYPE> Vector4<TYPE>::Invert()
	{
		Vector4<TYPE> rv;
		rv.x = -myX;
		rv.y = -myY;
		rv.z = -myZ;
		rv.w = -myW;
		return rv;
	}

	template <class TYPE>
	Vector4<TYPE> operator*(const Vector4<TYPE>& aLeftSideVector,const TYPE& aRightSideType)
	{
		Vector4<TYPE> returnValue(aLeftSideVector);
		returnValue*=aRightSideType;
		return(returnValue);
	}
	template <class TYPE>
	Vector4<TYPE> operator/(const Vector4<TYPE>& aLeftSideVector,const TYPE& aRightSideType)
	{
		Vector4<TYPE> returnValue(aLeftSideVector);
		returnValue/=aRightSideType;
		return(returnValue);
	}




	template <class TYPE>
	Vector4<TYPE>::Vector4()
	{
		myX = 0;
		myY = 0;
		myZ = 0;
		myW = 1;
	}

	template <class TYPE>
	Vector4<TYPE>::Vector4(const TYPE aX,const TYPE aY,const TYPE aZ,const TYPE aW)
	{
		myX=aX;
		myY=aY;
		myZ=aZ;
		myW=aW;
	}

	template <class TYPE>
	Vector4<TYPE>::~Vector4()
	{
	}

	template <class TYPE>
	void Vector4<TYPE>::Set(const TYPE aX,const TYPE aY,const TYPE aZ,const TYPE aW)
	{
		myX=aX;
		myY=aY;
		myZ=aZ;
		myW=aW;
	}

	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::operator+=(const Vector4<TYPE>& aVector)
	{
		myX+=aVector.myX;
		myY+=aVector.myY;
		myZ+=aVector.myZ;
		myW+=aVector.myW;
		return(*this);
	}
	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::operator-=(const Vector4<TYPE>& aVector)
	{
		myX-=aVector.myX;
		myY-=aVector.myY;
		myZ-=aVector.myZ;
		myW-=aVector.myW;
		return(*this);
	}

	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::operator*=(const Vector4<TYPE>& aVector)
	{
		myX*=aVector.myX;
		myY*=aVector.myY;
		myZ*=aVector.myZ;
		myW*=aVector.myW;

		return(*this);
	}
	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::operator/=(const Vector4<TYPE>& aVector)
	{
		myX/=aVector.myX;
		myY/=aVector.myY;
		myZ/=aVector.myZ;
		myW/=aVector.myW;
		return(*this);
	}

	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::operator*=(const TYPE& aType)
	{
		myX*=aType;
		myY*=aType;
		myZ*=aType;
		myW*=aType;
		return(*this);
	}

	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::operator/=(const TYPE& aType)
	{
		myX/=aType;
		myY/=aType;
		myZ/=aType;
		myW/=aType;
		return(*this);
	}

	template <class TYPE>
	void Vector4<TYPE>::PackIntoShortData( unsigned short* shortData )
	{
		CU::Vector3f copy = *this;
		copy.myX = copy.myX / 2.0f;
		copy.myY = copy.myY / 2.0f;
		copy.myZ = copy.myZ / 2.0f;
		copy += FLT_MAX * 0.5f;
		unsigned short data[3];
		data[0] = static_cast< unsigned short >( copy.myX );
		data[1] = static_cast< unsigned short >( copy.myY );
		data[2] = static_cast< unsigned short >( copy.myZ );

		memcpy( &shortData[0], &data, sizeof( unsigned short ) * 3 );
	}

	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::operator=(const Vector4<TYPE>& aVector)
	{
		myX=aVector.myX;
		myY=aVector.myY;
		myZ=aVector.myZ;
		myW=aVector.myW;

		return(*this);
	}
	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::operator=(const Vector3<TYPE>& aVector)
	{
		myX=aVector.myX;
		myY=aVector.myY;
		myZ=aVector.myZ;
		myW=1;

		return(*this);
	}


	template <class TYPE>
	bool Vector4<TYPE>::operator==(const Vector4<TYPE>& aRightSideVector) const 
	{
		if(myX!=aRightSideVector.myX) return(false);
		if(myY!=aRightSideVector.myY) return(false);
		if(myZ!=aRightSideVector.myZ) return(false);
		if(myW!=aRightSideVector.myW) return(false);
		return(true);
	}


	template <class TYPE>
	Vector4<TYPE>& Vector4<TYPE>::Normalize()
	{
		float length=Length();
		myX/=length;
		myY/=length;
		myZ/=length;
		myW/=length;
		return *this;
	}

	template <class TYPE>
	TYPE Vector4<TYPE>::Length() const 
	{
		return(sqrt(myX*myX+myY*myY+myZ*myZ+myW*myW));
	}

	template <class TYPE>
	TYPE Vector4<TYPE>::Length2() const 
	{
		return(myX*myX+myY*myY+myZ*myZ+myW*myW);
	}

	template <class TYPE>
	TYPE& Vector4<TYPE>::operator[](const int& anIndex)
	{
		if(anIndex==0) return myX;
		if(anIndex==1) return myY;
		if(anIndex==2) return myZ;
		if(anIndex==3) return myW;
		return myX;
	}

	typedef Vector4<float> Vector4f;
	typedef Vector4<double> Vector4d;
	typedef Vector4<int> Vector4i;

};

namespace CU = CommonUtilities;

#define ZEROV CU::Vector3f(0,0,0);

#endif