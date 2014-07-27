#include "stdafx.h"
#include "PostList.h"
#include "Message.h"
#include <assert.h>
#include <algorithm>
#include "Subscriber.h"

PostList::PostList()
{
	mySubscriberPairs.reserve(1000);
}
	

void PostList::DistributeMessage(const Message& aMessage)
{
	for(int index = 0; index < mySubscriberPairs.size(); ++index)
	{
		if(mySubscriberPairs[index].mySubscriber->ReceiveMessage(aMessage)==false)
		{
			return;	
		}
	}
}

void PostList::Subscribe(Subscriber* aSubscriber,SubsciberPrioEnum aPrio)
{
	assert(aSubscriber && "Subscriber is NULL");

	mySubscriberPairs.push_back(SubscriberPair(aSubscriber,aPrio));
	std::sort(mySubscriberPairs.begin(),mySubscriberPairs.end(),SubscriberPairCompareLess());
}

void PostList::UnSubscribe(Subscriber* aSubscriber)
{
	for(int index = mySubscriberPairs.size()-1; index >= 0; --index)
	{
   		if(mySubscriberPairs[index].mySubscriber == aSubscriber)
		{
			SubscriberPair sub = mySubscriberPairs[mySubscriberPairs.size()-1];
			mySubscriberPairs[ index ] = sub;
			mySubscriberPairs.pop_back();
		}
	}
	if(mySubscriberPairs.empty()==false)
	{
		std::sort(mySubscriberPairs.begin(),mySubscriberPairs.end());
	}
}