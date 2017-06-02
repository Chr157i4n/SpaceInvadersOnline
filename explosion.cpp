#include "explosion.h"


explosion::explosion()
{
    mHoehe=40;
    mBreite=40;
}

explosion::~explosion()
{
    //dtor
}

void explosion::explodieren(int x,int y)
{
    mX=x;
    mY=y;
    mLaufzeit=25;
    activate();
}

void explosion::ueberpruefen()
{

    mLaufzeit--;

        if (mLaufzeit<=0)
        {
           deactivate();
        }

}
