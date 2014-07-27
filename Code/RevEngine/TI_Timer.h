#ifndef TI_TIMER_HEADER
#define TI_TIMER_HEADER

#include "TI_Time.h"
#include <vector>

namespace TI_Time
{
	class Timer
	{
	public:
		Timer(Clocks aClock=MASTER_CLOCK);
		~Timer();

		void AttachTimer(Timer* aTimer);

		bool Update(Time& aElapsedTime);
		void SetPauseState(bool aPausedFlag);

		const Time& GetTotalTime() const;
		const Time& GetElapsedTime() const;
	private:
		bool myIsPausedFlag;
		std::vector<Timer*> myAttachedTimers;
		Time myCurrentTime;
		Time myElapsedTime;
	};
};

#endif