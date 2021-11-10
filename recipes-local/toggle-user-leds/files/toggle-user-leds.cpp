/**
 * program to control blink of user leds except led 1
**/
#include <iostream>
#include <fstream>
#include <exception>
#include <unistd.h>
#include <signal.h>

using namespace std;

#define ARGS_COUNT (2)
#define FIRST_LED  (2)
#define LAST_LED  (4)

const string ledsMap[] = {"53", "54", "55", "56"};

int ledNumber;
ofstream ofs;

void signalHandler(int sigNumber)
{
	switch (sigNumber)
	{
	case SIGINT:
	case SIGTSTP:
	case SIGTERM:
		cout << "SIGINT or SIGSTP received, terminating program." << endl;
		ofs << "0";
		ofs.flush();
		ofs.close();
		exit(0);
		break;
	
	default:
		break;
	}
}

int main(int argc, char *argv[])
{

	signal(SIGINT, signalHandler);
	signal(SIGTSTP, signalHandler);
	signal(SIGTERM, signalHandler);

	if (ARGS_COUNT == argc)
	{
		ledNumber = atoi(argv[1]);

		if ((ledNumber < FIRST_LED) || (ledNumber > LAST_LED))
		{
			cerr << "Incorrect LED number."<<endl;
			cerr << "Three available LEDs (2,3,4)." << endl;
			return 1;
		}
	}
	else
	{
		cerr << "Incorrect number, Please supply LED number." << endl;
		cerr << "Three available LEDs (2,3,4)." << endl;
		return 1;
	}


	/* decrement to match array mapping */
	ledNumber--;

	try
	{
		/* Pin export */
		ofs.open("/sys/class/gpio/export", ofstream::out);
		ofs << ledsMap[ledNumber];
		ofs.close();
		cout << "Pin exported." << endl;

		/* Pin direction */
		ofs.open("/sys/class/gpio/gpio" + ledsMap[ledNumber] + "/direction", ofstream::out);
		ofs << "out";
		ofs.close();
		cout << "Diection is set correclty." << endl;
	}
	catch(const exception& e)
	{
		cerr << "Error setting pin direction." << endl;
		return 1;
	}

	/* Pin value */
	ofs.open("/sys/class/gpio/gpio" + ledsMap[ledNumber] + "/value", ofstream::out);
	while (1)
	{

		ofs << "1";
		ofs.flush();
		sleep(3);
		ofs << "0";
		ofs.flush();
		sleep(3);
	}

	return 0;
}
