#include "Engine.h"

namespace Manbat{
	Timer::Timer(void){
		Reset();
		timer_start = getSystemTime();
	}
	Timer::~Timer(void){}
	float Timer::GetElapsed(){
		//Debug << "Get Elapsed " << (getSystemTime() - timer_start) << std::endl;
		return (getSystemTime() - timer_start);
	}
	void Timer::Rest(double ms){
		timer_start = getSystemTime();
		//Debug << "Resting" << std::endl;
		while(ms > GetElapsed()){
			//Debug << "SLEEP timer start:" << ((getSystemTime() - timer_start) ) << " Get Elapsed" << GetElapsed() << std::endl;
			//Sleep(1);
		}
	}
	void Timer::Reset(){
		stopwatch_start = timer_start = getSystemTime();
	}

	bool Timer::Stopwatch(double ms){
		if((stopwatch_start) +(ms/1000) < (getSystemTime())){
			//Debug << "Stopwatch true " << std::endl;
			Reset();
			return true;
		}
		else {
			//Debug << "Stopwatch false" << std::endl;
			return false;
		}
	}
	float Timer::getSystemTime()
	{
		//return ((double)std::chrono::system_clock::now().time_since_epoch().count())/1000;
		return ((float)clock()) / CLOCKS_PER_SEC;
	}
}