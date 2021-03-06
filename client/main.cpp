#include "mainwindow.h"
#include <QApplication>
#include "connectdatamanage.h"

std::ofstream logFile;
volatile bool isEndProgram = false;
std::string dataFromQTextEdit;
std::string dataFromServer;
int socketDesc;
std::mutex myMutex;
std::condition_variable myConditionVariable;
volatile bool readyM_CV = true;
volatile bool sendDATA = false;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    int returnedValueEventLoop;

    struct sockaddr_in serverAddr;
#define BUFF_SIZE 50
    char buffer[BUFF_SIZE];
    int byteGet;

    std::ifstream configFile("config_file.conf");
    std::string ip_addr;
    int servPORT;
    std::string temp;

    logFile.open("log.txt");
    if (!logFile.is_open())
    {
        return -1;
    }

    if(configFile.is_open())
    {
      getline(configFile, ip_addr);
      getline(configFile, temp);
      servPORT = atoi(temp.c_str());
      configFile.close();
      logFile << "#DEBUG: configurations loaded\n";
    }
    else
    {
        logFile <<"#ERROR: no config found!\n";
        return -1;
    }

    socketDesc = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketDesc < 0)
    {
        logFile << "#ERROR: Failed create socket!!!\n";
        logFile.close();
        return -2;
    }

    serverAddr.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(servPORT);

    if(connect(socketDesc, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
    {
        logFile << "#ERROR: Cannot connect to server!!!\n";
        logFile.close();
        return -3;
    }
    dataFromServer = "";
    for(int i = 0; i < BUFF_SIZE; i++) { buffer[i] = '\0'; }
    while(true)
    {
        byteGet = recv(socketDesc, &buffer, sizeof(char) * BUFF_SIZE, 0);
        logFile << "#INFO: recv bytes " << byteGet << "\n";
        if(byteGet < 0)
        {
            logFile << "#ERROR: recv\n";
            close(socketDesc);
            logFile.close();
            return -4;
        }
        else if(byteGet == 0) break;

        dataFromServer = dataFromServer + std::string(buffer);
        if(byteGet < int(sizeof(char) * BUFF_SIZE)) break;
    }
    if(dataFromServer == "?/?/#") {
        dataFromServer = "";
    }
    std::thread listenTH(listen_from_server, &w);
    w.show();
    returnedValueEventLoop = a.exec();
    logFile << "#INFO: Event loop return value " << returnedValueEventLoop << "\n";
    logFile.close();
    isEndProgram = true;
    close(socketDesc);
    listenTH.join();
    return returnedValueEventLoop;
}
