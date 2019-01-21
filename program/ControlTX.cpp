#include <iostream>
#include <string.h>
#include <sys/socket.h>
using namespace std;

void ControlTX(int &stan, class SterowanieTX &ster, int socket_)
{
    char change = {};
    char buffer[150];
    while (stan)
    {
        cin >> change;
        switch (change)
        {
        case 'E':
            ster.koniec();
            buffer[0] = 0;
            cout << "Koniec testu" << endl;
            if (send(socket_, buffer, sizeof(buffer), 0) <= 0)
            {
                perror("send() ERROR");
                exit(6);
            }
            break;
        case 'M':
            int przeplywnosc;
            przeplywnosc = getNumber(1, 1000);
            ster.zmiana_przeplywnosci(przeplywnosc);
            cout<<"#############################################################################"<<endl;
            cout << "Zmieniono przepływoność na:" << przeplywnosc << "Mb/s" << endl;
            ster.wypisanie();
            cout<<"#############################################################################"<<endl;
            sprintf(buffer, "%d", przeplywnosc);
            if (send(socket_, buffer, sizeof(buffer), 0) <= 0)
            {
                perror("send() ERROR");
                exit(6);
            }
            break;
        default:
        {
            cin.clear(); // czyścimy flagi błędu strumienia
            cin.sync();
            string cl;
            getline(cin, cl);
            cout << "Błędna wartość" << endl;
        }
        }
    }
}