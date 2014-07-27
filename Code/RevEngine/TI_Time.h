#ifndef TI_TIME_HEADER 
#define TI_TIME_HEADER 

namespace TI_Time
{
	enum Clocks
	{
		SYSTEM_CLOCK,
		MASTER_CLOCK,
		SECONDARY_CLOCK,
		TERTIARY_CLOCK,
		GAME_CLOCK_01,
		GAME_CLOCK_02,
		GAME_CLOCK_03,
		GAME_CLOCK_04,
		GAME_CLOCK_05,
		GAME_CLOCK_06,
		GAME_CLOCK_07,
		GAME_CLOCK_08,
		GAME_CLOCK_09,
		GAME_CLOCK_10,
		GAME_CLOCK_11,
		GAME_CLOCK_12,
		GAME_CLOCK_13,
		GAME_CLOCK_14,
		GAME_CLOCK_15,
		NUM_CLOCKS,
		ANY_CLOCK
	};

	class Time
	{
	public:
		Time();
		Time(const Clocks aClock,const double aTime);
		Time(const Time& aTime);
		~Time();

		Time& operator=(const Time& aTime);
		Time& operator=(const float& aFloat);
		Time& operator=(const double& aDouble);
		Time& operator=(const Clocks& aClock);
		

	//Compound operators
		
		Time& operator*=(const Time& aTime);
		Time& operator+=(const Time& aTime);
		Time& operator-=(const Time& aTime);
		Time& operator*=(const float& aTime);
		Time& operator*=(const double& aTime);
		Time& operator+=(const float& aTime);
		Time& operator+=(const double& aTime);
		Time& operator-=(const float& aTime);
		Time& operator-=(const double& aTime);


	// Comparisson operators

		bool operator<(const Time &aTime) const;
		bool operator>(const Time &aTime) const;
		bool operator<=(const Time &aTime) const;
		bool operator>=(const Time &aTime) const;



		double GetTime() const;
		Clocks GetClock() const;

		void SetTime(const double aTime);
		void SetClock(const Clocks aClock);
	private:
		Clocks myClock;
		double myTime;
	};

		// Time to Time operators
	Time operator+(const Time& aLeftSideTime,const Time& aRightSideTime);
	Time operator-(const Time& aLeftSideTime,const Time& aRightSideTime);
	Time operator*(const Time& aLeftSideTime,const Time& aRightSideTime);		

		//time to float operators

	float operator*(const float& aLeftSideTime,const Time& aRightSideTime);
	Time operator*(const Time& aLeftSideTime,const float& aRightSideTime);
	float operator+(const float& aLeftSideTime,const Time& aRightSideTime);
	double operator+(const double& aLeftSideTime,const Time& aRightSideTime);
	Time operator+(const Time& aLeftSideTime,const float& aRightSideTime);
	Time operator+(const Time& aLeftSideTime,const double& aRightSideTime);


};

#endif