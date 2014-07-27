#ifndef TI_SYSTEMTIME_HEADER
#define TI_SYSTEMTIME_HEADER

#include "TI_Timer.h"
#include "windows.h"

namespace TI_Time
{
	class Timer;

	class SystemTime // will share baseclass with Time  once we gain inheritance
	{
	public:
		SystemTime();
		~SystemTime();

		void Init();

		void AttachTimer(Timer* aTimer);

		bool Update();

		const Time& GetTotalTime() const;
		const Time& GetElapsedTime() const;
	private:
		std::vector<Timer*> myAttachedTimers;
		Time myCurrentTime;
		Time myElapsedTime;

		LARGE_INTEGER myPerformanceTimercalibrationData;
		LARGE_INTEGER myClockStarted;
		LARGE_INTEGER myFrameStarted;
		LARGE_INTEGER myCallOverhead;
	


	};
};

#endif