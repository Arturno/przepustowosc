#include <iostream>
#include <cstdio>
#include <stdlib.h>    // exit()
#include <string.h>    // memset()
#include <arpa/inet.h> // inet_pton()
#include <sys/socket.h>
#include "wysylanie.cpp"
#include "sterowanieTX.cpp"
#include "ControlTX.cpp"
#include "Adaptacja.cpp"
#include <thread>

using namespace std;

//Częśc służąca nadawaniu pakietów

void pomiar(int &stan, int &licznik, int &grupa_pakietow, int rozmiar)
//Funkcja dodatkowa mierząca liczbę wysłanych pakietów w ostatniej sekundzie
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
        cout << "Wyslano: " << wyslane * grupa_pakietow << " pakietow."
             << " Przeplywnosc: " << ((double)wyslane * (double)grupa_pakietow) / (125000 / rozmiar) << "Mb/s" << endl;
    }
}

void transmitter()
//Główna funkcja części nadawczej
{
    //Tworzenie struktur zawierających adresy strony nadawczej i odbiorczej
    struct sockaddr_in TX_TCP = {
        .sin_family = AF_INET,
        .sin_port = htons(8888)};
    struct sockaddr_in TX = {
        .sin_family = AF_INET,
        .sin_port = htons(8889)};
    struct sockaddr_in RX_TCP = {};
    struct sockaddr_in RX = {};

    cout << "TRYB TRANSMITER" << endl;
    //Oczekiwanie na połączenie przychodzące
    cout << "Oczekiwanie na przyłączenie RX" << endl;
    //Zestawianie połączenia TCP w celu bezbłędnej wymiany parametrów transmisji
    if (inet_pton(AF_INET, "127.0.0.1", &TX_TCP.sin_addr) <= 0)
    {
        perror("inet_pton() ERROR");
        exit(1);
    }
    if (inet_pton(AF_INET, "127.0.0.1", &TX.sin_addr) <= 0)
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

    socklen_t len = sizeof(TX_TCP);
    int opt = 1;
    setsockopt(socketTCP_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(socketTCP_, (struct sockaddr *)&TX_TCP, len) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }

    if (listen(socketTCP_, 10) < 0)
    {
        perror("listen() ERROR");
        exit(4);
    }
    char buffer[250];

    const int RXSocket = accept(socketTCP_, (struct sockaddr *)&RX_TCP, &len);
    if (RXSocket < 0)
    {
        perror("accept() ERROR");
    }

    memset(buffer, 0, sizeof(buffer));
    if (recv(RXSocket, buffer, sizeof(buffer), 0) <= 0)
    {
        perror("recv() ERROR");
        exit(5);
    }

    RX = RX_TCP;
    RX.sin_port = htons(9999);
    char buffer_ip[128] = {};
    cout << "IP RX:  ..........  " << inet_ntop(AF_INET, &RX.sin_addr, buffer_ip, sizeof(buffer_ip)) << endl;
    cout << "port:  ...........  " << ntohs(RX.sin_port) << endl;
    
    //Utworzenie struktury zawierającej paraketry transmisji
    class OgolneInfo parametry(buffer);
    parametry.wypisanie();
    class SterowanieTX ster(parametry.przeplywnosc, parametry.rozmiar_pakietu);
    ster.wypisanie();
    cout<<"#############################################################################"<<endl;
    cout<<"Sterowanie programem"<<endl;
    cout<<"M xxx - zmiana przepływności na wartość xxx Mb/s"<<endl;
    cout<<"E - zakonczenie pomiaru"<<endl;

        
        thread sterowanieProgramem(ControlTX, ref(ster.stan), ref(ster), RXSocket);
        thread adapt(adaptacja, ref(ster.licznik), ref(ster.do_wyslania), ref(ster.grupa_pakietow), ref(ster.opoznienie), ref(ster.stan), parametry.czas_adaptacji);
        //thread pomiaaar(pomiar, ref(ster.stan), ref(ster.licznik), ref(ster.grupa_pakietow), ster.rozmiar_pakietu);

        switch (parametry.protokol)
        {
        case 0:
        {
            //thread wysylanie(wysylanieUDP, TX, RX, ref(parametry.rozmiar_pakietu), ref(ster.opoznienie), ref(ster.grupa_pakietow), ref(ster.licznik), ref(ster.stan));
            wysylanieUDP(TX, RX, parametry.rozmiar_pakietu, ster.opoznienie, ster.grupa_pakietow, ster.licznik, ster.stan);
            break;
        }
        case 1:
        {
            //thread wysylanie(wysylanieUDPLite, TX, RX, ref(parametry.rozmiar_pakietu), ref(ster.opoznienie), ref(ster.grupa_pakietow),parametry.kodowanie, ref(ster.licznik), ref(ster.stan));
            wysylanieUDPLite(TX, RX, parametry.rozmiar_pakietu, ster.opoznienie, ster.grupa_pakietow, parametry.kodowanie, ster.licznik, ster.stan);
            break;
        }
        case 2:
        {
            //thread wysylanie(wysylanieTCP, TX, RX, ref(parametry.rozmiar_pakietu), ref(ster.opoznienie), ref(ster.grupa_pakietow), ref(ster.licznik), ref(ster.stan));
            wysylanieTCP(TX, RX, parametry.rozmiar_pakietu, ster.opoznienie, ster.grupa_pakietow, ster.licznik, ster.stan);
            break;
        }
        }
        sterowanieProgramem.join();
        adapt.join();
        //pomiaaar.join();
    
    //wysylanie.join()
    //zamykanie gniazd
   
    shutdown(RXSocket, SHUT_RDWR);
    shutdown(socketTCP_, SHUT_RDWR);
}
