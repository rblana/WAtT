/*
 * TCPserver.cpp
 *
 *  Created on: Apr 19, 2013
 *      Author: gaby
 */



#include "TCPserver.h"

using namespace std;


TCPserver::TCPserver() {port = 22556;running=true;};

TCPserver::~TCPserver()
{
	running= false;

}

int TCPserver::startGPIO()
{
    gpioLeftFront18 = new GPIOClass("18"); //create new GPIO object to be attached to  GPIO18
    gpioLeftBack17 = new GPIOClass("17"); //create new GPIO object to be attached to  GPIO17
    gpioRightFront23 = new GPIOClass("23"); //create new GPIO object to be attached to  GPIO23
    gpioRightBack22 = new GPIOClass("22"); //create new GPIO object to be attached to  GPIO22

    gpioLeftFront18->export_gpio(); //export GPIO18
    gpioLeftBack17->export_gpio(); //export GPIO17
    gpioRightFront23->export_gpio(); //export GPIO18
    gpioRightBack22->export_gpio(); //export GPIO17

    cout << " GPIO pins exported" << endl;

    gpioLeftFront18->setdir_gpio("out"); //GPIO4 set to output
    gpioLeftBack17->setdir_gpio("out"); // GPIO17 set to input
    gpioRightFront23->setdir_gpio("out"); //GPIO4 set to output
    gpioRightBack22->setdir_gpio("out"); // GPIO17 set to input

    cout << " Set GPIO pin directions" << endl;


    //loop 5 times ( for 5 seconds)
    int count = 0;
    while(count < 5)
    {
        usleep(1000000);  // wait for 0.5 seconds
        if ((count % 2) == 0)
        {
        	cout << "Swapping PINS step" << count <<endl;
        	gpioLeftFront18->setval_gpio("1"); // turn LED ON
        	gpioLeftBack17->setval_gpio("0"); // turn LED OFF
        }else
        {
        	cout << "Swapping PINS step" << count <<endl;
        	gpioLeftFront18->setval_gpio("0"); // turn LED ON
        	gpioLeftBack17->setval_gpio("1"); // turn LED OFF
        }
        count++;
    }
	gpioLeftFront18->setval_gpio("0"); // turn LED ON
	gpioLeftBack17->setval_gpio("0"); // turn LED OFF
	gpioRightFront23->setval_gpio("0"); // turn LED ON
	gpioRightBack22->setval_gpio("0"); // turn LED OFF
	return 0;
}
int TCPserver::start()
{


	sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(sock_descriptor, F_SETFL, O_NONBLOCK);

	// A valid descriptor is always a positive value
	if(sock_descriptor < 0)
	  cout << "Failed creating socket\n";

		// Initialize the server address struct to zero
	bzero((char *)&serv_addr, sizeof(serv_addr));

	// Fill server's address family
	serv_addr.sin_family = AF_INET;

	// Server should allow connections from any ip address
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	// 16 bit port number on which server listens
	// The function htons (host to network short) ensures that an integer is interpretted
	// correctly (whether little endian or big endian) even if client and server have different architectures
	serv_addr.sin_port = htons(port);

	// Attach the server socket to a port. This is required only for server since we enforce
	// that it does not select a port randomly on it's own, rather it uses the port specified
	// in serv_addr struct.
	int bindret = -1;
	while(bindret<0)
	{
		if ((bindret = bind(sock_descriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
		{
			printf("Failed to bind to port %d bindret:%d\n",bindret, port);
		}
		else
		{
			printf("Sock bind to port %d \t sock_descriptor:%d \t bindret:%d\n",port,sock_descriptor,bindret);
			int flag = 1;
			int result = setsockopt(sock_descriptor,            /* socket affected */
								  IPPROTO_TCP,     /* set option at TCP level */
								  TCP_NODELAY,     /* name of option */
								  (char *) &flag,  /* the cast is historical
														  cruft */
								  sizeof(int));    /* length of option value */
			if (result < 0)
				printf("ERRROR setting TCP_NODELAY");

			int opt=1;        /* option is to be on/TRUE or off/FALSE */

			setsockopt(sock_descriptor,SOL_SOCKET,SO_REUSEADDR,(char *)&opt,sizeof(opt));
		}

		usleep(500000);
	}
	// Server should start listening - This enables the program to halt on accept call (coming next)
	// and wait until a client connects. Also it specifies the size of pending connection requests queue
	// i.e. in this case it is 5 which means 5 clients connection requests will be held pending while
	// the server is already processing another connection request.
	listen(sock_descriptor, 5);


	socklen_t size = sizeof(client_addr);

	// Server blocks on this call until a client tries to establish connection.
	// When a connection is established, it returns a 'connected socket descriptor' different
	// from the one created earlier.
	//int count = 0;

	while(running)
	{

		//count++;
		printf("Waiting for android connection...\n");
		temp_conn_desc = accept(sock_descriptor, (struct sockaddr *)&client_addr, &size);

		if (temp_conn_desc == -1)
		{
			if(andro_con == 0)
				cout << "No android connection found \n";
			//if(count>20*3)
				//count =0;
		}
		else
		{
			printf("New Android Connected \t temp_conn_desc:%d\n",temp_conn_desc);

			andro_con = 0;
			usleep(500000);

			if(temp_conn_desc != conn_desc)
			{
				if(conn_desc != 0)
				{
					printf("New client connected. Disconnecting previous Client. \n");

					close(conn_desc);
					//pthread_exit(&android_handle_client_t);
					printf("Client disconnected conn_desc:%d\n",conn_desc);
				}
			}

			conn_desc = temp_conn_desc;
			andro_con = 1;


			while(andro_con)
			{
				 char delimiters[] = "#";
				printf("Waiting for data \n");
				bzero(buffer,256);
				// The new descriptor can be simply read from / written up just like a normal file descriptor
				int recv = read(conn_desc, buffer, sizeof(buffer)-1);
				printf("after read \n");
				if ( recv > 0)
				{
					printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
					printf("ANDROID RX: %s\n", buffer);
					printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");

					splitstring *ss = new splitstring(buffer);
					vector<string> strs = ss->split('#',0);
					printf("size RX: %d\n", strs.size());
					if(strs.size() >2)
					{
						len = atoi(strs[0].c_str());
						printf("MSG len: %d\n", len);

						lValue = atoi(strs[1].c_str());
						printf("left: %d\n", lValue);

						rValue = atoi(strs[2].c_str());
						printf("right: %d\n", rValue);
					}
					if(lValue == 0)
					{
						printf("left == 0 gpioLeftFront4=0 gpioLeftBack17=0\n");
						gpioLeftFront18->setval_gpio("0"); // turn LED OFF
						gpioLeftBack17->setval_gpio("0"); // turn LED OFF
					}
					if(lValue > 0)
					{
						printf("left > 0 gpioLeftFront4=1 gpioLeftBack17=0\n");
						gpioLeftFront18->setval_gpio("1"); // turn LED OFF
						gpioLeftBack17->setval_gpio("0"); // turn LED OFF
					}
					if(lValue < 0)
					{
						printf("left < 0 gpioLeftFront4=0 gpioLeftBack17=1\n");
						gpioLeftFront18->setval_gpio("0"); // turn LED OFF
						gpioLeftBack17->setval_gpio("1"); // turn LED OFF
					}

					////// right  ///////////////

					if(rValue == 0)
					{
						printf("right == 0 gpioLeftFront4=0 gpioLeftBack17=0\n");
						gpioRightFront23->setval_gpio("0"); // turn LED OFF
						gpioRightBack22->setval_gpio("0"); // turn LED OFF
					}
					if(rValue > 0)
					{
						printf("right > 0 gpioLeftFront4=1 gpioLeftBack17=0\n");
						gpioRightFront23->setval_gpio("1"); // turn LED OFF
						gpioRightBack22->setval_gpio("0"); // turn LED OFF
					}
					if(rValue < 0)
					{
						printf("right < 0 gpioLeftFront4=0 gpioLeftBack17=1\n");
						gpioRightFront23->setval_gpio("0"); // turn LED OFF
						gpioRightBack22->setval_gpio("1"); // turn LED OFF
					}

				}
				else
				{
					printf("Client Disconnected\n");
					printf("END\n");
					andro_con = 0;

					close(conn_desc);
					conn_desc = 0;

					printf("Thread ended andro_handle_client_Thread \n");
					break;
				}

				usleep(10000);

			}

		}
		usleep(500000);
	}
	// Program should always close all sockets (the connected one as well as the listening one)
	// as soon as it is done processing with it
	printf("Clossing TCP listner sock_descriptor:%d.\n",sock_descriptor);
	close(sock_descriptor);
}
