#include <stdafx.h>
#include "RevFile.h"
#include <stdio.h>
#include <io.h>
#include <sstream>
#include <windows.h>

using namespace FI_File;

RevFile::RevFile()
{
	myFile=NULL;
}

RevFile::~RevFile()
{
	myFile=NULL;
}


int RevFile::GetFileSize()
{
	long size = _filelength(_fileno(myFile));
	return(size);
}
	

bool RevFile::Open(std::string aFileName, std::bitset<8> aSetOfFlags)
{
	std::string flags;
	
	if(myFile!=NULL)
	{
		if(Close()==false)
		{
			return(false);
		}
	}
	myIsBinaryFlag=aSetOfFlags.test(BINARY_REV);


	if(aSetOfFlags.test(READ_REV))
	{
		if(aSetOfFlags.test(WRITE_REV))
		{
			flags+="r+";
		}
		else
		{
			flags+="r";
		}
	}
	else
	{
		if(aSetOfFlags.test(WRITE_REV))
		{
			if(aSetOfFlags.test(APPEND_REV))
			{
				flags+="a+";
			}
			else
			{
				flags+="w";
			}
		}
		else
		{
			flags+="a";
		}
	}

	if(aSetOfFlags.test(BINARY_REV))
	{
		flags+="b";
	}
	if(fopen_s(&myFile,aFileName.c_str(),flags.c_str())!=0)
	{
		return(false);
	}
	if(myFile==NULL)
	{
		return(false);
	}
	return(true);
}

bool RevFile::Close()
{
	if(fclose(myFile)!=0)
	{
		return(false);
	}
	myFile=NULL;
	return(true);
}



bool RevFile::GoToNewLine()
{
	unsigned char input;
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			return(false);
		}
	}while (input!=13);
	return(true);
}

bool RevFile::TextReadInt(int& aInteger)
{
	std::string inputBuffer;
	unsigned char input;
	bool found=true;
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			aInteger=atoi(inputBuffer.c_str());
			return(false);
		}
		found=false;
		if(isdigit(input) || (input=='-')) 
		{
			inputBuffer+=input;
			found=true;
		}
		
	} while ((inputBuffer.size()==0) || (found==true));
	aInteger=atoi(inputBuffer.c_str());
	return(true);

}

bool RevFile::TextReadBool(bool& aBool)
{
	std::string inputBuffer;
	unsigned char input;
	bool found=true;
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			aBool=false;
			return(false);
		}
		found=false;
		if(isdigit(input) || (input=='-')) 
		{
			inputBuffer+=input;
			found=true;
		}
		
	} while ((inputBuffer.size()==0) || (found==true));
	int integer=atoi(inputBuffer.c_str());
	if(integer==0)
	{
		aBool=false;
	}
	else
	{
		aBool=true;
	}
	return(true);

}


bool RevFile::TextReadFloat(float& aFloat)
{
	std::string inputBuffer;
	unsigned char input;
	bool found=true;
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			return(false);
		}
		found=false;
		if(isdigit(input) 
			|| (input=='-') 
			|| (input=='.') 
			|| (input==','))
		{
			inputBuffer+=input;
			found=true;
		}
		
	} while ((inputBuffer.size()==0) || (found==true));
	aFloat=static_cast<float>(atof(inputBuffer.c_str()));
	return(true);

}

bool RevFile::TextReadDouble(double& aDouble)
{
	std::string inputBuffer;
	unsigned char input;
	bool found=true;
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			return(false);
		}
		found=false;
		if(isdigit(input) || (input=='-') || (input=='.'))
		{
			inputBuffer+=input;
			found=true;
		}
		
	} while ((inputBuffer.size()==0) || (found==true));
	aDouble=atof(inputBuffer.c_str());
	return(true);
}

bool RevFile::TextReadWord(std::string& aString)
{
	aString.clear();
	unsigned char input;
	bool found=true;
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			return(false);
		}
		found=false;
		if(isalnum(input) || ispunct(input) || (input=='\\') || (input=='/'))
		{
			aString+=input;
			found=true;
		}
		
	} while ((aString.size()==0) || (found==true));
	return(true);

}

bool RevFile::TextReadString(std::string& aString)
{
	aString.clear();
	unsigned char input;
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			return(false);
		}
	} while (input!='"');
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			return(false);
		}
		if(input!='"')
		{
			aString+=input;
		}
	} while (input!='"');
	return(true);
}


bool RevFile::TextWriteText(std::string aString)
{
	if(fwrite(aString.c_str(),1,aString.size(),myFile)!=aString.size())
	{
		return(false);
	}
	return(true);
}

bool RevFile::TextWriteInt(int& aInteger)
{
	std::string text;
	std::ostringstream stream(text);
	stream << aInteger;
	return(TextWriteText(stream.str()));
}

bool RevFile::TextWriteFloat(float& aFloat)
{
	std::string text;
	std::ostringstream stream(text);
	stream << aFloat;
	return(TextWriteText(stream.str()));

}

