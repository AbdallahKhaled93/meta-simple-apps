/**
 * Daemon for blinking led 1
**/
#include <iostream>
#include <fstream>
#include <exception>
#include <unistd.h>
#include <signal.h>

using namespace std;

#define BACKGROUND_LED_PIN ((string)"53")

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

	if(daemon(0,0) == -1)
	{
		cerr << "Fork error" << endl;
	}

	try
	{
		/* Pin export */
		ofs.open("/sys/class/gpio/export", ofstream::out);
		ofs << BACKGROUND_LED_PIN;
		ofs.close();
		cout << "Pin exported." << endl;

		/* Pin direction */
		ofs.open("/sys/class/gpio/gpio" + BACKGROUND_LED_PIN + "/direction", ofstream::out);
		ofs << "out";
		ofs.close();
		cout << "Direction is set correclty." << endl;
	}
	catch(const exception& e)
	{
		cerr << "Error during setting pin direction." << endl;
		return 1;
	}

	/* Pin value */
	ofs.open("/sys/class/gpio/gpio" + BACKGROUND_LED_PIN + "/value", ofstream::out);
	while (1)
	{

		ofs << "1";
		ofs.flush();
		sleep(5);
		ofs << "0";
		ofs.flush();
		sleep(5);
	}

	return 0;
}
