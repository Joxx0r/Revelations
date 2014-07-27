#include "stdafx.h"
#include "RevAnimationCurve.h"


RevAnimationCurve::RevAnimationCurve(void)
{
	m_keyFrames.reserve( 10000 );
}

RevAnimationCurve::~RevAnimationCurve(void)
{
}

void RevAnimationCurve::AddKeyFrame( RevKeyFrame aFrame)
{
	m_keyFrames.push_back( aFrame );
}

void RevAnimationCurve::FinalizeCurve()
{
	assert( m_keyFrames.size() > 0 );
	m_startTime=m_keyFrames[0].m_time;
	m_endTime=m_keyFrames[m_keyFrames.size()-1].m_time;
}

float RevAnimationCurve::GetValueFromTime(float aTime)
{
	assert( m_keyFrames.size() > 0 );
	float time = aTime;

	if(time>m_endTime) 
	{
		time = fmod(aTime, m_endTime);
	}

	if(time<=m_startTime) return(m_keyFrames[0].m_val);
	if(time>m_endTime)
	{
		return(m_keyFrames[m_keyFrames.size()-1].m_val);
	}
	for(int i=0;i<m_keyFrames.size()-1;i++)
	{
		if(m_keyFrames[i].m_time>=time)
		{
			const float progress=(time-m_keyFrames[i-1].m_time)/(m_keyFrames[i].m_time-m_keyFrames[i-1].m_time);
			return(m_keyFrames[i-1].m_val+(m_keyFrames[i].m_val-m_keyFrames[i-1].m_val)*progress);
		}
	}
	return(m_keyFrames[0].m_val);
}
