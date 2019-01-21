#include <iostream>
#include <string.h>
#include <sys/socket.h>
using namespace std;

void ControlRX(int &stan, int &przeplywnosc, int socket_)
{
    char buffer[150] = {};
    while (stan)
    {
        if (recv(socket_, buffer, sizeof(buffer), 0) <= 0)
        {
            perror("recv() ERROR");
            exit(5);
        }
        if(buffer[0] == 0)
        {
            stan = 0;
            cout<<"Koniec testu"<<endl;
            break;
        }
        else
        {
        przeplywnosc = atoi(buffer);
        cout<<"Zmieniono przelywnosc na: "<<przeplywnosc<<endl;
        }
    }
}