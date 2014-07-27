#ifndef _OPAQUE_CONTAINER_HEADER
#define _OPAQUE_CONTAINER_HEADER

#include <map>
#include <assert.h>
#include <string>

#pragma warning(disable:4996)


class OpaqueContainer
{
public:
	OpaqueContainer() { };
	virtual ~OpaqueContainer() { };

	inline void AddChar(const std::string& aKey, char& aCharToBeStored);
	inline bool TryGetChar(const std::string& aKey, char& aRefValue);

	inline void EraseValueFromContainer(const std::string& aKey);
	inline void RemoveValueFromContainer(const std::string& aKey);
	inline void DeleteAll();

	inline bool IsEmpty() { return myDictonary.empty(); }

	template <class ContainerType>
	void AddValue(const std::string& aKey, ContainerType aValueToBeStored);
	template <>
	void AddValue<std::string>(const std::string& aKey, std::string aValueToBeStored);

	template <class ContainerType>
	const bool TryGetValue(const std::string& aKey, ContainerType& aRefValue);
	template <>
	const bool TryGetValue<std::string>(const std::string& aKey, std::string& aRefValue);

	template <class ContainerType>
	const bool TryChangeValue(const std::string& aKey, ContainerType& aRefValue);
	template <>
	const bool TryChangeValue<std::string>(const std::string& aKey, std::string& aRefValue);

	template <class ContainerType>
	const bool TryRemoveValue(const std::string& aKey, ContainerType& aRefValue);


private:
	typedef std::map<std::string, char*> Dictonary;
	Dictonary myDictonary;
};

void OpaqueContainer::EraseValueFromContainer(const std::string& aKey)
{
	Dictonary::iterator iter;
	iter = myDictonary.find(aKey);
	if(iter==myDictonary.end())
	{
		assert(0 && "Key not in dictonary");
	}

	char* tmpHolder = iter->second;
	delete tmpHolder;
	tmpHolder		= NULL;
	myDictonary.erase(iter);
}

void OpaqueContainer::RemoveValueFromContainer(const std::string& aKey)
{
	Dictonary::iterator iter;
	iter = myDictonary.find(aKey);
	if(iter==myDictonary.end())
	{
		assert(0 && "Key not in dictonary");
	}

	myDictonary.erase(iter);
}

template <class ContainerType>
void OpaqueContainer::AddValue(const std::string& aKey, ContainerType aValueToBeStored)
{

	int sizeOfType = sizeof(ContainerType);
	char* pointerToBeAdded = new char[sizeOfType];
	memcpy(pointerToBeAdded,&aValueToBeStored,sizeOfType);
	myDictonary[aKey] = pointerToBeAdded;
}

template <>
void OpaqueContainer::AddValue<std::string>(const std::string& aKey, std::string aValueToBeStored)
{
	int sizeOfType = aValueToBeStored.size();
	char* pointerToBeAdded = new char[sizeOfType+1];
	strcpy(pointerToBeAdded,aValueToBeStored.c_str());
	myDictonary[aKey] = pointerToBeAdded;
}

template <class ContainerType>
const bool OpaqueContainer::TryGetValue(const std::string& aKey, ContainerType& aRefValue)
{
	Dictonary::iterator iter = 	myDictonary.find(aKey);
	if(iter==myDictonary.end())return false;
	aRefValue = (*reinterpret_cast<ContainerType*>(myDictonary[aKey]));
	return true;
}
template <>
const bool OpaqueContainer::TryGetValue<std::string>(const std::string& aKey, std::string& aRefValue)
{
	Dictonary::iterator iter;
	iter = myDictonary.find(aKey);
	if(iter==myDictonary.end()) return false;
	aRefValue = myDictonary[aKey];
	return true;
}

void OpaqueContainer::DeleteAll()
{
	if(myDictonary.empty()) return;

	for(Dictonary::iterator iter = myDictonary.begin(); iter != myDictonary.end(); ++iter)
	{
		char* tmpHolder = iter->second;
		delete tmpHolder;
		tmpHolder		= NULL;
	}
	myDictonary.clear();
}

template <class ContainerType>
const bool OpaqueContainer::TryChangeValue(const std::string& aKey, ContainerType& aRefValue)
{
	if(myDictonary.empty()) return false;

	Dictonary::iterator iter = myDictonary.find(aKey);
	if(iter==myDictonary.end()) return false;

	EraseValueFromContainer(aKey);
	AddValue(aKey,aRefValue);
	return true;
}
template <>
const bool OpaqueContainer::TryChangeValue<std::string>(const std::string& aKey, std::string& aRefValue)
{
	if(myDictonary.empty()) return false;
	Dictonary::iterator iter = myDictonary.find(aKey);
	if(iter==myDictonary.end()) return false;

	EraseValueFromContainer(aKey);
	AddValue<std::string>(aKey,aRefValue);
	return true;
}

void OpaqueContainer::AddChar(const std::string& aKey, char& aCharToBeStored)
{	
	char* pointerToBeAdded = &aCharToBeStored;
	myDictonary[aKey] = pointerToBeAdded;
}

bool OpaqueContainer::TryGetChar(const std::string& aKey, char& aRefValue)
{

	if(myDictonary.empty()) return false;
	Dictonary::iterator iter = myDictonary.find(aKey);
	if(iter==myDictonary.end()) return false;
	char* tmp = myDictonary[aKey];
	aRefValue = (*tmp);
	return true;
}


template <class ContainerType>
const bool OpaqueContainer::TryRemoveValue(const std::string& aKey, ContainerType& aRefValue)
{
	if(myDictonary.empty()) return false;
	Dictonary::iterator iter = myDictonary.find(aKey);
	if(iter==myDictonary.end()) return false;
	aRefValue = (*reinterpret_cast<ContainerType*>(myDictonary[aKey]));;
	RemoveValueFromContainer(aKey);
	return true;
}

#endif