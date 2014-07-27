#include <stdafx.h>
#include "TI_Time.h"
#include <assert.h>


using namespace TI_Time;

Time::Time()
{
	myTime=0;
	myClock=ANY_CLOCK;
}

Time::Time(const Clocks aClock,const double aTime)
{
	myClock=aClock;
	myTime=aTime;
}

Time::Time(const Time& aTime)
{
	myTime=aTime.myTime;
	myClock=ANY_CLOCK;
}

Time::~Time()
{
}

Time& Time::operator=(const Time& aTime)
{
	myTime=aTime.myTime;
	myClock=aTime.myClock;
	return(*this);
}

Time& Time::operator=(const float& aFloat)
{
	myTime=aFloat;
	return(*this);
}

Time& Time::operator=(const double& aDouble)
{
	myTime=aDouble;
	return(*this);
}

Time& Time::operator=(const Clocks& aClock)
{
	myClock=aClock;
	return(*this);

}

double Time::GetTime() const
{
	return(myTime);
}

Clocks Time::GetClock() const
{
	return(myClock);
}

void Time::SetTime(const double aTime)
{
	myTime=aTime;
}

void Time::SetClock(const Clocks aClock)
{
	myClock=aClock;
}

Time& Time::operator*=(const Time& aTime)
{
	assert((myClock>=aTime.myClock) || (ANY_CLOCK==myClock) || (ANY_CLOCK==aTime.myClock));
	myTime*=aTime.myTime;
	return(*this);

}

Time& Time::operator+=(const Time& aTime)
{
	assert((myClock>=aTime.myClock) || (ANY_CLOCK==myClock) || (ANY_CLOCK==aTime.myClock));
	myTime+=aTime.myTime;
	return(*this);
}

Time& Time::operator-=(const Time& aTime)
{
	assert((myClock>=aTime.myClock) || (ANY_CLOCK==myClock) || (ANY_CLOCK==aTime.myClock));
	myTime-=aTime.myTime;
	return(*this);
}


Time& Time::operator+=(const float& aTime)
{
	myTime+=aTime;
	return(*this);
}

Time& Time::operator+=(const double& aTime)
{
	myTime+=aTime;
	return(*this);
}

Time& Time::operator-=(const float& aTime)
{
	myTime-=aTime;
	return(*this);
}

Time& Time::operator-=(const double& aTime)
{
	myTime-=aTime;
	return(*this);
}

Time& Time::operator*=(const float& aTime)
{
	myTime*=aTime;
	return(*this);
}

Time& Time::operator*=(const double& aTime)
{
	myTime*=aTime;
	return(*this);
}

bool Time::operator<(const Time &aTime) const 
{
	assert((myClock==aTime.myClock) || (ANY_CLOCK==myClock) || (ANY_CLOCK==aTime.myClock));
	return(myTime<aTime.myTime);
}

bool Time::operator>(const Time &aTime) const 
{
	assert((myClock==aTime.myClock) || (ANY_CLOCK==myClock) || (ANY_CLOCK==aTime.myClock));
	return(myTime>aTime.myTime);
}

bool Time::operator<=(const Time &aTime) const 
{
	assert((myClock==aTime.myClock) || (ANY_CLOCK==myClock) || (ANY_CLOCK==aTime.myClock));
	return(myTime<=aTime.myTime);
}

bool Time::operator>=(const Time &aTime) const 
{
	assert((myClock==aTime.myClock) || (ANY_CLOCK==myClock) || (ANY_CLOCK==aTime.myClock));
	return(myTime>=aTime.myTime);
}

Time operator+(const Time& aLeftSideTime,const Time& aRightSideTime)
{
	assert((aLeftSideTime.GetClock()==aRightSideTime.GetClock()) || (ANY_CLOCK==aLeftSideTime.GetClock()) || (ANY_CLOCK==aRightSideTime.GetClock()));
	Time returnValue(aLeftSideTime);
	returnValue+=aRightSideTime;
	return(returnValue);

}

Time operator-(const Time& aLeftSideTime,const Time& aRightSideTime)
{
	assert((aLeftSideTime.GetClock()==aRightSideTime.GetClock()) || (ANY_CLOCK==aLeftSideTime.GetClock()) || (ANY_CLOCK==aRightSideTime.GetClock()));
	Time returnValue(aLeftSideTime);
	returnValue-=aRightSideTime;
	return(returnValue);

}

Time operator*(const Time& aLeftSideTime,const Time& aRightSideTime)
{
	assert((aLeftSideTime.GetClock()==aRightSideTime.GetClock()) || (ANY_CLOCK==aLeftSideTime.GetClock()) || (ANY_CLOCK==aRightSideTime.GetClock()));
	Time returnValue(aLeftSideTime);
	returnValue*=aRightSideTime;
	return(returnValue);

}




float operator*(const float& aLeftSideTime,const Time& aRightSideTime) 
{
	return(static_cast<float>(aLeftSideTime*aRightSideTime.GetTime()));
}

Time operator*(const Time& aLeftSideTime,const float& aRightSideTime) 
{
	Time returnValue(aLeftSideTime);
	returnValue*=aRightSideTime;
	return(returnValue);
}


Time operator+(const Time& aLeftSideTime,const float& aRightSideTime) 
{
	Time returnValue(aLeftSideTime);
	returnValue+=aRightSideTime;
	return(returnValue);
}


float operator+(const float& aLeftSideTime,const Time& aRightSideTime)
{
	return(static_cast<float>(aLeftSideTime+aRightSideTime.GetTime()));
}

Time operator+(const Time& aLeftSideTime,const double& aRightSideTime)
{
	Time returnValue(aLeftSideTime);
	returnValue+=aRightSideTime;
	return(returnValue);

}

double operator+(const double& aLeftSideTime,const Time& aRightSideTime)
{
	return(aLeftSideTime+aRightSideTime.GetTime());
}
