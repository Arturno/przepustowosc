#include <iostream>
#include <cstdio>
#include <fstream>
#include <stdlib.h>

using namespace std;
string napisdata(class OgolneInfo parametry)
{
    string nazwapliku;
    tm *temp;
    temp = localtime(&parametry.data);
    nazwapliku += parametry.nazwa;
    string tempp;
    nazwapliku += '_';
    sprintf((char*)tempp.c_str(), "%d",1900 + temp->tm_year);
    nazwapliku += tempp.c_str();
    nazwapliku += '-';
    sprintf((char*)tempp.c_str(), "%d",temp->tm_mon);
    nazwapliku += tempp.c_str();
    nazwapliku += '-';
    sprintf((char*)tempp.c_str(), "%d",temp->tm_mday);
    nazwapliku += tempp.c_str();
    nazwapliku += '_';
    sprintf((char*)tempp.c_str(), "%d",temp->tm_hour);
    nazwapliku += tempp.c_str();
    nazwapliku += ':';
    sprintf((char*)tempp.c_str(), "%d",temp->tm_min);
    nazwapliku += tempp.c_str();
    nazwapliku += ':';
    sprintf((char*)tempp.c_str(), "%d",temp->tm_sec);
    nazwapliku += tempp.c_str();
    return nazwapliku;
}

int poziom_sygnalu()
{
    int link, level;
    string linia;
    string quality;
    fstream plik;
    plik.open("/proc/net/wireless", ios::in);
    getline(plik, linia);
    getline(plik, linia);
    if(getline(plik, linia))
    {
    quality = linia.substr(15,15);
    cout<<quality<<endl;
    //link=stoi(quality.substr(0,2));
    level=stoi(quality.substr(5,7));
    //cout<<level<<endl;
    }
    else
    level =-256;
    plik.close();
    return level;
}

void zapisipomiar(class OgolneInfo &parametry,int &polozenie,int &licznik, int &stan)
{

    fstream plik;
    plik.open(napisdata(parametry), ios::app);
    plik<<"Protokół:        "<<parametry.protokol<<endl;
    plik<<"Rozmiar pakietu: "<<parametry.rozmiar_pakietu<<endl;
    if(parametry.protokol ==1)
    {
        plik<<"Kodowanie:       "<<parametry.kodowanie<<endl;
    };
    int ostatnio = 0;
    int wyslane = 0;
    int nrpomiaru =0;
    while (stan)
    {
        nrpomiaru ++;
        ostatnio = licznik;
        sleep(1);
        if (ostatnio <= licznik)
        {
            wyslane = licznik - ostatnio;
        }
        else
        {
            wyslane = INT_MAX - ostatnio + licznik - INT_MIN;
        }
        plik << nrpomiaru<< ". "<<"Poziom: "<<poziom_sygnalu()/*<<" polozenie: "<< polozenie*/<<" odebrano: " <<  wyslane  << " pakietow." <<" Przeplywnosc: "<< (double)wyslane /(125000/parametry.rozmiar_pakietu)<< "Mb/s"<< endl;
    }
    plik.close();
}

