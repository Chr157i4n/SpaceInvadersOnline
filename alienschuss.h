#ifndef ALIENSCHUSS_H
#define ALIENSCHUSS_H
#include "objekt.h"

class spieler;
class spiel;

class alienschuss : public objekt
{
    public:
        alienschuss();
        virtual ~alienschuss();

        void bewegen(int schussgeschwAliens);

         void trefferpruefen(spieler* ZuPruefenderSpieler,spiel* Spiel,bool* clientGetroffen);



        void abschiessen(int x,int y)
        {
         if (x>0 && x<500) mX=x;
         if (y>0 && y<500) mY=y;
         activate();
        };

    protected:

    private:

};

#endif // ALIENSCHUSS_H
