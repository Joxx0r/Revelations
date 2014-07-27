#include <stdafx.h>
#include "TI_TimeManager.h"
#include "TI_Timer.h"
#include "TI_SystemTime.h"

#include <assert.h>

using namespace TI_Time;

TimeManager::TimeManager()
{
	myClocks.reserve(NUM_CLOCKS);
	for(int i = 0; i < NUM_CLOCKS; i++) {  myClocks.push_back(NULL); }
	mySystemClock=NULL; // Refactor away once we gain inheritance
}

TimeManager::~TimeManager()
{
	for(unsigned int i = 0; i < myClocks.size(); i++)
	{
		if(myClocks[i]!=NULL)
		{
			delete myClocks[i];
			myClocks[i] = NULL;
		}
	}
	delete mySystemClock;
	mySystemClock = NULL;
}

void TimeManager::InitDefaults()
{
	CreateTimer(SYSTEM_CLOCK);
	CreateTimer(MASTER_CLOCK);
	AttachTimerToTimer(SYSTEM_CLOCK,MASTER_CLOCK);
	SetCurrentDefaultClock(MASTER_CLOCK);
}

bool TimeManager::Update()
{
	return(mySystemClock->Update());
}

void TimeManager::CreateTimer(const Clocks aTimer)
{
	if(aTimer==SYSTEM_CLOCK) // Refactor away once we gain inheritance
	{
		if(mySystemClock!=NULL)
		{
			delete mySystemClock;
			mySystemClock = NULL;
		}
		mySystemClock=new SystemTime();
		mySystemClock->Init();

	}
	else
	{
		if(myClocks[aTimer]!=NULL)
		{
			delete myClocks[aTimer];
			myClocks[ aTimer ] = NULL;
		}
		myClocks[aTimer]=new Timer(aTimer);
	}
}

bool TimeManager::AttachTimerToTimer(const Clocks aTimerToAttachTo,const Clocks aTimerToAttach)
{
	assert(myClocks[aTimerToAttach]!=NULL && aTimerToAttach!=SYSTEM_CLOCK);
	assert(myClocks[aTimerToAttachTo]!=NULL || (aTimerToAttachTo==SYSTEM_CLOCK));// Refactor away once we gain inheritance

	if(aTimerToAttachTo==SYSTEM_CLOCK)
	{
		mySystemClock->AttachTimer(myClocks[aTimerToAttach]);
	}
	else
	{
		myClocks[aTimerToAttachTo]->AttachTimer(myClocks[aTimerToAttach]);
	}

	return(true);
}

const Time& TimeManager::GetElapsedTime() const
{
	return(myClocks[MASTER_CLOCK]->GetElapsedTime());
}

const Time& TimeManager::GetTotalTime() const
{
	return(myClocks[myDefaultClock]->GetTotalTime());
}

const Time& TimeManager::GetElapsedTime(const Clocks aTimer)  const
{
	assert((myClocks[aTimer]!=NULL) || (aTimer==SYSTEM_CLOCK));
	if(aTimer==SYSTEM_CLOCK) 
	{
		return(mySystemClock->GetElapsedTime());
	}
	else
	{
		return(myClocks[aTimer]->GetElapsedTime());
	}
}

const Time& TimeManager::GetTotalTime(const Clocks aTimer)  const
{
	assert((myClocks[aTimer]!=NULL) || (aTimer==SYSTEM_CLOCK));
	if(aTimer==SYSTEM_CLOCK) 
	{
		return(mySystemClock->GetTotalTime());
	}
	else
	{
		return(myClocks[aTimer]->GetTotalTime());
	}
}

void TimeManager::SetCurrentDefaultClock(const Clocks aTimer)
{
	myDefaultClock=aTimer;
}
		

bool TimeManager::SetClockPauseState(const Clocks aTimer,const bool aPauseState)
{
	assert(myClocks[aTimer]!=NULL);

	myClocks[aTimer]->SetPauseState(aPauseState);
	return(true);
}
