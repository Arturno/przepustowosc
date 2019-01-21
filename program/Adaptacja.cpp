#include <iostream>
#include <unistd.h>
#include "PID.cpp"
#include <climits>

void adaptacja(int &licznik, int &do_wyslania, int &grupa_pakietow, int &opoznienie, int &stan, int czas_adaptacji)
{
    sleep(1);
    int wyslane;
    int licznik_wyslane = 0;
    PID pid = PID(0.1, 200, -200, 0.1, 0.01, 0.5);
    while (stan)
    {
        int ostatnio = licznik;
        int opoznienie_docelowe = 1000000 / (do_wyslania/grupa_pakietow);
        usleep(czas_adaptacji*1000);
        if (ostatnio <= licznik)
        {
            wyslane = licznik - ostatnio;
        }
        else
        {
            wyslane = INT_MAX - ostatnio + licznik - INT_MIN;
        }
        //cout<<"Wyslano: "<<wyslane<<endl;
        if (wyslane != 0)
        {
            double opoznienie_obecne = (czas_adaptacji*1000) / wyslane;
            double roznica = opoznienie_obecne - opoznienie_docelowe;
            //cout<<"Roznica"<< roznica<<endl;
            //cout << "OP obecne:" << opoznienie_obecne << " " << opoznienie_docelowe << endl;
            //cout << "Opoznienie" << opoznienie << endl;
            double inc = pid.calculate(0, roznica);
            
            //cout << "inc " << inc << endl;
            opoznienie += inc;
            //cout<<"OP:"<<opoznienie<<" INC:"<<inc<<endl;
            if(opoznienie<0)
            {
                opoznienie =0;
                //cout<<"MAX"<<endl;
            }
            licznik_wyslane = 0;
        }
        else
        {
            licznik_wyslane++;
            if (licznik_wyslane == 10000000 / (czas_adaptacji*1000))
            {
                cout <<"ZERO"<< endl;
                break;
            }
        }
    }
}
