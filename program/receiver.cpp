#include <iostream>
#include <cstdio>
#include <stdlib.h>    // exit()
#include <string.h>    // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include <thread>

#include "ControlRX.cpp"
#include "sterowanieRX.cpp"
#include "odbieranie.cpp"
#include "zapis.cpp"

//Część slużąca odbieraniu i analizowaniu pakietów

void pomiar2(int &stan, int &licznik, int rozmiar)
{
    int ostatnio = 0;
    int wyslane = 0;
    while (stan)
    {
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
        cout << "Wyslano: " <<  wyslane  << " pakietow." <<" Przeplywnosc: "<< (double)wyslane /(125000/rozmiar)<< "Mb/s"<< endl;
    }
}
void receiver()
//Główna funkcja służąca odbieraniu
{
    //Tworzenie struktur przechowujących adresy
    struct sockaddr_in TX_TCP = {
        .sin_family = AF_INET,
        .sin_port = htons(8888)};
    struct sockaddr_in TX = {
        .sin_family = AF_INET,
        .sin_port = htons(8889)};
    struct sockaddr_in RX_TCP = {
        .sin_family = AF_INET,
        .sin_port = htons(9998)};
    struct sockaddr_in RX = {
        .sin_family = AF_INET,
        .sin_port = htons(9999)};
    cout << "TRYB RECEIVER" <<endl;

    if (inet_pton(AF_INET, "127.0.0.1", &TX_TCP.sin_addr) <= 0)
    {
        perror("inet_pton() ERROR");
        exit(1);
    }

    if (inet_pton(AF_INET, "127.0.0.1", &RX_TCP.sin_addr) <= 0)
    {
        perror("inet_pton() ERROR");
        exit(1);
    }
    if (inet_pton(AF_INET, "127.0.0.1", &TX.sin_addr) <= 0)
    {
        perror("inet_pton() ERROR");
        exit(1);
    }
    if (inet_pton(AF_INET, "127.0.0.1", &RX.sin_addr) <= 0)
    {
        perror("inet_pton() ERROR");
        exit(1);
    }
    const int socketTCP_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socketTCP_ < 0)
    {
        perror("socket() ERROR");
        exit(2);
    }
    socklen_t len = sizeof(RX_TCP);
    int opt = 1;
    setsockopt(socketTCP_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(socketTCP_, (struct sockaddr *)&RX_TCP, len) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }

    char buffer[250];
    memset(buffer, 0, sizeof(buffer));
    //pobranie parametrów transmisji od użytkownika 
    class OgolneInfo parametry;
    parametry.tochar(buffer);
    parametry.wypisanie();
    //połączenie ze stroną nadawczą
    connect(socketTCP_, (struct sockaddr *)&TX_TCP, sizeof(TX_TCP));
    //wysłanie parametrów do strony nadawczej
    if (send(socketTCP_, buffer, sizeof(buffer), 0) <= 0)
    {
        perror("send() ERROR");
        exit(6);
    }
    //
    int pol=50;
    class SterowanieRX sterRX(pol);

    thread sterowanieProgramem(ControlRX, ref(sterRX.stan), ref(parametry.przeplywnosc), socketTCP_);
    thread pom(zapisipomiar, ref(parametry),ref(sterRX.polozenie), ref(sterRX.licznik), ref(sterRX.stan));
    //W zależnosci od wybranego protokołu, uruchomienie odpowiedniej funkcji odpowiedzialnej za odbiór pakietów
    switch (parametry.protokol)
    {
    case 0:
    {
        odbieranieUDP(TX, RX, parametry.rozmiar_pakietu, sterRX.licznik, sterRX.stan);
        break;
    }
    case 1:
    {
        odbieranieUDPLite(TX, RX, parametry.rozmiar_pakietu, parametry.kodowanie,sterRX.licznik, sterRX.stan);
        break;
    }
    case 2:
    {
        odbieranieTCP(TX, RX, parametry.rozmiar_pakietu, sterRX.licznik, sterRX.stan);
        break;
    }
    }
    

    pom.join();
    sterowanieProgramem.join();

    shutdown(socketTCP_, SHUT_RDWR);

}
