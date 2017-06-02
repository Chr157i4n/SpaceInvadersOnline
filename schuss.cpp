#include "schuss.h"
#include "alien.h"
#include "explosion.h"
#include "spiel.h"
#include "spieler.h"

schuss::schuss()
{
    mHoehe=9;
    mBreite=4;
}

schuss::~schuss()
{
    //dtor
}

void schuss::bewegen(int schussgeschwSpieler)
{
    mY=mY-schussgeschwSpieler;
}

bool schuss::trefferpruefen(alien Alien[],explosion Explosion[],spiel* Spiel,spieler* Spieler)
{
for (int c=0; c<40; c++)
        {
            if(Alien[c].isActive())
            {
if ( (mX>Alien[c].getX()) && (mX+mBreite<Alien[c].getX()+Alien[c].getBreite()) && (mY>Alien[c].getY()) && (mY+mHoehe<Alien[c].getY()+Alien[c].getHoehe()) )
            {               ///Treffer erkennen

                for (int i=0;i<10;i++)
                {
                    if (!Explosion[i].isActive())
                    {Explosion[i].explodieren(Alien[c].getX(),Alien[c].getY());
                    break;}
                }


                Alien[c].decleben();
                deactivate();
                Spiel->aliensGeschwindigkeitErhoehen(Alien);

                if (Spiel->isGameRunning())
                {Spiel->addPunkte();}               ///Punkte hinzufügen

            return true;
            }
            }
        }
    return false;
}
