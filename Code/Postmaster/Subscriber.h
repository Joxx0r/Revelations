#ifndef _SUBSCRIBER_HEADER
#define _SUBSCRIBER_HEADER

class Message;
class Subscriber
{
public:
	Subscriber(void) { };
	virtual ~Subscriber(void) { };
	virtual bool ReceiveMessage(Message aMessage)=0;
};


#endif