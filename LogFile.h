#pragma once
// Include Thomas the dank engine
#include "Engine.h"

namespace Manbat{
	class LogFile{
		public:
			LogFile();
			~LogFile();
			void Print(std::string s);
			void Prince(char c[]);
			std::ofstream& operator << (char c[]);
			std::ofstream& operator << (std::string s);
			std::ofstream& operator << (double d);
			std::ofstream& operator << (float f);
			std::ofstream& operator << (int i);
			std::ofstream& operator << (bool b);
	};
}
static Manbat::LogFile Debug;
