#include "stdafx.h"
#include "TI_SystemTime.h"

using namespace TI_Time;


SystemTime::SystemTime()
:myCurrentTime(SYSTEM_CLOCK,0),
myElapsedTime(SYSTEM_CLOCK,0)
{
}

SystemTime::~SystemTime()
{
}

void SystemTime::Init()
{
	myCurrentTime.SetTime(0);
	myElapsedTime.SetTime(0);
	

	myCallOverhead.QuadPart=0;
	for(int i = 0; i < (10000); i++)
	{
		LARGE_INTEGER start,end;
		QueryPerformanceCounter(&start);
		QueryPerformanceCounter(&end);
		myCallOverhead.QuadPart=end.QuadPart-start.QuadPart;

	}
	myCallOverhead.QuadPart+=5000;
	myCallOverhead.QuadPart/=10000;


	QueryPerformanceFrequency(&myPerformanceTimercalibrationData);
	QueryPerformanceCounter(&myClockStarted);
	myFrameStarted=myClockStarted;

}


void SystemTime::AttachTimer(Timer* aTimer)
{
	myAttachedTimers.push_back(aTimer);
}

bool SystemTime::Update()
{
	LARGE_INTEGER frameEnd;
	QueryPerformanceCounter(&frameEnd);
	myClockStarted.QuadPart+=myCallOverhead.QuadPart; // to compensate for calling overhead to get a dead on accurate timer

	double lastFrameTime=myCurrentTime.GetTime();
	myCurrentTime.SetTime(static_cast<double>((frameEnd.QuadPart-myClockStarted.QuadPart))/static_cast<double>(myPerformanceTimercalibrationData.QuadPart));
	myElapsedTime.SetTime(myCurrentTime.GetTime()-lastFrameTime);
	// do it like this to escape possible problem from round errors making adding all the  elapsed times together different from CurrentTime
	for(unsigned int i = 0; i < myAttachedTimers.size(); i++)
	{
		myAttachedTimers[i]->Update(myElapsedTime);
	}
	QueryPerformanceCounter(&myFrameStarted);

	return(true);
}

const Time& SystemTime::GetTotalTime()  const
{	
	return(myCurrentTime);
}

const Time& SystemTime::GetElapsedTime()  const
{
	return(myElapsedTime);
}
	