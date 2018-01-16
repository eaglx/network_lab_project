#ifndef MAIN_HPP
#define MAIN_HPP

#include <arpa/inet.h>
#include <iostream>
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
#include <signal.h>
#include <chrono>
#include <condition_variable>
#include <thread>
#include <vector>
#include <cstring>
#include <sys/poll.h>

#define QUEUE_SIZE 10
#define PORT_E 6666
#define PORT_A 7777

#define PAGE_X 26
#define PAGE_Y 45
#define CLIENT_LIMIT 7

using namespace std;

/* STRUCT */
struct ClientSelectText
{
    int descriptor;
    int clientSPECIAL_ID;
    int selectStart;
    int selectEnd;
    int timeoutcount;
    bool allupdate;
};

struct clientACA
{
    int desc;
    int id;
};

extern bool end_program;
extern condition_variable cv;
extern mutex cv_m;
extern bool READY_THREAD_GLOBAL_SYNC;
extern int nSocketDesc;
extern char bufforFE[PAGE_X][PAGE_Y];
extern vector < int > clientsDescriptors;
extern int numberClientsDescriptors;
extern struct ClientSelectText CST[CLIENT_LIMIT];
extern bool numberClientsDescriptorsChang;

extern vector < clientACA > clientsDescriptorsACA;
extern bool numberClientsDescriptorsChangACA;
extern int numberClientsDescriptorsACA;
extern bool manage_thread_ACA;
extern condition_variable cvACA;
extern mutex cv_mACA;
extern bool READY_THREAD_GLOBAL_SYNC_ACA;
extern pollfd *waitforACA;

/* FUNCTIONS */
void client_handle_editor(int nClientDesc_HE, int code_msg_HE);
void control_client();
void control_clientACA();
void test_connectionACA();
void accept_connections_ed();
void accept_connections_activ();
void client_handle_activ(int nClientDesc_ACV, int clientSP_ID, int code_msg_ACV);

#endif
