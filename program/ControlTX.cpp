#include <iostream>
#include <string.h>
#include <sys/socket.h>
using namespace std;

//Funkcja słuząca do zarządzania działaniem programu w części odbiorczej
//Umożliwia komunikację z użytkownikiem w trakcie działania programu
//Za jego pomocą można zakonczyć działanie programu jak i zmienić przepływność nadawanych pakietów

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
            //W przypadku podania nieprawidłowej wartości jest czyszczony bufor i flagi błędu 
            //aby uzytkownik miał możliwość podania poprawnej wartości
            cin.clear(); 
            cin.sync();
            string cl;
            getline(cin, cl);
            cout << "Błędna wartość" << endl;
        }
        }
    }
}
