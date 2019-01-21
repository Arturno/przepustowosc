#include <iostream>
#include <stdlib.h>    // exit()
#include <string.h>    // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include "OgolneInfo.cpp"

#include "transmitter.cpp"
#include "receiver.cpp"
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <thread>

using namespace std;
int main()
{
    int i;
    cout<<"Program do pomiaru przepływności łącza Wifi"<<endl;
    cout<<"0 - aby uruchomić w trybie TX"<<endl;
    cout<<"1 - aby uruchomić w trybie RX"<<endl;
    cin>>i;
    if(i==0)
    {
        transmitter();
    }
    else
    {
        receiver();
    }

   return 0;
}