bool RevFile::TextWriteDouble(double& aDouble)
{
	std::string text;
	std::ostringstream stream(text);
	stream << aDouble;
	return(TextWriteText(stream.str()));
}



bool RevFile::BinaryReadInt(int& aInteger)
{
	if(fread(&aInteger,sizeof(aInteger),1,myFile)!=1)
	{
		return(false);
	}
	return(true);
}

bool RevFile::BinaryReadBool(bool& aBool)
{
	if(fread(&aBool,sizeof(aBool),1,myFile)!=1)
	{
		return(false);
	}
	return(true);
}


bool RevFile::BinaryReadFloat(float& aFloat)
{
	if(fread(&aFloat,sizeof(aFloat),1,myFile)!=1)
	{
		return(false);
	}
	return(true);

}


bool RevFile::BinaryReadDouble(double& aDouble)
{
	if(fread(&aDouble,sizeof(aDouble),1,myFile)!=1)
	{
		return(false);
	}
	return(true);
}

bool RevFile::BinaryReadString(std::string& aString)
{
	aString.clear();
	char input;
	do {
		if(fread(&input,1,1,myFile)!=1)
		{
			return(false);
		}
		aString+=input;
	} while (input!=NULL);
	return(true);
}

bool RevFile::ReadData(void* someData,int aSizeToRead)
{
	int bytesRead=fread(someData,1,aSizeToRead,myFile);
	if(bytesRead!=aSizeToRead)
	{
		return(false);
	}
	return(true);
}


bool RevFile::BinaryWriteInt(int& aInteger)
{
	if(fwrite(&aInteger,sizeof(aInteger),1,myFile)!=1)
	{
		return(false);
	}
	return(true);
}

bool RevFile::BinaryWriteFloat(float& aFloat)
{
	if(fwrite(&aFloat,sizeof(aFloat),1,myFile)!=1)
	{
		return(false);
	}
	return(true);
}

bool RevFile::BinaryWriteDouble(double& aDouble)
{
	if(fwrite(&aDouble,sizeof(aDouble),1,myFile)!=1)
	{
		return(false);
	}
	return(true);
}


bool RevFile::FlushToDisk()
{
	if(fflush(myFile)!=0)
	{
		return(false);
	}
	return(true);
}

bool RevFile::WriteData(void* someData,int aSizeTowrite)
{
	if(fwrite(someData,aSizeTowrite,1,myFile)!=1)
	{
		return(false);
	}
	return(true);
}

bool RevFile::ReadBool(bool& aBool)
{
	if(myIsBinaryFlag==true)
	{
		return(BinaryReadBool(aBool));
	}
	else
	{
		return(TextReadBool(aBool));
	}
}



bool RevFile::ReadInt(int& aInteger)
{
	if(myIsBinaryFlag==true)
	{
		return(BinaryReadInt(aInteger));
	}
	else
	{
		return(TextReadInt(aInteger));
	}
}

bool RevFile::ReadFloat(float& aFloat)
{
	if(myIsBinaryFlag==true)
	{
		return(BinaryReadFloat(aFloat));
	}
	else
	{
		return(TextReadFloat(aFloat));
	}
}

bool RevFile::ReadDouble(double& aDouble)
{
	if(myIsBinaryFlag==true)
	{
		return(BinaryReadDouble(aDouble));
	}
	else
	{
		return(TextReadDouble(aDouble));
	}
}

bool RevFile::ReadWord(std::string& aString)
{
	if(myIsBinaryFlag==true)
	{
		return(false);
	}
	else
	{
		return(TextReadWord(aString));
	}
}

bool RevFile::ReadString(std::string& aString)
{
	if(myIsBinaryFlag==true)
	{
		return(BinaryReadString(aString));
	}
	else
	{
		return(TextReadString(aString));
	}
}


bool RevFile::WriteString(std::string aString)
{
	if(TextWriteText(aString)==false)
	{
		return(false);
	}
	if(myIsBinaryFlag==true)
	{
		if(fwrite(0,1,1,myFile)!=1)
		{
			return(false);
		}
	}
	return(true);
}

bool RevFile::WriteInt(int& aInteger)
{
	if(myIsBinaryFlag==true)
	{
		return(BinaryWriteInt(aInteger));
	}
	else
	{
		return(TextWriteInt(aInteger));
	}
}

bool RevFile::WriteFloat(float& aFloat)
{
	if(myIsBinaryFlag==true)
	{
		return(BinaryWriteFloat(aFloat));
	}
	else
	{
		return(TextWriteFloat(aFloat));
	}
}

bool RevFile::WriteDouble(double& aDouble)
{
	if(myIsBinaryFlag==true)
	{
		return(BinaryWriteDouble(aDouble));
	}
	else
	{
		return(TextWriteDouble(aDouble));
	}
}


bool RevFile::ReadVector3f(CommonUtilities::Vector3f& aVector)
{
	if(ReadFloat(aVector.x)==false) return(false);
	if(ReadFloat(aVector.y)==false) return(false);
	if(ReadFloat(aVector.z)==false) return(false);
	return(true);
}