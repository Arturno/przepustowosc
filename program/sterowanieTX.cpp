#include <iostream>

using namespace std;
class SterowanieTX
{
  public:
    int stan;
    int licznik;
    int opoznienie;
    int do_wyslania;
    int grupa_pakietow;
    int rozmiar_pakietu;

    SterowanieTX(int, int);
    void zmiana_przeplywnosci(int przeplywnosc);
    void koniec();
    void wypisanie();
};

SterowanieTX::SterowanieTX(int przeplywnosc, int rozmiar)
{
    stan = 1;
    licznik = 0;
    rozmiar_pakietu = rozmiar;
    if (przeplywnosc == 0)
    {
        przeplywnosc = 2000;
    }

    do_wyslania = (przeplywnosc * 1000000) / (rozmiar_pakietu * 8);
    if ((do_wyslania * 0.01) < 1)
    {
        grupa_pakietow = 1;
    }
    else
    {
        grupa_pakietow = do_wyslania * 0.001;
        opoznienie = (rozmiar_pakietu * grupa_pakietow * 1000) / (przeplywnosc * 250);
    }
}

void SterowanieTX::zmiana_przeplywnosci(int przeplywnosc)
{
    do_wyslania = (przeplywnosc * 1000000) / (rozmiar_pakietu * 8);
    if ((do_wyslania * 0.001) < 1)
    {
        grupa_pakietow = 1;
    }
    else
        grupa_pakietow = do_wyslania * 0.001;
    opoznienie = (rozmiar_pakietu * grupa_pakietow * 1000) / (przeplywnosc * 250);
}

void SterowanieTX::wypisanie()
{
    cout << "Liczba pakietow do wyslania w 1 sekundzie: ....  " << do_wyslania << endl;
    cout << "Liczba pakietow w grupie: .....................  " << grupa_pakietow << endl;
    cout << "Rozmiar pakietu: ..............................  " << rozmiar_pakietu << endl;
    cout << "Opoznienie: ...................................  " << 1000000 / do_wyslania << endl;
}

void SterowanieTX::koniec()
{
    stan = 0;
}