#ifndef _MESSAGE_TEMP_HEADER
#define _MESSAGE_TEMP_HEADER

#include <string>
#include "OpaqueContainer.h"
#include "MessageListTypeEnum.h"

class Message
{
public:

	Message() { };
	Message(const int& aMessageListType)
		:myMessageListType(aMessageListType) { };
	virtual ~Message() { };

	inline OpaqueContainer& GetContainer()	{ return myContainer; }
	inline const int GetMessageListType()	const { return myMessageListType; }

	inline void SetMessageListType(const MessageListType& aMessageListType) { myMessageListType = aMessageListType; }

	inline void DeleteAllRemainingData() { myContainer.DeleteAll(); }

	inline bool TryGetString(const std::string aKey, std::string& someString, std::string aDefaultValue="");
	//inline bool TryAddString(const std::string aKey, std::string& someString, std::string aDefaultValue="");

	template <class Type>
	void AddValue(const std::string& aKey, Type aValue);

	template <class Type>
	const bool TryGetValue(const std::string& aKey, Type& aValue, Type* aDefaultValue = NULL);

	template <class Type>
	const bool TryChangeValue(const std::string& aKey, Type aValue);

private:
	int					myMessageListType;
	OpaqueContainer		myContainer;
};

bool Message::TryGetString(const std::string aKey, std::string& someString, std::string aDefaultValue)
{
	if(!myContainer.TryGetValue<std::string>(aKey,someString))
	{
		if(aDefaultValue!="")
		{
			someString = aDefaultValue;
		}
		return false;
	}
	return true;
}

template <class Type>
void Message::AddValue(const std::string& aKey, Type aValue)
{
	myContainer.AddValue(aKey, aValue);
}

template <class Type>
const bool Message::TryGetValue(const std::string& aKey, Type& aValue, Type* aDefaultValue)
{
	if(!myContainer.TryGetValue(aKey,aValue))
	{
		if(aDefaultValue!=NULL)
		{
			aValue = (*aDefaultValue);
		}
		return false;
	}
	return true;
}

template <class Type>
const bool Message::TryChangeValue(const std::string& aKey, Type aValue)
{
	return myContainer.TryChangeValue(aKey,aValue);
}
#endif