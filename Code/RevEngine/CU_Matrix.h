#ifndef I3M_MATRIX_HEADER
#define I3M_MATRIX_HEADER

#include "cu_vector.h"
#include <DirectXMath.h>

namespace CommonUtilities
{
	template <class TYPE>
	class Matrix44;

	template <class TYPE>
	class Matrix33
	{
	public:
		Matrix33() 
		{
			myMatrix[0][0]=1;
			myMatrix[1][0]=0;
			myMatrix[2][0]=0;
			myMatrix[0][1]=0;
			myMatrix[1][1]=1;
			myMatrix[2][1]=0;
			myMatrix[0][2]=0;
			myMatrix[1][2]=0;
			myMatrix[2][2]=1;
		};

		~Matrix33() 
		{
		};

		Matrix33(	TYPE o0,
			TYPE o1,
			TYPE o2,
			TYPE o3,
			TYPE o4,
			TYPE o5,
			TYPE o6,
			TYPE o7,
			TYPE o8)
		{
			myMatrix[0][0] = o0;
			myMatrix[0][1] = o1;
			myMatrix[0][2] = o2;
			myMatrix[1][0] = o3;
			myMatrix[1][1] = o4;
			myMatrix[1][2] = o5;
			myMatrix[2][0] = o6;
			myMatrix[2][1] = o7;
			myMatrix[2][2] = o8;
		}

		void ConvertD3D(TYPE* aMatrix) 
		{
			myMatrix[0][0]=aMatrix[0];
			myMatrix[0][1]=aMatrix[1];
			myMatrix[0][2]=aMatrix[2];
			myMatrix[1][0]=aMatrix[3];
			myMatrix[1][1]=aMatrix[4];
			myMatrix[1][2]=aMatrix[5];
			myMatrix[2][0]=aMatrix[6];
			myMatrix[2][1]=aMatrix[7];
			myMatrix[2][2]=aMatrix[8];
		}


		Matrix33 operator~() const;			// take the  inverse of the matrice

		Matrix33 operator*(const Matrix33& aMatrix);		// matrice multiplikation
		Matrix33 operator+(const Matrix33& aMatrix);		// matrice addition
		Matrix33 operator-(const Matrix33& aMatrix);		// matrice subtraction
		Matrix33& operator*=(const Matrix33& aMatrix);	// matrice multiplikation
		Matrix33& operator+=(const Matrix33& aMatrix);	// matrice addition
		Matrix33& operator-=(const Matrix33& aMatrix);	// matrice subtraction
		Matrix33 RotateTowardsDir(CU::Vector3f aVector);
		CU::Vector3f GetEulers();
		CU::Vector3f GetRotationVector();
		Matrix33& operator=(Matrix44<TYPE>& aMatrix);

		static Matrix33 RotateVectorToVector(Vector3<TYPE> aV, Vector3<TYPE> aNotherV);
		static Matrix33 RotateAroundXPI(const float aPIAngle);
		static Matrix33 RotateAroundYPI(const float aPIAngle);
		static Matrix33 RotateAroundZPI(const float aPIAngle);
		static Matrix33 RotationAroundVector(Vector3<TYPE> aVector,TYPE aAngle);

		static Matrix33 Identity();
		void SetIdentity();
		Matrix33 Transpose();
		static Matrix33 RotateTowardsVector2(CU::Vector2f point, const float aPIAngle);
		TYPE myMatrix[3][3]; 
	};

