#ifndef MAIN_HPP
#define MAIN_HPP

#include <algorithm>
#include <arpa/inet.h>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <string>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <thread>
#include <unistd.h>
#include <vector>

#define QUEUE_SIZE 20
#define PORT 6666

#define FLAG_INSERT_BEFORE 111
#define FLAG_REPLACE 222

#define SEND_ALL_DATA 0
#define SEND_ERROR -1
#define RECIVE_ZERO 0
#define RECIVE_ERROR -1

using namespace std;

#define PACKETSIZE sizeof(MESSAGE_INFO)

typedef struct MESSAGE_INFO
{
    int flag;
    int posX;
    int posY;
    char chr;
};

#endif