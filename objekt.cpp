#include "objekt.h"

objekt::objekt()
{
}

objekt::~objekt()
{
    //dtor
}

void objekt::decleben()
{
       mLeben--;
       if (mLeben<=0)
        deactivate();
}

void objekt::changeType(int type,wxBitmap picture,int leben,int schusswahrscheinlichkeit)
{
    mLeben=leben;
    mObjektType=type;
    mBreite=picture.GetWidth();
    mHoehe=picture.GetHeight();
    mWahrscheinlichkeit=schusswahrscheinlichkeit;
}
