#include "main.hpp"


void pollfd_array_resize_ACA()
{
    cout << "#DEBUG***********: pollfd_array_resize_ACA" << endl;

    if(waitforACA != NULL)
    {
        delete waitforACA;
        waitforACA = NULL;
    }

    if(numberClientsDescriptorsACA != 0)
    {
        waitforACA = new pollfd[numberClientsDescriptorsACA];
        for(unsigned int i = 0; i < clientsDescriptorsACA.size(); i++)
        {
            waitforACA[i].fd = clientsDescriptorsACA[i].desc;
            waitforACA[i].events = POLLIN;
            cout << "#DEBUG-pollfd_array_resize_ACA: Active descriptions " << clientsDescriptors[i] << endl;
            cout << "#DEBUG-pollfd_array_resize_ACA: Active clients number " << numberClientsDescriptorsACA << endl;
        }
    }
}

void control_clientACA()
{
    int readypoll;
    int codeMsg;
    int bytesSR;
    int timeout = 0;

    cout << "#DEBUG-accept_connections_activ: control_clientACA run" << endl;
    while(!manage_thread_ACA)
    {
        unique_lock<std::mutex> lk2(cv_mACA);
        cvACA.wait(lk2, []{return READY_THREAD_GLOBAL_SYNC_ACA;});

        if(numberClientsDescriptorsACA == 0)
        {
            lk2.unlock();
            cvACA.notify_all();
            continue;
        }

        if(numberClientsDescriptorsChangACA == true)
        {
            numberClientsDescriptorsChangACA = false;
            pollfd_array_resize_ACA();
        }

        if(numberClientsDescriptorsACA != 0)
        {
            readypoll = poll(waitforACA, numberClientsDescriptorsACA, 5000);
            if(readypoll == -1)
            {
                cout << "#DEBUG-control_clientACA: POLL ERROR" << endl;
                lk2.unlock();
                cvACA.notify_all();
                continue;
            }
            else if(readypoll == 0)
            {
                cout <<"#DEBUG-control_clientACA: POLL TIMEOUT" << endl;
                for(unsigned int i = 0; i < clientsDescriptorsACA.size(); i++)
                {
                    cout << "#DEBUG-control_clientACA: test client with special id" << clientsDescriptorsACA[i].id  << endl;
                    cout << "#DEBUG-control_clientACA: test client desc " << clientsDescriptorsACA[i].desc  << endl;
                    bytesSR  = send(clientsDescriptorsACA[i].desc, &codeMsg, sizeof(codeMsg), MSG_NOSIGNAL);
                    if(bytesSR == -1)
                    {
                        cout << "#DEBUG-control_clientACA: close special id" << clientsDescriptorsACA[i].id  << endl;
                        close(clientsDescriptorsACA[i].desc);
                        clientsDescriptorsACA.erase(clientsDescriptorsACA.begin() + i);
                        numberClientsDescriptorsChangACA = true;
                        i = numberClientsDescriptorsACA;
                        --numberClientsDescriptorsACA;
                        timeout = 0;
                    }
                    else ++timeout;
                }

                if(timeout > 3)
                {
                    for(unsigned int i = 0; i < clientsDescriptorsACA.size(); i++) close(clientsDescriptorsACA[i].desc);
                    clientsDescriptorsACA.clear();
                    numberClientsDescriptorsACA = 0;
                    timeout = 0;
                }

                lk2.unlock();
                cvACA.notify_all();
                continue;
            }
            else
            {
                if(waitforACA != NULL)
                {
                    for(int i = 0; i < numberClientsDescriptorsACA; i++)
                        if(waitforACA[i].revents & POLLIN)
                        {
                            bytesSR = recv(waitforACA[i].fd, &codeMsg, sizeof(codeMsg), 0);
                            if(bytesSR > 0)
                            {
                                //cout << "#DEBUG-control_clientACA: recv bytes " << bytesSR << " code_msg " << codeMsg << endl;
                                if(!client_handle_activ(waitforACA[i].fd, codeMsg))
                                {
                                    cout <<"#DEBUG-control_clientACA:  control_clientACA Delete client desc" << waitforACA[i].fd << endl;
                                    close(waitforACA[i].fd);
                                    clientsDescriptorsACA.erase(clientsDescriptorsACA.begin() + i);
                                    numberClientsDescriptorsChangACA = true;
                                    --numberClientsDescriptorsACA;
                                }
                            }
                            else
                            {
                                cout <<"#DEBUG-control_clientACA:  control_clientACA Delete client desc" << waitforACA[i].fd  << endl;
                                close(waitforACA[i].fd);
                                clientsDescriptorsACA.erase(clientsDescriptorsACA.begin() + i);
                                numberClientsDescriptorsChangACA = true;
                                --numberClientsDescriptorsACA;
                            }
                        }
                }
            }
        }

        lk2.unlock();
        cvACA.notify_all();
    }
    cout << "#DEBUG-accept_connections_activ: control_clientACA stop" << endl;
}
