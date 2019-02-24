#pragma once
#include <stdio.h>
#include <fstream>
#include "Stopwatch.h"

class Monitor
{
public: 
	std::string Logger;
	std::string getCurrentDateTime;

	void message()
	{
		std::cout << Logger;
	}


};

inline string getCurrentDateTime(string s) {
	time_t now = time(0);
	struct tm  tstruct;
	char  buf[80];
	tstruct = *localtime(&now);
	if (s == "now")
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	else if (s == "date")
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return string(buf);
};
inline void Logger(string logMsg) {

	string filePath = "/basichttpclient/log_" + getCurrentDateTime("date") + ".txt";
	string now = getCurrentDateTime("now");
	ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << now << '\t' << logMsg << '\t' << Time_measurement << '\n';
	ofs.close();
}
