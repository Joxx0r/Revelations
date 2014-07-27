#ifndef _POST_LIST_HEADER
#define _POST_LIST_HEADER
#include <vector>

class Subscriber;
class Message;

enum SubsciberPrioEnum
{
	SUBSCRIBER_EDITOR,
	SUBSCRIBER_DIRECTOR,
	SUBSCRIBER_NORMAL,
	SUBSCRIBER_LOW,
};

struct SubscriberPair
{
	SubscriberPair(Subscriber* aSubscriber=0, SubsciberPrioEnum aPrio = SUBSCRIBER_NORMAL)
	{
		mySubscriber=aSubscriber;
		myPrio=aPrio;
	}

	bool operator <(const SubscriberPair& b) const
	{
		if(myPrio < b.myPrio)
		{
			return true;
		}
		return false;
	}

	Subscriber* mySubscriber;
	SubsciberPrioEnum myPrio;
};
struct SubscriberPairCompareLess
{
	bool operator()(const SubscriberPair& a, const SubscriberPair& b) const
	{
		return(a < b);
	}
};
class PostList
{
public:
	PostList(void);
	virtual ~PostList() {}

	void Subscribe(Subscriber* aSubscriber,SubsciberPrioEnum aPrio);
	void UnSubscribe(Subscriber* aSubscriber);

	void DistributeMessage(const Message& aMessage);

private:	
	std::vector<SubscriberPair>	mySubscriberPairs;
};

#endif