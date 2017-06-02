#ifndef SCHUSS_H
#define SCHUSS_H
#include "objekt.h"

class alien;
class explosion;
class spiel;
class spieler;


class schuss : public objekt
{
    public:
        schuss();
        virtual ~schuss();

        void bewegen(int schussgeschwSpieler);
        bool trefferpruefen(alien ZuPruefenderAlien[],explosion Explosion[],spiel* Spiel,spieler* Spieler);



        void abschiessen(int x,int y)
        {
         if (x>0 && x<500) mX=x;
         if (y>0 && y<500) mY=y;
         activate();
        };


    protected:

    private:

};

#endif // SCHUSS_H
