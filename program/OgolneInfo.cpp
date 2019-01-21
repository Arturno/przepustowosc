#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ctime>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iomanip>

using namespace std;

class OgolneInfo
///klasa przechowująca podstawowe informacje o warunkach pomiaru, obiekty tej klasy są tworzone po obu stronach (odbiorczej i nadawczej)
{
  public:
    char nazwa[100]; //nazwa pomiaru wysbrana przez użytkownika
    time_t data;     //data rozpoczęcia testu

    int rozmiar_pakietu;
    int protokol;
    int kodowanie;
    int przeplywnosc;
    int czas_adaptacji;

    OgolneInfo();
    OgolneInfo(char[]);
    void wypisanie();
    void getData();
    void tochar(char[]);
};
//funkcja do sprawdzania danych wprowadzanych przez urzytkownika
int getNumber(int min, int max)
{
    int temp;
    string cl;
    while (1)
    {
        cin >> temp;
        if (!cin)
        {
            cin.clear(); // czyścimy flagi błędu strumienia
            cin.sync();
            cout << "Podana wartość nie jest liczbą." << endl;
            getline(cin, cl);
            continue;
        }
        if (temp > max || temp < min)
        {
            cout << "Podana wartość jesc spoza zakresu [" << min << "," << max << "]" << endl;
            continue;
        }
        break;
    }
    return temp;
}

//FUNKCJE DO TWORZENIA STRUKTUR
OgolneInfo::OgolneInfo()
//tworzenie strunktury po stronie RX za pomocą  podawania wartości
{
    cout << "Podaj nazwę testu:" << endl;
    cin >> nazwa;
    cout << "Podaj wymaganą przepływność w Mb/s:" << endl;
    przeplywnosc = getNumber(0, 1000);
    cout << "Podaj rozmiar ramki w B:" << endl;
    rozmiar_pakietu = getNumber(1, 10000);
    if (przeplywnosc != 0)
    {
        cout << "Podaj protokół (UDP-0, UDPLite-1, TCP-2):" << endl;
        protokol = getNumber(0, 2);
    }
    else
    {
        cout << "Podaj protokół (UDP-0, UDPLite-1):" << endl;
        protokol = getNumber(0, 1);
    }

    if (protokol == 1)
    {
        cout << "Podaj poziom kodowania w %:" << endl;
        kodowanie = getNumber(0, 100);
    }
    else
        kodowanie = 100;
    if (przeplywnosc != 0)
    {
        cout << "Podaj czas adaptacji przepływności w ms:" << endl;
        czas_adaptacji = getNumber(0, 1000);
    }
    else
        czas_adaptacji = 10;
    data = time(NULL);
}
OgolneInfo::OgolneInfo(char tablica[])
{
    int przesuniecie = 0;
    char przeniesienie[4];
    strncpy(nazwa, tablica, sizeof(nazwa));
    przesuniecie += sizeof(nazwa);
    strncpy(przeniesienie, &tablica[przesuniecie], sizeof(przeniesienie));
    przeplywnosc = atoi(przeniesienie);
    przesuniecie += sizeof(przeniesienie);
    strncpy(przeniesienie, &tablica[przesuniecie], sizeof(przeniesienie));
    rozmiar_pakietu = atoi(przeniesienie);
    przesuniecie += sizeof(przeniesienie);
    strncpy(przeniesienie, &tablica[przesuniecie], sizeof(przeniesienie));
    protokol = atoi(przeniesienie);
    przesuniecie += sizeof(przeniesienie);
    strncpy(przeniesienie, &tablica[przesuniecie], sizeof(przeniesienie));
    kodowanie = atoi(przeniesienie);
    przesuniecie += sizeof(przeniesienie);
    strncpy(przeniesienie, &tablica[przesuniecie], sizeof(przeniesienie));
    czas_adaptacji = atoi(przeniesienie);
    data = time(NULL);
}

void OgolneInfo::tochar(char tablica[])
{
    int przesuniecie = 0;
    char przeniesienie[4];
    strncpy(tablica, nazwa, sizeof(nazwa));
    przesuniecie += sizeof(nazwa);
    sprintf(przeniesienie, "%d", przeplywnosc);
    strncpy(&tablica[przesuniecie], przeniesienie, sizeof(przeniesienie));
    przesuniecie += sizeof(przeniesienie);
    sprintf(przeniesienie, "%d", rozmiar_pakietu);
    strncpy(&tablica[przesuniecie], przeniesienie, sizeof(przeniesienie));
    przesuniecie += sizeof(przeniesienie);
    sprintf(przeniesienie, "%d", protokol);
    strncpy(&tablica[przesuniecie], przeniesienie, sizeof(przeniesienie));
    przesuniecie += sizeof(przeniesienie);
    sprintf(przeniesienie, "%d", kodowanie);
    strncpy(&tablica[przesuniecie], przeniesienie, sizeof(przeniesienie));
    przesuniecie += sizeof(przeniesienie);
    sprintf(przeniesienie, "%d", czas_adaptacji);
    strncpy(&tablica[przesuniecie], przeniesienie, sizeof(przeniesienie));
}

//FUNKCJE DO WYPISYWANIA

void OgolneInfo::getData()
///wypisanie daty w formacie yyyy-mm-dd hh-mm-ss
{
    data = time(NULL);
    tm *temp;
    temp = localtime(&data);
    cout << 1900 + temp->tm_year << "-" << setw(2) << setfill('0') << temp->tm_mon << "-" << setw(2) << setfill('0') << temp->tm_mday << " ";
    cout << setw(2) << setfill('0') << temp->tm_hour << ":" << setw(2) << setfill('0') << temp->tm_min << ":" << setw(2) << setfill('0') << temp->tm_sec << endl;
};
/*
void OgolneInfo::getAddress()
//wypisanie adresów i portów TX i RX
{
    char buffer_ip[128] = {};
    cout << "IP TX:  .............................  " << inet_ntop(AF_INET, &TX.sin_addr, buffer_ip, sizeof(buffer_ip)) << endl;
    cout << "port:  ..............................  " << ntohs(TX.sin_port) << endl;
    cout << "IP RX:  .............................  " << inet_ntop(AF_INET, &RX.sin_addr, buffer_ip, sizeof(buffer_ip)) << endl;
    cout << "port:  ..............................  " << ntohs(RX.sin_port) << endl;
}
*/
void OgolneInfo::wypisanie()
{
    cout << endl;
    cout << "#############################################################################" << endl;
    cout << "Nazwa testu:  .................................  " << nazwa << endl;
    cout << "Data wykonania testu:  ........................  ";
    getData();
    cout << "Wymagana przepływność:  .......................  ";
    if (przeplywnosc == 0)
        cout << "MAX "
             << "Mb/s" << endl;
    else
        cout << przeplywnosc << "Mb/s" << endl;
    cout << "Rozmiar ramki:  ...............................  " << rozmiar_pakietu << " B" << endl;
    cout << "Protokół:  ....................................  ";
    switch (protokol)
    {
    case 2:
        cout << "TCP" << endl;
        break;
    case 1:
        cout << "UDP-Lite" << endl;
        cout << "Poziom kodowania:  ............................  " << kodowanie << "%" << endl;
        break;
    case 0:
        cout << "UDP" << endl;
        break;
    }
    if (przeplywnosc != 0)
        cout << "Czas adaptacji przepustowości w ms: ...........  " << czas_adaptacji << "ms" << endl;
}
