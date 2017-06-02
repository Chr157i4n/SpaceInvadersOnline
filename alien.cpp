#include "alien.h"
#include "alienschuss.h"
#include <ctime>
#include <stdlib.h>     /* srand, rand */

alien::alien()
{

    std::srand(std::time(0));
    mHoehe=30;
    mBreite=30;
    active=false;
    mLeben=1;
    mObjektType=1;
}

alien::~alien()
{
    //dtor
}

void alien::bewegenY(int geschwY,int fensterBreite)
{
if (mX+breite>fensterBreite || mX<0) {mY=mY+geschwY;}
}

void alien::bewegenX(int fensterBreite)
{
    mX+=mGeschwX;
    if (mX+breite>fensterBreite || mX<0) {mGeschwX=-1*mGeschwX;}
}

void alien::speedUp()
{
    if (mGeschwX<0)
    {
        mGeschwX--;
    }
    else
    {
        mGeschwX++;
    }
}

void alien::reseten(int x,int y)
{
    mX=x;
    mY=y;
    mGeschwX=1;
    mLeben=1;
    activate();
    mObjektType=1;

}

void alien::schiessen(alienschuss* Alienschuss)
{
     int zufall= std::rand()%1000+1;

        if (zufall>998-mWahrscheinlichkeit)
            {


                for (int i=0;i<100;i++)
                {
                    if (!Alienschuss[i].isActive())
                    {
                     Alienschuss[i].abschiessen(mX+14,mY+30);
                     break;
                    }
                }



            }

}
