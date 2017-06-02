#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "objekt.h"



class explosion : public objekt
{
    public:
        explosion();
        virtual ~explosion();

        int getLaufzeit(){return mLaufzeit;};

        void setLaufzeit(int laufzeit)
        {if (laufzeit>0) mLaufzeit=laufzeit;};

        void explodieren(int x,int Y);
        void ueberpruefen();

    protected:

    private:
        int mLaufzeit;
};

#endif // EXPLOSION_H
