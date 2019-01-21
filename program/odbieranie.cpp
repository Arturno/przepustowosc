#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include "sprawdzanie.cpp"
#include <fstream>
#define SOL_UDPLITE 136
#define UDPLITE_RECV_CSCOV 11
using namespace std;

void odbieranieUDP(struct sockaddr_in TX, struct sockaddr_in RX, int rozmiar_pakietu, int &licznik, int &stan)
{
    char bufor[rozmiar_pakietu-10];
    char buffer[rozmiar_pakietu];
    char przeniesienie[10];
    const int socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(2);
    }
    socklen_t len = sizeof(RX);
    if (bind(socket_, (struct sockaddr *)&RX, len) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }

    while (stan)
    {
        //if(((spr.dodawanie+1)%100)==spr.sprawdzanie)
        //{
        //     cout<<"errrr"<<endl;
        // }
        if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&RX, &len) < 0)
        {
            perror("recvfrom() ERROR");
            exit(4);
        }
        /*int nrpakietu;
        strncpy(przeniesienie, buffer, 10);
        nrpakietu = atoi(przeniesienie);
        cout<<nrpakietu<<" ";
        strncpy(bufor, &(buffer[10]),sizeof(bufor));
        cout<<bufor<<endl;*/
        licznik++;
    }
    shutdown(socket_, SHUT_RDWR);
}

void odbieranieUDPLite(struct sockaddr_in TX, struct sockaddr_in RX, int rozmiar_pakietu, int kodowanie, int &licznik, int &stan)
{
    char buffer[rozmiar_pakietu] = {};
    const int socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDPLITE);
    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(2);
    }
    setsockopt(socket_, SOL_UDPLITE, UDPLITE_RECV_CSCOV, &kodowanie, sizeof(int));
    socklen_t len = sizeof(RX);
    if (bind(socket_, (struct sockaddr *)&RX, len) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }
    while (stan)
    {
        if (recvfrom(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&RX, &len) < 0)
        {
            perror("recvfrom() ERROR");
            exit(4);
        }
        licznik++;
    }
    shutdown(socket_, SHUT_RDWR);
}

void odbieranieTCP(struct sockaddr_in TX, struct sockaddr_in RX, int rozmiar_pakietu, int &licznik, int &stan)
{
    const int socket_ = socket(AF_INET, SOCK_STREAM, 0);
    char buffer[rozmiar_pakietu] = {};
    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(2);
    }

    socklen_t len = sizeof(TX);
    int opt = 1;
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(socket_, (struct sockaddr *)&RX, len) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }

    if (listen(socket_, 10) < 0)
    {
        perror("listen() ERROR");
        exit(4);
    }

    const int TXSocket = accept(socket_, (struct sockaddr *)&TX, &len);
    if (TXSocket < 0)
    {
        perror("accept() ERROR");
    }
    while (stan)
    {
        if (recv(TXSocket, buffer, sizeof(buffer), 0) <= 0)
        {
            perror("recv() ERROR");
            exit(5);
        }
        licznik++;
    }
    shutdown(TXSocket, SHUT_RDWR);
    shutdown(socket_, SHUT_RDWR);
}
