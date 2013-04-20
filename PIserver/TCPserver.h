/*
 * TCPserver.h
 *
 *  Created on: Apr 19, 2013
 *      Author: gaby
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/unistd.h>
#include <sys/fcntl.h>
#include <strings.h>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <stdlib.h>     /* atoi */
#include <vector>
#include "GPIOClass.h"
#include "SplitSTR.h"

class TCPserver
{
  public:
	TCPserver();
    virtual ~TCPserver();
    int startGPIO();
    int start();
    bool running;
    GPIOClass* gpioLeftFront18;
    GPIOClass* gpioLeftBack17;
    GPIOClass* gpioRightFront23;
    GPIOClass* gpioRightBack22;


  private:
    int         port;
    int 		andro_con;
    int 		sock_descriptor;
    int conn_desc, temp_conn_desc;
    struct sockaddr_in serv_addr, client_addr;
    int client;
    pthread_t android_handle_client_t;

    char buffer[256];
    int len, lValue,rValue;
    int count;


    char *token;
    char *runn;

};


#endif /* TCPSERVER_H_ */
