#include "spieler.h"
#include "schuss.h"
#include <wx/textfile.h>
#include <direct.h>
#include <wx/fileconf.h>

spieler::spieler()
{
    mX=250-16;        //Anfangspositionen
    mY=350;
    mHoehe=45;
    mBreite=28;
    mLeben=-100;
setNameVorschlag();

}

spieler::~spieler()
{
    //dtor
}

void spieler::setNameVorschlag()
{
   wxFileConfig* configFile = new wxFileConfig(wxT(""),wxT(""),wxT("config.ini"),wxT(""),wxCONFIG_USE_RELATIVE_PATH);
    configFile->Read("Name",&mNameVorschlag);
}

void spieler::schiessenerlauben()
{
    schusszaehler++;

    if (schusszaehler>50)
    {
    darfschiessen=true;
    schusszaehler=0;
    }
}

void spieler::schiessen(schuss Schuss[])
{

for (int i=0; i<10;i++)
{
    if (!Schuss[i].isActive())
    {
    Schuss[i].abschiessen(mX+12,mY-8);      ///Schuss positionieren
    darfschiessen=false;                        ///Beides dafür, dass man nicht durchgehend schießen kann.
    schusszaehler=0;
    break;
    }
}
}

void spieler::setLeben(int neueLeben)
{

    if (neueLeben>=0 && neueLeben<=10)
        {mLeben=neueLeben;}


}
