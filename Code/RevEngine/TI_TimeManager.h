#ifndef TI_TIMEMANAGER_HEADER
#define TI_TIMEMANAGER_HEADER

#include "TI_time.h"
#include <vector>

namespace TI_Time
{
	class Timer;
	class SystemTime;

	class TimeManager
	{
	public:
		TimeManager();
		~TimeManager();
		
		void InitDefaults();

		bool Update();

		void CreateTimer(const Clocks aTimer);
		bool AttachTimerToTimer(const Clocks aTimerToAttachTo,const Clocks aTimerToAttach);

		const Time& GetElapsedTime() const;
		const Time& GetTotalTime() const;

		const Time& GetElapsedTime(const Clocks aTimer) const;
		const Time& GetTotalTime(const Clocks aTimer) const;
		bool SetClockPauseState(const Clocks aTimer,const bool aPauseState);

		void SetCurrentDefaultClock(const Clocks aTimer);
		
	private:
		Clocks myDefaultClock;
		std::vector<Timer*> myClocks;
		SystemTime* mySystemClock;
	};
};

#endif