	template <class TYPE>
	CU::Vector3f Matrix33<TYPE>::GetRotationVector()
	{	
		CU::Vector3f returnVector;
		returnVector.x += myMatrix [0][0];
		returnVector.x += myMatrix [0][1];
		returnVector.x += myMatrix [0][2];

		returnVector.x += myMatrix [1][0];
		returnVector.y += myMatrix [1][1];
		returnVector.z += myMatrix [1][2];

		returnVector.x += myMatrix [2][0];
		returnVector.y += myMatrix [2][1];
		returnVector.z += myMatrix [2][2];



		return returnVector;
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::Transpose()
	{	
		Matrix33<TYPE> returnMatrix;
		returnMatrix.myMatrix[0][0] = myMatrix [0][0];
		returnMatrix.myMatrix[1][0] = myMatrix [0][1];
		returnMatrix.myMatrix[2][0] = myMatrix [0][2];

		returnMatrix.myMatrix[0][1] = myMatrix [1][0];
		returnMatrix.myMatrix[1][1] = myMatrix [1][1];
		returnMatrix.myMatrix[2][1] = myMatrix [1][2];

		returnMatrix.myMatrix[0][2] = myMatrix [2][0];
		returnMatrix.myMatrix[1][2] = myMatrix [2][1];
		returnMatrix.myMatrix[2][2] = myMatrix [2][2];



		return returnMatrix;
	}
	template <class TYPE>
	void Matrix33<TYPE>::SetIdentity()
	{
		myMatrix[0][0]=1;
		myMatrix[1][0]=0;
		myMatrix[2][0]=0;
		myMatrix[0][1]=0;
		myMatrix[1][1]=1;
		myMatrix[2][1]=0;
		myMatrix[0][2]=0;
		myMatrix[1][2]=0;
		myMatrix[2][2]=1;
	}


	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::RotateTowardsDir(CU::Vector3f aVector)
	{
		Matrix33<TYPE> temp;
		CU::Vector3f direction = aVector.Normalize();

		CU::Vector3f up(0.0f, 1.0f, 0.0f);
		if(direction.y == 1.0f)
		{
			direction.y = 0.99f;
		}
		if(direction.y == -1.0f)
		{
			direction.y = -0.99f;
		}

		CU::Vector3f right(direction.Cross(up));
		right = right.Normalize();
		up = direction.Cross(right);

		temp.myMatrix[0][0] = right.x;
		temp.myMatrix[0][1] = right.y;
		temp.myMatrix[0][2] = right.z;
		/*temp.myMatrix[1][0] = up.x;s
		temp.myMatrix[1][1] = up.y;
		temp.myMatrix[1][2] = up.z;*/
		temp.myMatrix[2][0] = direction.x;
		temp.myMatrix[2][1] = direction.y;
		temp.myMatrix[2][2] = direction.z;
		return temp;

	}

	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::RotationAroundVector(Vector3<TYPE> aVector,TYPE aAngle)
	{
		Matrix33 temp;
		float Sin,Cos;

		Sin=(float)sin(aAngle);
		Cos=(float)cos(aAngle);

		temp.myMatrix[0][0]=aVector.x*aVector.x*(1-Cos)+Cos;
		temp.myMatrix[1][0]=aVector.x*aVector.y*(1-Cos)-aVector.z*Sin;
		temp.myMatrix[2][0]=aVector.x*aVector.z*(1-Cos)+aVector.y*Sin;

		temp.myMatrix[0][1]=-aVector.y*aVector.x*(1-Cos)+aVector.z*Sin;
		temp.myMatrix[1][1]=aVector.y*aVector.y*(1-Cos)+Cos;
		temp.myMatrix[2][1]=aVector.y*aVector.z*(1-Cos)-aVector.x*Sin;

		temp.myMatrix[0][2]=-aVector.x*aVector.z*(1-Cos)-aVector.y*Sin;
		temp.myMatrix[1][2]=aVector.y*aVector.z*(1-Cos)+aVector.x*Sin;
		temp.myMatrix[2][2]=aVector.z*aVector.z*(1-Cos)+Cos;



		return(temp);
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::operator~() const			// take the  inverse of the matrice
	{
		return Matrix33(myMatrix[0][0] , myMatrix[0][1], myMatrix[0][2],  
			myMatrix[1][0] , myMatrix[1][1], myMatrix[1][2],
			myMatrix[2][0] , myMatrix[2][1], myMatrix[2][2]);  
	}

	template <class TYPE>	
	Matrix33<TYPE>& Matrix33<TYPE>::operator=(Matrix44<TYPE>& aMatrix) 
	{
		myMatrix[0][0]=aMatrix.myMatrix[0][0];
		myMatrix[0][1]=aMatrix.myMatrix[0][1];
		myMatrix[0][2]=aMatrix.myMatrix[0][2];
		myMatrix[1][0]=aMatrix.myMatrix[1][0];
		myMatrix[1][1]=aMatrix.myMatrix[1][1];
		myMatrix[1][2]=aMatrix.myMatrix[1][2];
		myMatrix[2][0]=aMatrix.myMatrix[2][0];
		myMatrix[2][1]=aMatrix.myMatrix[2][1];
		myMatrix[2][2]=aMatrix.myMatrix[2][2];
		return(*this);
	}



	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::operator*(const Matrix33& aMatrix)	// matrice multiplikation
	{
		return Matrix33
			(myMatrix[0][0]*aMatrix.myMatrix[0][0]+myMatrix[1][0]*aMatrix.myMatrix[0][1]+myMatrix[2][0]*aMatrix.myMatrix[0][2],
			myMatrix[0][0]*aMatrix.myMatrix[1][0]+myMatrix[1][0]*aMatrix.myMatrix[1][1]+myMatrix[2][0]*aMatrix.myMatrix[1][2],
			myMatrix[0][0]*aMatrix.myMatrix[2][0]+myMatrix[1][0]*aMatrix.myMatrix[2][1]+myMatrix[2][0]*aMatrix.myMatrix[2][2],
			myMatrix[0][1]*aMatrix.myMatrix[0][0]+myMatrix[1][1]*aMatrix.myMatrix[0][1]+myMatrix[2][1]*aMatrix.myMatrix[0][2],
			myMatrix[0][1]*aMatrix.myMatrix[1][0]+myMatrix[1][1]*aMatrix.myMatrix[1][1]+myMatrix[2][1]*aMatrix.myMatrix[1][2],
			myMatrix[0][1]*aMatrix.myMatrix[2][0]+myMatrix[1][1]*aMatrix.myMatrix[2][1]+myMatrix[2][1]*aMatrix.myMatrix[2][2],
			myMatrix[0][2]*aMatrix.myMatrix[0][0]+myMatrix[1][2]*aMatrix.myMatrix[0][1]+myMatrix[2][2]*aMatrix.myMatrix[0][2],
			myMatrix[0][2]*aMatrix.myMatrix[1][0]+myMatrix[1][2]*aMatrix.myMatrix[1][1]+myMatrix[2][2]*aMatrix.myMatrix[1][2],
			myMatrix[0][2]*aMatrix.myMatrix[2][0]+myMatrix[1][2]*aMatrix.myMatrix[2][1]+myMatrix[2][2]*aMatrix.myMatrix[2][2]);
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::operator-(const Matrix33& aMatrix)	// matrice subtraction
	{
		return Matrix33(
			myMatrix[0][0] - aMatrix.myMatrix[0][0] , myMatrix[1][0] - aMatrix.myMatrix[1][0]
		,myMatrix[2][0] - aMatrix.myMatrix[2][0] , myMatrix[0][1] - aMatrix.myMatrix[0][1]  
		,myMatrix[1][1] - aMatrix.myMatrix[1][1] , myMatrix[2][1] - aMatrix.myMatrix[2][1] 
		,myMatrix[0][2] - aMatrix.myMatrix[0][2] , myMatrix[1][2] - aMatrix.myMatrix[1][2] 
		,myMatrix[2][2] - aMatrix.myMatrix[2][2] );  

	}


	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::operator+(const Matrix33& aMatrix)	// matrice addition
	{
		return Matrix33(
			myMatrix[0][0] + aMatrix.myMatrix[0][0] , myMatrix[1][0] + aMatrix.myMatrix[1][0]
		,myMatrix[2][0] + aMatrix.myMatrix[2][0] , myMatrix[0][1] + aMatrix.myMatrix[0][1]  
		,myMatrix[1][1] + aMatrix.myMatrix[1][1] , myMatrix[2][1] + aMatrix.myMatrix[2][1] 
		,myMatrix[0][2] + aMatrix.myMatrix[0][2] , myMatrix[1][2] + aMatrix.myMatrix[1][2] 
		,myMatrix[2][2] + aMatrix.myMatrix[2][2] );  

	}


	template <class TYPE>
	Matrix33<TYPE>& Matrix33<TYPE>::operator*=(const Matrix33& aMatrix)	// matrice multiplikation
	{
		Matrix33 temp;


		temp.myMatrix[0][0]=myMatrix[0][0]*aMatrix.myMatrix[0][0]+myMatrix[0][1]*aMatrix.myMatrix[1][0]+myMatrix[0][2]*aMatrix.myMatrix[2][0];
		temp.myMatrix[0][1]=myMatrix[0][0]*aMatrix.myMatrix[0][1]+myMatrix[0][1]*aMatrix.myMatrix[1][1]+myMatrix[0][2]*aMatrix.myMatrix[2][1];
		temp.myMatrix[0][2]=myMatrix[0][0]*aMatrix.myMatrix[0][2]+myMatrix[0][1]*aMatrix.myMatrix[1][2]+myMatrix[0][2]*aMatrix.myMatrix[2][2];

		temp.myMatrix[1][0]=myMatrix[1][0]*aMatrix.myMatrix[0][0]+myMatrix[1][1]*aMatrix.myMatrix[1][0]+myMatrix[1][2]*aMatrix.myMatrix[2][0];
		temp.myMatrix[1][1]=myMatrix[1][0]*aMatrix.myMatrix[0][1]+myMatrix[1][1]*aMatrix.myMatrix[1][1]+myMatrix[1][2]*aMatrix.myMatrix[2][1];
		temp.myMatrix[1][2]=myMatrix[1][0]*aMatrix.myMatrix[0][2]+myMatrix[1][1]*aMatrix.myMatrix[1][2]+myMatrix[1][2]*aMatrix.myMatrix[2][2];

		temp.myMatrix[2][0]=myMatrix[2][0]*aMatrix.myMatrix[0][0]+myMatrix[2][1]*aMatrix.myMatrix[1][0]+myMatrix[2][2]*aMatrix.myMatrix[2][0];
		temp.myMatrix[2][1]=myMatrix[2][0]*aMatrix.myMatrix[0][1]+myMatrix[2][1]*aMatrix.myMatrix[1][1]+myMatrix[2][2]*aMatrix.myMatrix[2][1];
		temp.myMatrix[2][2]=myMatrix[2][0]*aMatrix.myMatrix[0][2]+myMatrix[2][1]*aMatrix.myMatrix[1][2]+myMatrix[2][2]*aMatrix.myMatrix[2][2];

		memcpy(myMatrix,temp.myMatrix,sizeof(Matrix33<TYPE>));



		return *this;
	}

	template <class TYPE>
	Matrix33<TYPE>& Matrix33<TYPE>::operator+=(const Matrix33& aMatrix)	// matrice addition
	{
		myMatrix[0][0]+=aMatrix.myMatrix[0][0];
		myMatrix[1][0]+=aMatrix.myMatrix[1][0];
		myMatrix[2][0]+=aMatrix.myMatrix[2][0];
		myMatrix[0][1]+=aMatrix.myMatrix[0][1];
		myMatrix[1][1]+=aMatrix.myMatrix[1][1];
		myMatrix[2][1]+=aMatrix.myMatrix[2][1];
		myMatrix[0][2]+=aMatrix.myMatrix[0][2];
		myMatrix[1][2]+=aMatrix.myMatrix[1][2];
		myMatrix[2][2]+=aMatrix.myMatrix[2][2];

		return *this;

	}

	template <class TYPE>
	Matrix33<TYPE>& Matrix33<TYPE>::operator-=(const Matrix33& aMatrix)	// matrice subtraction
	{
		myMatrix[0][0]-=aMatrix.myMatrix[0][0];
		myMatrix[1][0]-=aMatrix.myMatrix[1][0];
		myMatrix[2][0]-=aMatrix.myMatrix[2][0];
		myMatrix[0][1]-=aMatrix.myMatrix[0][1];
		myMatrix[1][1]-=aMatrix.myMatrix[1][1];
		myMatrix[2][1]-=aMatrix.myMatrix[2][1];
		myMatrix[0][2]-=aMatrix.myMatrix[0][2];
		myMatrix[1][2]-=aMatrix.myMatrix[1][2];
		myMatrix[2][2]-=aMatrix.myMatrix[2][2];

		return *this;
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::RotateAroundXPI(const float aPIAngle)
	{
		Matrix33 temp;
		float Sin,Cos;

		Sin=(float)sin(aPIAngle);
		Cos=(float)cos(aPIAngle);

		temp.myMatrix[1][1]=Cos;
		temp.myMatrix[2][1]=Sin;
		temp.myMatrix[2][2]=Cos;
		temp.myMatrix[1][2]=-Sin;

		
		return(temp);
	}
	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::RotateTowardsVector2(CU::Vector2f point, const float aPIAngle)
	{
		Matrix33 temp;
		float Sin,Cos;

		Sin=(float)sin(aPIAngle);
		Cos=(float)cos(aPIAngle);

		temp.myMatrix[0][0]=Cos;
		temp.myMatrix[2][0]=-Sin;
		temp.myMatrix[2][2]=Cos;
		temp.myMatrix[0][2]=Sin;

		return(temp);
	}
	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::RotateAroundYPI(const float aPIAngle)
	{
		Matrix33 temp;
		float Sin,Cos;

		Sin=(float)sin(aPIAngle);
		Cos=(float)cos(aPIAngle);

		temp.myMatrix[0][0]=Cos;
		temp.myMatrix[2][0]=-Sin;
		temp.myMatrix[2][2]=Cos;
		temp.myMatrix[0][2]=Sin;

		return(temp);

	}

	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::RotateAroundZPI(const float aPIAngle)
	{
		Matrix33 temp;
		float Sin,Cos;

		Sin=(float)sin(aPIAngle);
		Cos=(float)cos(aPIAngle);

		temp.myMatrix[0][0]=Cos;
		temp.myMatrix[1][0]=Sin;
		temp.myMatrix[1][1]=Cos;
		temp.myMatrix[0][1]=-Sin;


		return(temp);
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::Identity()
	{
		Matrix33 temp;
		return(temp);
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix33<TYPE>::RotateVectorToVector(Vector3<TYPE> aV, Vector3<TYPE> aNotherV)
	{
		Matrix33<TYPE> M;  
		aV.Normalize();
		M.myMatrix[0][0] = aV.x;
		M.myMatrix[1][0] = aV.y;
		M.myMatrix[2][0] = aV.z;
		
		CU::Vector3f v2 =  aV.Cross(aNotherV).Normalize();
		M.myMatrix[0][2] = v2.x;
		M.myMatrix[1][2] = v2.y;
		M.myMatrix[2][2] = v2.z;

		CU::Vector3f norm(M.myMatrix[0][2], M.myMatrix[1][2], M.myMatrix[2][2]);
		
		CU::Vector3f v3 =  norm.Cross(aV).Normalize();

		M.myMatrix[0][1] = v3.x;
		M.myMatrix[1][1] = v3.y;
		M.myMatrix[2][1] = v3.z;

		return M;
	}

	template <class TYPE>
	class Matrix44
	{
	public:
		Matrix44(TYPE* aMatrix) 
		{
			myMatrix[0][0]=aMatrix[0];
			myMatrix[1][0]=aMatrix[1];
			myMatrix[2][0]=aMatrix[2];
			myMatrix[3][0]=aMatrix[3];
			myMatrix[0][1]=aMatrix[4];
			myMatrix[1][1]=aMatrix[5];
			myMatrix[2][1]=aMatrix[6];
			myMatrix[3][1]=aMatrix[7];
			myMatrix[0][2]=aMatrix[8];
			myMatrix[1][2]=aMatrix[9];
			myMatrix[2][2]=aMatrix[10];
			myMatrix[3][2]=aMatrix[11];
			myMatrix[0][3]=aMatrix[12];
			myMatrix[1][3]=aMatrix[13];
			myMatrix[2][3]=aMatrix[14];
			myMatrix[3][3]=aMatrix[15];
		}


		void ConvertD3D(TYPE* aMatrix) 
		{
			myMatrix[0][0]=aMatrix[0];
			myMatrix[0][1]=aMatrix[1];
			myMatrix[0][2]=aMatrix[2];
			myMatrix[0][3]=aMatrix[3];
			myMatrix[1][0]=aMatrix[4];
			myMatrix[1][1]=aMatrix[5];
			myMatrix[1][2]=aMatrix[6];
			myMatrix[1][3]=aMatrix[7];
			myMatrix[2][0]=aMatrix[8];
			myMatrix[2][1]=aMatrix[9];
			myMatrix[2][2]=aMatrix[10];
			myMatrix[2][3]=aMatrix[11];
			myMatrix[3][0]=aMatrix[12];
			myMatrix[3][1]=aMatrix[13];
			myMatrix[3][2]=aMatrix[14];
			myMatrix[3][3]=aMatrix[15];
		}


		Matrix44(float m1,float m2,float m3,float m4,float m5,float m6,float m7,float m8,float m9,float m10,float m11,float m12,float m13,float m14,float m15,float m16) 
		{
			myMatrix[0][0]=m1;
			myMatrix[1][0]=m2;
			myMatrix[2][0]=m3;
			myMatrix[3][0]=m4;
			myMatrix[0][1]=m5;
			myMatrix[1][1]=m6;
			myMatrix[2][1]=m7;
			myMatrix[3][1]=m8;
			myMatrix[0][2]=m9;
			myMatrix[1][2]=m10;
			myMatrix[2][2]=m11;
			myMatrix[3][2]=m12;
			myMatrix[0][3]=m13;
			myMatrix[1][3]=m14;
			myMatrix[2][3]=m15;
			myMatrix[3][3]=m16;
		}

		Matrix44() 
		{
			myMatrix[0][0]=1;
			myMatrix[1][0]=0;
			myMatrix[2][0]=0;
			myMatrix[3][0]=0;
			myMatrix[0][1]=0;
			myMatrix[1][1]=1;
			myMatrix[2][1]=0;
			myMatrix[3][1]=0;
			myMatrix[0][2]=0;
			myMatrix[1][2]=0;
			myMatrix[2][2]=1;
			myMatrix[3][2]=0;
			myMatrix[0][3]=0;
			myMatrix[1][3]=0;
			myMatrix[2][3]=0;
			myMatrix[3][3]=1;
		};

		~Matrix44() 
		{
		};

		Matrix44 operator~() const;			// take the  inverse of the matrice

		Matrix44 operator*(const Matrix44& aSecondMatrix) const;		// matrice multiplikation
		Matrix44& operator*=(const Matrix44& aMatrix);	// matrice multiplikation

		Matrix44& operator*=(const Vector4<TYPE>& aVector);		// matrice multiplikation
		Matrix44 operator*(const Matrix33<TYPE>& aMatrix);		// matrice multiplikation
		Matrix44& operator*=(const Matrix33<TYPE>& aMatrix);	// matrice multiplikation
		Matrix44& operator=(const Matrix33<TYPE>& aMatrix);	
		//Matrix44 RealInverse();
		Matrix44 Real4X4Inverse();
		float Detetermant();
		static Matrix44 RotateAroundXPI(const float aPIAngle);
		static Matrix44 RotateAroundYPI(const float aPIAngle);
		static Matrix44 RotateAroundZPI(const float aPIAngle);
		Matrix33<TYPE> RotateTowardsVector(const CU::Vector3f aVector); 
		Matrix33<TYPE> RotateTowardsDir(CU::Vector3f aVector); 
		void RotateXYZ(const float& anX, const float& anY, const float& anZ);
		static Matrix44 Identity();
		void SetIdentity();
		Vector3<TYPE> GetPositon();
		void SetPosition(const CU::Vector3f& aPosition);
		void SetRotation(const Matrix33<TYPE>& aMatrix);	
		bool operator==(const CU::Matrix44<TYPE>& aMatrix);
		DirectX::XMMATRIX GetD3DMatrix();


		Matrix33<TYPE> RotateTowardsDirYawPitch(CU::Vector3f aVector);

		Vector3<TYPE> GetPosition3();

		Matrix33<TYPE> Get33();
		Matrix44<TYPE> Transpose();
		Vector4<TYPE> GetPosition();

		TYPE myMatrix[4][4]; 
	};

	template <class TYPE>
	void Matrix44<TYPE>::SetIdentity()
	{
		myMatrix[0][0]=1;
		myMatrix[1][0]=0;
		myMatrix[2][0]=0;
		myMatrix[3][0]=0;
		myMatrix[0][1]=0;
		myMatrix[1][1]=1;
		myMatrix[2][1]=0;
		myMatrix[3][1]=0;
		myMatrix[0][2]=0;
		myMatrix[1][2]=0;
		myMatrix[2][2]=1;
		myMatrix[3][2]=0;
		myMatrix[0][3]=0;
		myMatrix[1][3]=0;
		myMatrix[2][3]=0;
		myMatrix[3][3]=1;
	}


	template <class TYPE>
	Matrix44<TYPE> Matrix44<TYPE>::operator~() const			// take the  inverse of the matrice
	{
		CU::Vector3f invPos;
		invPos.x=(-myMatrix[3][0]*myMatrix[0][0])+(-myMatrix[3][1]*myMatrix[0][1])+(-myMatrix[3][2]*myMatrix[0][2]);
		invPos.y=(-myMatrix[3][0]*myMatrix[1][0])+(-myMatrix[3][1]*myMatrix[1][1])+(-myMatrix[3][2]*myMatrix[1][2]);
		invPos.z=(-myMatrix[3][0]*myMatrix[2][0])+(-myMatrix[3][1]*myMatrix[2][1])+(-myMatrix[3][2]*myMatrix[2][2]);

		return Matrix44(myMatrix[0][0] , myMatrix[0][1], myMatrix[0][2],  invPos.x,
			myMatrix[1][0] , myMatrix[1][1], myMatrix[1][2] ,invPos.y,
			myMatrix[2][0] , myMatrix[2][1], myMatrix[2][2] ,invPos.z,
			-myMatrix[0][3] , -myMatrix[1][3], -myMatrix[2][3] ,myMatrix[3][3]);  
	}
	/*

	template <class TYPE>
	Matrix44<TYPE> Matrix44<TYPE>::operator~() const			// take the  inverse of the matrice
	{
	return Matrix44(myMatrix[0][0] , myMatrix[0][1], myMatrix[0][2],  myMatrix[0][3],
	myMatrix[1][0] , myMatrix[1][1], myMatrix[1][2] ,myMatrix[1][3],
	myMatrix[2][0] , myMatrix[2][1], myMatrix[2][2] ,myMatrix[2][3],
	myMatrix[3][0] , myMatrix[3][1], myMatrix[3][2] ,myMatrix[3][3]);  
	}
	*/

	template <class TYPE>
	bool Matrix44<TYPE>::operator==(const CU::Matrix44<TYPE>& aMatrix)
	{

		if(myMatrix[0][0] != aMatrix.myMatrix[0][0]) return false;
		if(myMatrix[0][1] != aMatrix.myMatrix[0][1]) return false;
		if(myMatrix[0][2] != aMatrix.myMatrix[0][2]) return false;
		if(myMatrix[0][3] != aMatrix.myMatrix[0][3]) return false;
		if(myMatrix[1][0] != aMatrix.myMatrix[1][0]) return false;
		if(myMatrix[1][1] != aMatrix.myMatrix[1][1]) return false;
		if(myMatrix[1][2] != aMatrix.myMatrix[1][2]) return false;
		if(myMatrix[1][3] != aMatrix.myMatrix[1][3]) return false;
		if(myMatrix[2][0] != aMatrix.myMatrix[2][0]) return false;
		if(myMatrix[2][1] != aMatrix.myMatrix[2][1]) return false;
		if(myMatrix[2][2] != aMatrix.myMatrix[2][2]) return false;
		if(myMatrix[2][3] != aMatrix.myMatrix[2][3]) return false;
		if(myMatrix[3][0] != aMatrix.myMatrix[3][0]) return false;
		if(myMatrix[3][1] != aMatrix.myMatrix[3][1]) return false;
		if(myMatrix[3][2] != aMatrix.myMatrix[3][2]) return false;
		if(myMatrix[3][3] != aMatrix.myMatrix[3][3]) return false;
		return true;
	}

	template <class TYPE>
	Matrix44<TYPE> Matrix44<TYPE>::operator*(const Matrix44& aSecondMatrix) const	// matrice multiplikation
	{
		Matrix44 temp;

		return(Matrix44(
			myMatrix[0][0]*aSecondMatrix.myMatrix[0][0]+myMatrix[0][1]*aSecondMatrix.myMatrix[1][0]+myMatrix[0][2]*aSecondMatrix.myMatrix[2][0]+myMatrix[0][3]*aSecondMatrix.myMatrix[3][0],
			myMatrix[1][0]*aSecondMatrix.myMatrix[0][0]+myMatrix[1][1]*aSecondMatrix.myMatrix[1][0]+myMatrix[1][2]*aSecondMatrix.myMatrix[2][0]+myMatrix[1][3]*aSecondMatrix.myMatrix[3][0],
			myMatrix[2][0]*aSecondMatrix.myMatrix[0][0]+myMatrix[2][1]*aSecondMatrix.myMatrix[1][0]+myMatrix[2][2]*aSecondMatrix.myMatrix[2][0]+myMatrix[2][3]*aSecondMatrix.myMatrix[3][0],
			myMatrix[3][0]*aSecondMatrix.myMatrix[0][0]+myMatrix[3][1]*aSecondMatrix.myMatrix[1][0]+myMatrix[3][2]*aSecondMatrix.myMatrix[2][0]+myMatrix[3][3]*aSecondMatrix.myMatrix[3][0],

			myMatrix[0][0]*aSecondMatrix.myMatrix[0][1]+myMatrix[0][1]*aSecondMatrix.myMatrix[1][1]+myMatrix[0][2]*aSecondMatrix.myMatrix[2][1]+myMatrix[0][3]*aSecondMatrix.myMatrix[3][1],
			myMatrix[1][0]*aSecondMatrix.myMatrix[0][1]+myMatrix[1][1]*aSecondMatrix.myMatrix[1][1]+myMatrix[1][2]*aSecondMatrix.myMatrix[2][1]+myMatrix[1][3]*aSecondMatrix.myMatrix[3][1],
			myMatrix[2][0]*aSecondMatrix.myMatrix[0][1]+myMatrix[2][1]*aSecondMatrix.myMatrix[1][1]+myMatrix[2][2]*aSecondMatrix.myMatrix[2][1]+myMatrix[2][3]*aSecondMatrix.myMatrix[3][1],
			myMatrix[3][0]*aSecondMatrix.myMatrix[0][1]+myMatrix[3][1]*aSecondMatrix.myMatrix[1][1]+myMatrix[3][2]*aSecondMatrix.myMatrix[2][1]+myMatrix[3][3]*aSecondMatrix.myMatrix[3][1],

			myMatrix[0][0]*aSecondMatrix.myMatrix[0][2]+myMatrix[0][1]*aSecondMatrix.myMatrix[1][2]+myMatrix[0][2]*aSecondMatrix.myMatrix[2][2]+myMatrix[0][3]*aSecondMatrix.myMatrix[3][2],
			myMatrix[1][0]*aSecondMatrix.myMatrix[0][2]+myMatrix[1][1]*aSecondMatrix.myMatrix[1][2]+myMatrix[1][2]*aSecondMatrix.myMatrix[2][2]+myMatrix[1][3]*aSecondMatrix.myMatrix[3][2],
			myMatrix[2][0]*aSecondMatrix.myMatrix[0][2]+myMatrix[2][1]*aSecondMatrix.myMatrix[1][2]+myMatrix[2][2]*aSecondMatrix.myMatrix[2][2]+myMatrix[2][3]*aSecondMatrix.myMatrix[3][2],
			myMatrix[3][0]*aSecondMatrix.myMatrix[0][2]+myMatrix[3][1]*aSecondMatrix.myMatrix[1][2]+myMatrix[3][2]*aSecondMatrix.myMatrix[2][2]+myMatrix[3][3]*aSecondMatrix.myMatrix[3][2],

			myMatrix[0][0]*aSecondMatrix.myMatrix[0][3]+myMatrix[0][1]*aSecondMatrix.myMatrix[1][3]+myMatrix[0][2]*aSecondMatrix.myMatrix[2][3]+myMatrix[0][3]*aSecondMatrix.myMatrix[3][3],
			myMatrix[1][0]*aSecondMatrix.myMatrix[0][3]+myMatrix[1][1]*aSecondMatrix.myMatrix[1][3]+myMatrix[1][2]*aSecondMatrix.myMatrix[2][3]+myMatrix[1][3]*aSecondMatrix.myMatrix[3][3],
			myMatrix[2][0]*aSecondMatrix.myMatrix[0][3]+myMatrix[2][1]*aSecondMatrix.myMatrix[1][3]+myMatrix[2][2]*aSecondMatrix.myMatrix[2][3]+myMatrix[2][3]*aSecondMatrix.myMatrix[3][3],
			myMatrix[3][0]*aSecondMatrix.myMatrix[0][3]+myMatrix[3][1]*aSecondMatrix.myMatrix[1][3]+myMatrix[3][2]*aSecondMatrix.myMatrix[2][3]+myMatrix[3][3]*aSecondMatrix.myMatrix[3][3]));
	}


	template <class TYPE>
	Matrix44<TYPE>& Matrix44<TYPE>::operator*=(const Matrix44& aMatrix)	// matrice multiplikation
	{
		Matrix44 temp;

		temp.myMatrix[0][0]=myMatrix[0][0]*aMatrix.myMatrix[0][0]+myMatrix[0][1]*aMatrix.myMatrix[1][0]+myMatrix[0][2]*aMatrix.myMatrix[2][0]+myMatrix[0][3]*aMatrix.myMatrix[3][0];
		temp.myMatrix[0][1]=myMatrix[0][0]*aMatrix.myMatrix[0][1]+myMatrix[0][1]*aMatrix.myMatrix[1][1]+myMatrix[0][2]*aMatrix.myMatrix[2][1]+myMatrix[0][3]*aMatrix.myMatrix[3][1];
		temp.myMatrix[0][2]=myMatrix[0][0]*aMatrix.myMatrix[0][2]+myMatrix[0][1]*aMatrix.myMatrix[1][2]+myMatrix[0][2]*aMatrix.myMatrix[2][2]+myMatrix[0][3]*aMatrix.myMatrix[3][2];
		temp.myMatrix[0][3]=myMatrix[0][0]*aMatrix.myMatrix[0][3]+myMatrix[0][1]*aMatrix.myMatrix[1][3]+myMatrix[0][2]*aMatrix.myMatrix[2][3]+myMatrix[0][3]*aMatrix.myMatrix[3][3];

		temp.myMatrix[1][0]=myMatrix[1][0]*aMatrix.myMatrix[0][0]+myMatrix[1][1]*aMatrix.myMatrix[1][0]+myMatrix[1][2]*aMatrix.myMatrix[2][0]+myMatrix[1][3]*aMatrix.myMatrix[3][0];
		temp.myMatrix[1][1]=myMatrix[1][0]*aMatrix.myMatrix[0][1]+myMatrix[1][1]*aMatrix.myMatrix[1][1]+myMatrix[1][2]*aMatrix.myMatrix[2][1]+myMatrix[1][3]*aMatrix.myMatrix[3][1];
		temp.myMatrix[1][2]=myMatrix[1][0]*aMatrix.myMatrix[0][2]+myMatrix[1][1]*aMatrix.myMatrix[1][2]+myMatrix[1][2]*aMatrix.myMatrix[2][2]+myMatrix[1][3]*aMatrix.myMatrix[3][2];
		temp.myMatrix[1][3]=myMatrix[1][0]*aMatrix.myMatrix[0][3]+myMatrix[1][1]*aMatrix.myMatrix[1][3]+myMatrix[1][2]*aMatrix.myMatrix[2][3]+myMatrix[1][3]*aMatrix.myMatrix[3][3];

		temp.myMatrix[2][0]=myMatrix[2][0]*aMatrix.myMatrix[0][0]+myMatrix[2][1]*aMatrix.myMatrix[1][0]+myMatrix[2][2]*aMatrix.myMatrix[2][0]+myMatrix[2][3]*aMatrix.myMatrix[3][0];
		temp.myMatrix[2][1]=myMatrix[2][0]*aMatrix.myMatrix[0][1]+myMatrix[2][1]*aMatrix.myMatrix[1][1]+myMatrix[2][2]*aMatrix.myMatrix[2][1]+myMatrix[2][3]*aMatrix.myMatrix[3][1];
		temp.myMatrix[2][2]=myMatrix[2][0]*aMatrix.myMatrix[0][2]+myMatrix[2][1]*aMatrix.myMatrix[1][2]+myMatrix[2][2]*aMatrix.myMatrix[2][2]+myMatrix[2][3]*aMatrix.myMatrix[3][2];
		temp.myMatrix[2][3]=myMatrix[2][0]*aMatrix.myMatrix[0][3]+myMatrix[2][1]*aMatrix.myMatrix[1][3]+myMatrix[2][2]*aMatrix.myMatrix[2][3]+myMatrix[2][3]*aMatrix.myMatrix[3][3];

		temp.myMatrix[3][0]=myMatrix[3][0]*aMatrix.myMatrix[0][0]+myMatrix[3][1]*aMatrix.myMatrix[1][0]+myMatrix[3][2]*aMatrix.myMatrix[2][0]+myMatrix[3][3]*aMatrix.myMatrix[3][0];
		temp.myMatrix[3][1]=myMatrix[3][0]*aMatrix.myMatrix[0][1]+myMatrix[3][1]*aMatrix.myMatrix[1][1]+myMatrix[3][2]*aMatrix.myMatrix[2][1]+myMatrix[3][3]*aMatrix.myMatrix[3][1];
		temp.myMatrix[3][2]=myMatrix[3][0]*aMatrix.myMatrix[0][2]+myMatrix[3][1]*aMatrix.myMatrix[1][2]+myMatrix[3][2]*aMatrix.myMatrix[2][2]+myMatrix[3][3]*aMatrix.myMatrix[3][2];
		temp.myMatrix[3][3]=myMatrix[3][0]*aMatrix.myMatrix[0][3]+myMatrix[3][1]*aMatrix.myMatrix[1][3]+myMatrix[3][2]*aMatrix.myMatrix[2][3]+myMatrix[3][3]*aMatrix.myMatrix[3][3];

		memcpy(myMatrix,temp.myMatrix,4*4*4);

		return *this;
	}



	template <class TYPE>
	Matrix44<TYPE>& Matrix44<TYPE>::operator*=(const Matrix33<TYPE>& aMatrix)	// matrice multiplikation
	{
		Matrix44 temp;



		temp.myMatrix[0][0]=myMatrix[0][0]*aMatrix.myMatrix[0][0]+myMatrix[0][1]*aMatrix.myMatrix[1][0]+myMatrix[0][2]*aMatrix.myMatrix[2][0];
		temp.myMatrix[0][1]=myMatrix[0][0]*aMatrix.myMatrix[0][1]+myMatrix[0][1]*aMatrix.myMatrix[1][1]+myMatrix[0][2]*aMatrix.myMatrix[2][1];
		temp.myMatrix[0][2]=myMatrix[0][0]*aMatrix.myMatrix[0][2]+myMatrix[0][1]*aMatrix.myMatrix[1][2]+myMatrix[0][2]*aMatrix.myMatrix[2][2];
		temp.myMatrix[0][3]=myMatrix[0][3];

		temp.myMatrix[1][0]=myMatrix[1][0]*aMatrix.myMatrix[0][0]+myMatrix[1][1]*aMatrix.myMatrix[1][0]+myMatrix[1][2]*aMatrix.myMatrix[2][0];
		temp.myMatrix[1][1]=myMatrix[1][0]*aMatrix.myMatrix[0][1]+myMatrix[1][1]*aMatrix.myMatrix[1][1]+myMatrix[1][2]*aMatrix.myMatrix[2][1];
		temp.myMatrix[1][2]=myMatrix[1][0]*aMatrix.myMatrix[0][2]+myMatrix[1][1]*aMatrix.myMatrix[1][2]+myMatrix[1][2]*aMatrix.myMatrix[2][2];
		temp.myMatrix[1][3]=myMatrix[1][3];

		temp.myMatrix[2][0]=myMatrix[2][0]*aMatrix.myMatrix[0][0]+myMatrix[2][1]*aMatrix.myMatrix[1][0]+myMatrix[2][2]*aMatrix.myMatrix[2][0];
		temp.myMatrix[2][1]=myMatrix[2][0]*aMatrix.myMatrix[0][1]+myMatrix[2][1]*aMatrix.myMatrix[1][1]+myMatrix[2][2]*aMatrix.myMatrix[2][1];
		temp.myMatrix[2][2]=myMatrix[2][0]*aMatrix.myMatrix[0][2]+myMatrix[2][1]*aMatrix.myMatrix[1][2]+myMatrix[2][2]*aMatrix.myMatrix[2][2];
		temp.myMatrix[2][3]=myMatrix[2][3];

		temp.myMatrix[3][0]=myMatrix[3][0];
		temp.myMatrix[3][1]=myMatrix[3][1];
		temp.myMatrix[3][2]=myMatrix[3][2];
		temp.myMatrix[3][3]=myMatrix[3][3];


		memcpy(myMatrix,temp.myMatrix,4*4*4);

		return *this;
	}

	template <class TYPE>
	Vector3<TYPE> Matrix44<TYPE>::GetPositon()
	{
		Vector3<TYPE> returnPos;
		returnPos.x=myMatrix[3][0];
		returnPos.y=myMatrix[3][1];
		returnPos.z=myMatrix[3][2];

		return(returnPos);
	}


	template <class TYPE>
	Matrix44<TYPE>& Matrix44<TYPE>::operator=(const Matrix33<TYPE>& aMatrix)	// matrice multiplikation
	{
		SetIdentity();	
		myMatrix[0][0]=aMatrix.myMatrix[0][0];
		myMatrix[1][0]=aMatrix.myMatrix[1][0];
		myMatrix[2][0]=aMatrix.myMatrix[2][0];

		myMatrix[0][1]=aMatrix.myMatrix[0][1];
		myMatrix[1][1]=aMatrix.myMatrix[1][1];
		myMatrix[2][1]=aMatrix.myMatrix[2][1];

		myMatrix[0][2]=aMatrix.myMatrix[0][2];
		myMatrix[1][2]=aMatrix.myMatrix[1][2];
		myMatrix[2][2]=aMatrix.myMatrix[2][2];
		return *this;
	}


	template <class TYPE>
	DirectX::XMMATRIX Matrix44<TYPE>::GetD3DMatrix()
	{
		DirectX::XMMATRIX returnMatrix;
		for(int i = 0; i < 4; i++)
		{	
			for(int j = 0; j < 4; j++)
			{
				returnMatrix.m[i][j] = myMatrix[i][j];
			}
		}
		return returnMatrix;
	}


	template <class TYPE>
	Matrix44<TYPE> Matrix44<TYPE>::RotateAroundXPI(const float aPIAngle)
	{
		Matrix44 temp;
		float Sin,Cos;

		Sin=(float)sin(aPIAngle);
		Cos=(float)cos(aPIAngle);

		temp.myMatrix[1][1]=Cos;
		temp.myMatrix[2][1]=Sin;
		temp.myMatrix[2][2]=Cos;
		temp.myMatrix[1][2]=-Sin;
		temp.myMatrix[3][3]=1;


		return(temp);
	}

	template <class TYPE>
	Matrix44<TYPE> Matrix44<TYPE>::RotateAroundYPI(const float aPIAngle)
	{
		Matrix44 temp;
		float Sin,Cos;

		Sin=(float)sin(aPIAngle);
		Cos=(float)cos(aPIAngle);

		temp.myMatrix[0][0]=Cos;
		temp.myMatrix[2][0]=-Sin;
		temp.myMatrix[2][2]=Cos;
		temp.myMatrix[0][2]=Sin;
		temp.myMatrix[3][3]=1;

		return(temp);

	}

	template <class TYPE>
	Matrix44<TYPE> Matrix44<TYPE>::RotateAroundZPI(const float aPIAngle)
	{
		Matrix44 temp;
		float Sin,Cos;

		Sin=(float)sin(aPIAngle);
		Cos=(float)cos(aPIAngle);

		temp.myMatrix[0][0]=Cos;
		temp.myMatrix[1][0]=Sin;
		temp.myMatrix[1][1]=Cos;
		temp.myMatrix[0][1]=-Sin;
		temp.myMatrix[3][3]=1;



		return(temp);
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix44<TYPE>::RotateTowardsVector(Vector3f point)
	{
		Matrix33<TYPE> temp;
		CU::Vector3f direction(GetPositon().x - point.x, GetPositon().y - point.y, GetPositon().z - point.z);
		direction = direction.Normalize();

		CU::Vector3f up(0.0f, 1.0f, 0.0f);

		CU::Vector3f right(direction.Cross(up));
		right = right.Normalize();
		up = direction.Cross(right);


		temp.myMatrix[0][0] = right.x;
		temp.myMatrix[0][1] = right.y;
		temp.myMatrix[0][2] = right.z;
		temp.myMatrix[1][0] = up.x;
		temp.myMatrix[1][1] = up.y;
		temp.myMatrix[1][2] = up.z;
		temp.myMatrix[2][0] = direction.x;
		temp.myMatrix[2][1] = direction.y;
		temp.myMatrix[2][2] = direction.z;
		return temp;
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix44<TYPE>::RotateTowardsDir(CU::Vector3f aVector)
	{
		Matrix33<TYPE> temp;
		CU::Vector3f direction = aVector.Normalize();
		//In general what it does it that it limits direction of y to avoid gimbal
		//Proper implemenration of quatenerions is need to remove this
		if(direction.y == 1)
		{
			direction.y = 0.9f;
			direction.x = 0.1f;
		}	
		
		if(direction.y == -1)
		{
			direction.y = -0.9f;
			direction.x = -0.1f;
		}	
		CU::Vector3f up(0.0f, 1.0f, 0.0f);

		CU::Vector3f right(direction.Cross(up));
		right = right.Normalize();
		up = direction.Cross(right);

		temp.myMatrix[0][0] = right.x;
		temp.myMatrix[0][1] = right.y;
		temp.myMatrix[0][2] = right.z;
		temp.myMatrix[1][0] = up.x;
		temp.myMatrix[1][1] = up.y;
		temp.myMatrix[1][2] = up.z;
		temp.myMatrix[2][0] = direction.x;
		temp.myMatrix[2][1] = direction.y;
		temp.myMatrix[2][2] = direction.z;
		return temp;

	}

	
	template <class TYPE>
	Matrix33<TYPE> Matrix44<TYPE>::RotateTowardsDirYawPitch(CU::Vector3f aVector)
	{
		Matrix33<TYPE> temp;
		CU::Vector3f direction = aVector.Normalize();
		//In general what it does it that it limits direction of y to avoid gimbal
		//Proper implemenration of quatenerions is need to remove this
		if(direction.y == 1)
		{
			direction.y = 0.9f;
			direction.x = 0.1f;
		}	
		
		if(direction.y == -1)
		{
			direction.y = -0.9f;
			direction.x = -0.1f;
		}	
		CU::Vector3f up(0.0f, 1.0f, 0.0f);
		
		CU::Vector3f right(direction.Cross(up));
		right = right.Normalize();

		temp.myMatrix[0][0] = right.x;
		temp.myMatrix[0][1] = right.y;
		temp.myMatrix[0][2] = right.z;
		temp.myMatrix[1][0] = up.x;
		temp.myMatrix[1][1] = up.y;
		temp.myMatrix[1][2] = up.z;
		temp.myMatrix[2][0] = direction.x;
		temp.myMatrix[2][1] = direction.y;
		temp.myMatrix[2][2] = direction.z;
		return temp;

	}

	template <class TYPE>
	Matrix44<TYPE> Matrix44<TYPE>::Identity()
	{
		Matrix44 temp;
		return(temp);
	}


	template <class TYPE>
	Matrix44<TYPE>& Matrix44<TYPE>::operator*=(const Vector4<TYPE>& aVector)	// matrice multiplikation
	{
		myMatrix[0][0]*=aVector.x;
		myMatrix[1][0]*=aVector.x;
		myMatrix[2][0]*=aVector.x;
		myMatrix[3][0]*=aVector.x;
		myMatrix[0][1]*=aVector.y;
		myMatrix[1][1]*=aVector.y;
		myMatrix[2][1]*=aVector.y;
		myMatrix[3][1]*=aVector.y;
		myMatrix[0][2]*=aVector.z;
		myMatrix[1][2]*=aVector.z;
		myMatrix[2][2]*=aVector.z;
		myMatrix[3][2]*=aVector.z;
		myMatrix[0][3]*=aVector.w;
		myMatrix[1][3]*=aVector.w;
		myMatrix[2][3]*=aVector.w;
		myMatrix[3][3]*=aVector.w;
		return(*this);
	}

	template <class TYPE>
	void Matrix44<TYPE>::SetPosition(const CU::Vector3f& aPosition)
	{
		myMatrix[3][0] = aPosition.myX;
		myMatrix[3][1] = aPosition.myY;
		myMatrix[3][2] = aPosition.myZ;
		myMatrix[3][3] = 1;
	}

	template <class TYPE>
	void Matrix44<TYPE>::SetRotation(const Matrix33<TYPE>& aMatrix)
	{
		myMatrix[0][0] = aMatrix.myMatrix[0][0];
		myMatrix[0][1] = aMatrix.myMatrix[0][1];
		myMatrix[0][2] = aMatrix.myMatrix[0][2];
		myMatrix[1][0] = aMatrix.myMatrix[1][0];
		myMatrix[1][1] = aMatrix.myMatrix[1][1];
		myMatrix[1][2] = aMatrix.myMatrix[1][2];
		myMatrix[2][0] = aMatrix.myMatrix[2][0];
		myMatrix[2][1] = aMatrix.myMatrix[2][1];
		myMatrix[2][2] = aMatrix.myMatrix[2][2];
	}

	template <class TYPE>
	Matrix33<TYPE> Matrix44<TYPE>::Get33()
	{	
		Matrix33<TYPE> returnMatrix;
		returnMatrix.myMatrix[0][0] = myMatrix[0][0];
		returnMatrix.myMatrix[0][1] = myMatrix[0][1];
		returnMatrix.myMatrix[0][2] = myMatrix[0][2];
		returnMatrix.myMatrix[1][0] = myMatrix[1][0];
		returnMatrix.myMatrix[1][1] = myMatrix[1][1];
		returnMatrix.myMatrix[1][2] = myMatrix[1][2];
		returnMatrix.myMatrix[2][0] = myMatrix[2][0];
		returnMatrix.myMatrix[2][1] = myMatrix[2][1];
		returnMatrix.myMatrix[2][2] = myMatrix[2][2];
		return returnMatrix;
	}
	template <class TYPE>
	Matrix44<TYPE> Matrix44<TYPE>::Transpose()
	{	
		Matrix44<TYPE> returnMatrix;
		returnMatrix.myMatrix[0][0] = myMatrix [0][0];
		returnMatrix.myMatrix[1][0] = myMatrix [0][1];
		returnMatrix.myMatrix[2][0] = myMatrix [0][2];
		returnMatrix.myMatrix[3][0] = myMatrix [0][3];

		returnMatrix.myMatrix[0][1] = myMatrix [1][0];
		returnMatrix.myMatrix[1][1] = myMatrix [1][1];
		returnMatrix.myMatrix[2][1] = myMatrix [1][2];
		returnMatrix.myMatrix[3][1] = myMatrix [1][3];

		returnMatrix.myMatrix[0][2] = myMatrix [2][0];
		returnMatrix.myMatrix[1][2] = myMatrix [2][1];
		returnMatrix.myMatrix[2][2] = myMatrix [2][2];
		returnMatrix.myMatrix[3][2] = myMatrix [2][3];

		returnMatrix.myMatrix[0][3] = myMatrix [3][0];
		returnMatrix.myMatrix[1][3] = myMatrix [3][1];
		returnMatrix.myMatrix[2][3] = myMatrix [3][2];
		returnMatrix.myMatrix[3][3] = myMatrix [3][3];

		return returnMatrix;
	}

	template <class TYPE>
	Vector4<TYPE> Matrix44<TYPE>::GetPosition()
	{
		Vector4<TYPE> returnVector;
		returnVector.myX = myMatrix[3][0];
		returnVector.myY = myMatrix[3][1];
		returnVector.myZ = myMatrix[3][2];
		returnVector.myW = myMatrix[3][3];
		return returnVector;
	}

	template <class TYPE>
	Vector3<TYPE> Matrix44<TYPE>::GetPosition3()
	{
		Vector3<TYPE> returnVector;
		returnVector.myX = myMatrix[3][0];
		returnVector.myY = myMatrix[3][1];
		returnVector.myZ = myMatrix[3][2];
		return returnVector;
	}


	template <class TYPE>
	float Matrix44<TYPE>::Detetermant()
	{	
		float deter =	myMatrix[0][0] * ( myMatrix[1][1] * myMatrix[2][2] - myMatrix[1][2] * myMatrix[2][1])
			+		myMatrix[0][1] * ( myMatrix[1][2] * myMatrix[2][0] - myMatrix[1][0] * myMatrix[2][2])
			+		myMatrix[0][2] * ( myMatrix[2][0] * myMatrix[2][1] - myMatrix[1][1] * myMatrix[2][0]);
		return deter;
	}

	template <class TYPE>
	void Matrix44<TYPE>::RotateXYZ(const float& anX, const float& anY, const float& anZ)
	{
		SetRotation(CU::Matrix33f::RotateAroundXPI(anX));
		*this *= CU::Matrix33f::RotateAroundYPI(anY);
		*this *= CU::Matrix33f::RotateAroundZPI(anZ);
	}

		/*template <class TYPE>
		Matrix44<TYPE> Matrix44<TYPE>::RealInverse()
		{
		CU::Matrix44f matrix;
		float deter = Detetermant();
		float oneOverDet = 1 / deter;

		matrix.myMatrix[0][0] = (myMatrix[1][1] * myMatrix[2][2] - myMatrix[1][2] * myMatrix[2][1]) * oneOverDet;
		matrix.myMatrix[0][1] = (myMatrix[0][2] * myMatrix[2][1] - myMatrix[0][1] * myMatrix[2][2]) * oneOverDet;
		matrix.myMatrix[0][2] = (myMatrix[0][1] * myMatrix[1][2] - myMatrix[0][2] * myMatrix[1][1]) * oneOverDet;

		matrix.myMatrix[1][0] = (myMatrix[1][2] * myMatrix[2][0] - myMatrix[1][0] * myMatrix[2][2]) * oneOverDet;
		matrix.myMatrix[1][1] = (myMatrix[0][0] * myMatrix[2][2] - myMatrix[0][2] * myMatrix[2][0]) * oneOverDet;
		matrix.myMatrix[1][2] = (myMatrix[0][2] * myMatrix[1][0] - myMatrix[0][0] * myMatrix[1][2]) * oneOverDet;

		matrix.myMatrix[2][0] = (myMatrix[1][0] * myMatrix[2][1] - myMatrix[1][1] * myMatrix[2][0]) * oneOverDet;
		matrix.myMatrix[2][1] = (myMatrix[0][1] * myMatrix[2][0] - myMatrix[0][0] * myMatrix[2][1]) * oneOverDet;
		matrix.myMatrix[2][2] = (myMatrix[0][0] * myMatrix[1][1] - myMatrix[0][1] * myMatrix[1][0]) * oneOverDet;


		CU::Vector3f t = CU::Vector3f(myMatrix[3][0], myMatrix[3][1], myMatrix[3][2]);
		matrix.myMatrix[3][0] = -(t.x * matrix.myMatrix[0][0] + t.y * matrix.myMatrix[1][0] + t.z * matrix.myMatrix[2][0]);
		matrix.myMatrix[3][1] = -(t.x * matrix.myMatrix[0][1] + t.y * matrix.myMatrix[1][1] + t.z * matrix.myMatrix[2][1]);
		matrix.myMatrix[3][2] = -(t.x * matrix.myMatrix[0][2] + t.y * matrix.myMatrix[1][2] + t.z * matrix.myMatrix[2][2]);

		matrix.myMatrix[3][3] = 1;
		return matrix;

		}*/

		template <class TYPE>
		Matrix44<TYPE>  Matrix44<TYPE>::Real4X4Inverse()
		{
			DirectX::XMFLOAT4X4 m;
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					m.m[i][j] = myMatrix[i][j];
				}
			}

			DirectX::XMMATRIX xmMatrix( &m.m[0][0] );
			xmMatrix = DirectX::XMMatrixInverse(nullptr, xmMatrix);
			DirectX::XMFLOAT4X4 matrixout;
			DirectX::XMStoreFloat4x4( &matrixout, xmMatrix );
			Matrix44<TYPE> returnMatrix;
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 4; j++)
				{
					returnMatrix.myMatrix[i][j] = matrixout.m[i][j];
				}
			}
			return returnMatrix;

		}


		typedef Matrix33<float> Matrix33f;
		typedef Matrix33<double> Matrix33d;
		typedef Matrix33<int> Matrix33i;

		typedef Matrix44<float> Matrix44f;
		typedef Matrix44<double> Matrix44d;
		typedef Matrix44<int> Matrix44i;
	};

	namespace CU = CommonUtilities;

#endif