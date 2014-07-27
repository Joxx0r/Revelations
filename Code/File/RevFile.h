#ifndef FI_RevFile_HEADER
#define FI_RevFile_HEADER

#include <string>
#include <stdio.h>
#include <bitset>
#include "CU_Vector.h"
#include "CU_Matrix.h"

namespace FI_File
{
	enum RevFileFlags
	{
		READ_REV,
		WRITE_REV,
		APPEND_REV,
		BINARY_REV
	};

	class RevFile
	{
	public:
		RevFile();
		~RevFile();

		bool Open(std::string aRevFileName, std::bitset<8> aSetOfFlags=0xF);
		bool Close();


		bool GoToNewLine();
		
		bool ReadInt(int& aInteger);
		bool ReadBool(bool& aBool);
		bool ReadFloat(float& aFloat);
		bool ReadDouble(double& aDouble);
		bool ReadWord(std::string& aString);
		bool ReadString(std::string& aString);
		bool ReadVector3f(CommonUtilities::Vector3f& aVector);

		int GetFileSize();
		bool WriteString(std::string aString);
		bool WriteInt(int& aInteger);
		bool WriteFloat(float& aFloat);
		bool WriteDouble(double& aDouble);
		bool WriteMatrix(CU::Matrix44f& aMatrix);

		template <class Type>
		bool Write(Type& someData);
		template <class Type>
		bool Read(Type& someData);
		template <class Type>
		bool WriteArray(Type* someData,int aLength);
		template <class Type>
		bool ReadArray(Type* someData,int aLength);


		bool WriteData(void* someData,int aSizeToRead);
		bool ReadData(void* someData,int aSizeToRead);
		bool FlushToDisk();
		int GetRevFileSize();
	private:

			bool TextReadBool(bool& aBool);
			bool TextReadInt(int& aInteger);
			bool TextReadFloat(float& aFloat);
			bool TextReadDouble(double& aDouble);
			bool TextReadWord(std::string& aString);
			bool TextReadString(std::string& aString);


			bool TextWriteText(std::string aString);
			bool TextWriteInt(int& aInteger);
			bool TextWriteFloat(float& aFloat);
			bool TextWriteDouble(double& aDouble);

			bool BinaryReadBool(bool& aBool);
			bool BinaryReadInt(int& aInteger);
			bool BinaryReadFloat(float& aFloat);
			bool BinaryReadDouble(double& aDouble);
			bool BinaryReadString(std::string& aString);
				

			bool BinaryWriteInt(int& aInteger);
			bool BinaryWriteFloat(float& aFloat);
			bool BinaryWriteDouble(double& aDouble);

	
			


		FILE* myFile;
		bool myIsBinaryFlag;
		
	};



	template <class Type>
	bool RevFile::Write(Type& someData)
	{
		if(fwrite(&someData,sizeof(someData),1,myFile)!=1)
		{
			return(false);
		}
		return(true);
	}

	template <class Type>
	bool RevFile::Read(Type& someData)
	{
		if(fread(&someData,sizeof(someData),1,myFile)!=1)
		{
			return(false);
		}
		return(true);

	}
	
	template <class Type>
	bool RevFile::WriteArray(Type* someData,int aLength)
	{
		if(fwrite(&someData,sizeof(someData),aLength,myFile)!=aLength)
		{
			return(false);
		}
		return(true);

	}

	template <class Type>
	bool RevFile::ReadArray(Type* someData,int aLength)
	{
		if(fread(&someData,sizeof(someData),aLength,myFile)!=aLength)
		{
			return(false);
		}
		return(true);
	}



};



#endif