/*
 * PIserver.cpp
 *
 *  Created on: Apr 2, 2013
 *      Author: gaby
 */
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <strings.h>
#include "GPIOClass.h"
#include "thread.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "TCPserver.h"

using namespace std;

void sig_handler(int sig);

static bool ctrl_c_pressed = false;
TCPserver* server;

int main (void)
{
    cout << "Raspberry PI server !!! v.1.0" << endl;

    struct sigaction sig_struct;
    sig_struct.sa_handler = sig_handler;
    sig_struct.sa_flags = 0;
    sigemptyset(&sig_struct.sa_mask);

    if (sigaction(SIGINT, &sig_struct, NULL) == -1) {
        cout << "Problem with sigaction" << endl;
        exit(1);
    }

    string inputstate;
/*
    GPIOClass* gpioLeftFront4 = new GPIOClass("4"); //create new GPIO object to be attached to  GPIO4
    GPIOClass* gpioLeftBack17 = new GPIOClass("17"); //create new GPIO object to be attached to  GPIO17

    gpioLeftFront4->export_gpio(); //export GPIO4
    gpioLeftBack17->export_gpio(); //export GPIO17

    cout << " GPIO pins exported" << endl;

    gpioLeftFront4->setdir_gpio("out"); //GPIO4 set to output
    gpioLeftBack17->setdir_gpio("out"); // GPIO17 set to input

    cout << " Set GPIO pin directions" << endl;



    //loop 30 times ( for 10 seconds)
    int count = 0;
    while(count < 30)
    {
        usleep(1000000);  // wait for 0.5 seconds
        if ((count % 2) == 0)
        {
        	cout << "Swapping PINS step" << count <<endl;
        	gpioLeftFront4->setval_gpio("1"); // turn LED ON
        	gpioLeftBack17->setval_gpio("0"); // turn LED OFF
        }else
        {
        	cout << "Swapping PINS step" << count <<endl;
        	gpioLeftFront4->setval_gpio("0"); // turn LED ON
        	gpioLeftBack17->setval_gpio("1"); // turn LED OFF
        }
        count++;
        if(ctrl_c_pressed)
		{
			break;

		}
    }

    gpioLeftFront4->unexport_gpio();
    gpioLeftBack17->unexport_gpio();
	cout << "deallocating GPIO Objects" << endl;
	delete gpioLeftFront4;
	gpioLeftFront4 = 0;
	delete gpioLeftBack17;
	gpioLeftBack17 =0;
*/

    cout << " Waiting for Android" << endl;
	server = new TCPserver();
	server->running = true;
	server->startGPIO();
	server->start();

    // Should never get here
    return 0;

}
void sig_handler(int sig)
{
    write(0,"\nCtrl^C pressed in sig handler\n",32);
    server->running = false;
    ctrl_c_pressed = true;
}



