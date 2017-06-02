#ifndef ALIEN_H
#define ALIEN_H
#include "objekt.h"

class alienschuss;

class alien : public objekt
{
    public:
        alien();
        virtual ~alien();

        void bewegenY(int geschwY,int fensterBreite);
        void bewegenX(int fensterBreite);
        void schiessen(alienschuss* NaechsterSchuss);
        void reseten(int x,int y);

        void speedUp();



    protected:

    private:
        int mSchusswahrscheinlichkeit=1;
        int mGeschwX=2;
        int breite=40;

};

#endif // ALIEN_H
