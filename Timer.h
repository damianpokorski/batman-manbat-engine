#pragma once
#include "Engine.h"
#include <chrono>
namespace Manbat{
	typedef std::chrono::high_resolution_clock Clock;
	class Timer{
		// supposed to be private
	public:
	//public:
		Timer(void);
		~Timer(void);
		float timer_start = 0.0f;
		float stopwatch_start = 0.0f;
		float GetElapsed();
		void Rest(double ms);
		void Reset();
		bool Stopwatch(double ms);
		float getSystemTime();

		// Using steady_clock
		std::chrono::steady_clock::time_point startClock = std::chrono::steady_clock::now();
		float getElapsedClock()
		{
			// Get current timestamp
			std::chrono::steady_clock::time_point currentClock = std::chrono::steady_clock::now();
			// Get elapsed time
			const float elapsedTime = float((currentClock-startClock).count()) * std::chrono::steady_clock::period::num / std::chrono::steady_clock::period::den;
			// replace current clock with new one
			startClock = currentClock;
			// return the time
			return elapsedTime;
		}
		int NewTime = 0;
		int OldTime = 0;
		float dt = 0;
		// using timeGetTime
		float getElapsedTimeGetTime()
		{
			NewTime = timeGetTime();
			dt = (float)(NewTime - OldTime) / 1000; //delta time
			OldTime = NewTime;

			if (dt > (0.016f)) dt = (0.016f);  //delta time
			if (dt < 0.001f) dt = 0.001f;
			return dt;
		}
	};
};