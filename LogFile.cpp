#include "Engine.h"
using namespace std;

std::ofstream out("output.txt");
namespace Manbat{
	LogFile::LogFile(){}
	LogFile::~LogFile(){
		if(out.good()) out.close();
	}
	void LogFile::Print(std::string s){
		out << s << endl;
	}
	void LogFile::Prince(char c[]){
		out << c << endl;
	}
	std::ofstream& LogFile::operator << (char c[]){
		out << c;
		return out;
	}
	std::ofstream& LogFile::operator << (std::string s){
		out << s;
		return out;
	}
	std::ofstream& LogFile::operator << (double d){
		out << d;
		return out;
	}
	std::ofstream& LogFile::operator << (float f){
		out << f;
		return out;
	}
	std::ofstream& LogFile::operator << (int i){
		out << i;
		return out;
	}
	std::ofstream& LogFile::operator << (bool b){
		if(b) out << "True";
		else out << "False";
		return out;
	}
}