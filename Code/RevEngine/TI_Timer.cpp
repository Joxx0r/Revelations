#include <stdafx.h>
#include "TI_Timer.h"

using namespace TI_Time;

Timer::Timer(Clocks aClock)
:myCurrentTime(aClock,0),
myElapsedTime(aClock,0)
{
	myIsPausedFlag=false;
}
	
Timer::~Timer()
{
}

void Timer::AttachTimer(Timer* aTimer)
{
	myAttachedTimers.push_back(aTimer);
}

bool Timer::Update(Time& aElapsedTime)
{
	if(false==myIsPausedFlag)
	{
		myCurrentTime+=aElapsedTime;
		myElapsedTime=aElapsedTime;
	}
	return(true);
}

void Timer::SetPauseState(bool aPausedFlag) 
{
	myIsPausedFlag=aPausedFlag;
}

const Time& Timer::GetTotalTime() const
{	
	return(myCurrentTime);
}

const Time& Timer::GetElapsedTime() const
{
	return(myElapsedTime);
}
