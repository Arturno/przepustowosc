#include <iostream>


using namespace std;
class SterowanieRX
{
    public:
    int stan;
    int licznik;
    int polozenie;
    double stopa_bledow;


    SterowanieRX(int );
};


SterowanieRX::SterowanieRX(int pol)
{
    stan = 1;
    licznik =0;
    polozenie = pol;
    stopa_bledow =0;
}