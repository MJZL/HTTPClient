#include "stdafx.h"
#include "BasicHTTP.h"
#include <chrono>
#include <thread>
#include "BasicHTTPClient.h"
#include <iostream>
#include "Stopwatch.h"
#include "Monitor.h"


int main()
{

	//BasicHTTP::Request("GET", "soundly.me", 80, "/", NULL, response);


	try
	{
		BasicHTTP* client = new BasicHTTP();
		double micro_elapsed;
		while (true)
		{
			Monitor.Stopwatch.Start();
			string response = client->Get("localhost", 8080, "/temperature/26.5", "userdata");
			micro_elapsed = Monitor.Stopwatch.Stop();
			Print(filePath,message,micro_elapsed);
			Print(response);
			Monitor.Stopwatch.Start();
			response = client->Post("localhost", 8080, "/temperature/30", "userdata");
			micro_elapsed = Monitor.Stopwatch.Stop();
			Print(filelocation,message);
			string message = "The elapsed time in micro is : " + std::to_string(micro_elapsed);
			Print(response);
		}

	}
	catch (const std::exception& ex)
	{
		cout << &ex << endl;

	}

	//std::this_thread::sleep_for(std::chrono::milliseconds(1000));


	return 0;
	/*
	Doing a POST, note the "percent" encoding for 'opt_urlencode':

	HTTPReq("POST", "soundly.me", 80, "/not-a-real-resource",
	"userdata=%7B%22key%22%3A%5B%22value0%22%5D%7D", response);

	*/
}

void Print(std::string &response)
{
	cout << "=============================== response:"
		<< endl
		<< response
		<< endl
		<< "=============================== "
		<< endl;
}
