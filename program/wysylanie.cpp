#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#define SOL_UDPLITE 136
#define UDPLITE_SEND_CSCOV 10

using namespace std;

void wysylanieUDP(struct sockaddr_in TX, struct sockaddr_in RX, int rozmiar_pakietu, int &opoznienie, int &grupa_pakietow, int &licznik, int &stan)
{
    char buffer[rozmiar_pakietu + 1] = {};
    srand(50);
    unsigned int wyslane = 0;
    for (int i = 10; i < rozmiar_pakietu - 1; i++)
    {
        buffer[i] = ((rand() % ('z' - 'a')) + 'a');
    }
    buffer[rozmiar_pakietu - 1] = '\0';
    const int socket_ = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(2);
    }
    if (bind(socket_, (struct sockaddr *)&TX, sizeof(TX)) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }
    while (stan)
    {
        for (int i = 0; i < grupa_pakietow; i++)
        {
            sprintf(buffer, "%d", wyslane);
            if (sendto(socket_, buffer, sizeof(buffer), 0, (struct sockaddr *)&RX, sizeof(RX)) < 0)
            {
                perror("sendto() ERROR");
                exit(5);
            }
            wyslane++;
        }
        licznik++;
        usleep(opoznienie);
        //cout << "Wsłano UDP" << endl;
    }
    usleep(100000);
    if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)&RX, sizeof(RX)) < 0)
    {
        perror("sendto() ERROR");
        exit(5);
    };

    shutdown(socket_, SHUT_RDWR);
}

void wysylanieUDPLite(struct sockaddr_in TX, struct sockaddr_in RX, int rozmiar_pakietu, int &opoznienie, int &grupa_pakietow, int kodowanie, int &licznik, int &stan)
{
    char buffer[rozmiar_pakietu] = {};
    srand(50);
    unsigned int wyslane = 0;
    for (int i = 10; i < rozmiar_pakietu; i++)
    {
        buffer[i] = ((rand() % ('z' - 'a')) + 'a');
    }
    const int socket_ = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDPLITE);
    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(2);
    }
    setsockopt(socket_, SOL_UDPLITE, UDPLITE_SEND_CSCOV, &kodowanie, sizeof(int));
    if (bind(socket_, (struct sockaddr *)&TX, sizeof(TX)) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }
    while (stan)
    {
        for (int i = 0; i < grupa_pakietow; i++)
        {
            sprintf(buffer, "%d", wyslane);
            if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)&RX, sizeof(RX)) < 0)
            {
                perror("sendto() ERROR");
                exit(5);
            }
            wyslane++;
        }
        licznik++;
        usleep(opoznienie);
    }
    usleep(100000);
    if (sendto(socket_, buffer, strlen(buffer), 0, (struct sockaddr *)&RX, sizeof(RX)) < 0)
    {
        perror("sendto() ERROR");
        exit(5);
    };
    shutdown(socket_, SHUT_RDWR);
}
void wysylanieTCP(struct sockaddr_in TX, struct sockaddr_in RX, int rozmiar_pakietu, int &opoznienie, int &grupa_pakietow, int &licznik, int &stan)
{
    char buffer[rozmiar_pakietu] = {};
    srand(50);
    unsigned int wyslane = 0;
    for (int i = 10; i < rozmiar_pakietu; i++)
    {
        buffer[i] = ((rand() % ('z' - 'a')) + 'a');
    }
    const int socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_ < 0)
    {
        perror("socket() ERROR");
        exit(2);
    }
    int opt = 1;
    setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (bind(socket_, (struct sockaddr *)&TX, sizeof(TX)) < 0)
    {
        perror("bind() ERROR");
        exit(3);
    }
    connect(socket_, (struct sockaddr *)&RX, sizeof(RX));
    while (stan)
    {
        for (int i = 0; i <= grupa_pakietow; i++)
        {
            sprintf(buffer, "%d", wyslane);
            if (send(socket_, buffer, sizeof(buffer), 0) <= 0)
            {
                perror("send() ERROR");
                exit(6);
            }
            wyslane++;
        }
        licznik++;
        usleep(opoznienie);
    }
    shutdown(socket_, SHUT_RDWR);
}
