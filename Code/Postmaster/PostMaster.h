#ifndef _POST_MASTER_H_
#define _POST_MASTER_H_

#include <vector>
#include "PostList.h"
#include "MessageListTypeEnum.h"

class Message;
class Subscriber;

class PostMaster
{
public:
	PostMaster();
	~PostMaster();

	void AddMessage(const Message& aMessage);
	void Subscribe(const MessageListType aMessageType, Subscriber* aSubscriber,SubsciberPrioEnum aPrio = SUBSCRIBER_NORMAL);
	void UnSubscribe(const MessageListType aMessageType, Subscriber* aSubscriber);
	void DistributeMessages();
	void DeleteMessagePile();

	
private:
	std::vector<Message> myMessagePile;
	PostList myPostLists[ NUM_OF_MESSAGE_TYPES ];

};

#endif