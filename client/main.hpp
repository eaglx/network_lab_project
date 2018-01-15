#ifndef _MAIN_HPP
#define _MAIN_HPP

#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/stat.h>
#include <time.h>
#include <signal.h>
#include <thread>

#define PAGE_X 26
#define PAGE_Y 45

using namespace std;

extern int socketDesc;
extern string servIPaddr;
extern int servPORT;
extern bool end_program;
extern bool reconnect;
extern struct tm *foo;
extern struct stat attrib;

void manage_editor();

#endif
