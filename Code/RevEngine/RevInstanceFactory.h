#ifndef _REV_INSTANCE_FACTORY_H
#define _REV_INSTANCE_FACTORY_H

#include <vector>

class RevBaseInstance;

class RevInstanceFactory
{
public:
	RevInstanceFactory(void);
	~RevInstanceFactory(void);

	RevBaseInstance* GetNewInstance(const int& newInstance);

	void DestroyAll();

private:

	std::vector< RevBaseInstance* > m_instances;
};

#endif