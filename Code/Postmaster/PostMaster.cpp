#include "stdafx.h"
#include "PostMaster.h"
#include "Message.h"

PostMaster::PostMaster()
{
	myMessagePile.reserve( 10000 );
}
PostMaster::~PostMaster()
{
}

void PostMaster::AddMessage(const Message& aMessage)
{
	myMessagePile.push_back(aMessage);
}

void PostMaster::Subscribe(const MessageListType aMessageType, Subscriber* aSubscriber,SubsciberPrioEnum aPrio)
{
	myPostLists[aMessageType].Subscribe(aSubscriber,aPrio);
}

void PostMaster::UnSubscribe(const MessageListType aMessageType, Subscriber* aSubscriber)
{
	myPostLists[aMessageType].UnSubscribe(aSubscriber);
}

void PostMaster::DistributeMessages()
{
	for(int index = 0; index < myMessagePile.size(); ++index)
	{
		myPostLists[ myMessagePile[index].GetMessageListType() ].DistributeMessage(myMessagePile[index]);
	}
	for(int loopIndex = 0; loopIndex < myMessagePile.size(); loopIndex++)
	{		
		myMessagePile[loopIndex].DeleteAllRemainingData();
	}

	myMessagePile.clear();
}

void PostMaster::DeleteMessagePile()
{
	for(int loopIndex = 0; loopIndex < myMessagePile.size(); loopIndex++)
	{		
		myMessagePile[loopIndex].DeleteAllRemainingData();
	}
	myMessagePile.clear();
}