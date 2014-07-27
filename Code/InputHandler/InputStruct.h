#ifndef DXINPUTSTRUCT_HEADER
#define DXINPUTSTRUCT_HEADER

#include <vector>
#include "Message.h"

struct MessageQueue
{
	MessageQueue()
	{
		Queue = new std::vector< Message >;
		Queue->reserve( 10000 );
		IfBusy = false;
	}
	~MessageQueue()
	{	
		delete Queue; Queue = NULL;
	}
	
	std::vector< Message >* Queue;
	bool IfBusy;
};


#endif