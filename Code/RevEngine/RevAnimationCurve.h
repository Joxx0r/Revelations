#ifndef _REV_ANIMATION_CURVE_H
#define _REV_ANIMATION_CURVE_H

#include <vector>

struct RevKeyFrame
{
	float m_time;
	float m_val;
};

class RevAnimationCurve
{
public:
	RevAnimationCurve(void);
	~RevAnimationCurve(void);

	void AddKeyFrame(RevKeyFrame aFrame);

	void FinalizeCurve();

	float GetValueFromTime(float aTime);

protected:
	float m_startTime;
	float m_endTime;
	std::vector<RevKeyFrame> m_keyFrames;

};

#endif