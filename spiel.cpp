#include "spiel.h"
#include "Spieler.h"
#include "alien.h"
#include <wx/textfile.h>
#include "alienschuss.h"
#include "schuss.h"
#include "explosion.h"
#include <direct.h>

spiel::spiel(alien Alien[], alienschuss Alienschuss[], schuss Schuss[],explosion Explosion[],spieler Spieler[])
{
mAlien=Alien;
mSchuss=Schuss;
mExplosion=Explosion;
mAlienschuss=Alienschuss;
mSpieler=Spieler;
}

spiel::~spiel()
{
    //dtor
}

bool spiel::isGameInForeground()
{
    return (fensterImVordergrund==GetForegroundWindow());
}

void spiel::addPunkte()
{
    mSpieler->addPunkte(mSpieler->getPunkte()+(geschwAlien.x+geschwAlien.y+schusswahrscheinlichkeit-lebenPUNKTE-schussgeschwSpieler+schussgeschwAliens));
}


void spiel::werteuebernehmen()
{
            schussgeschwSpieler=schussgeschwSpielerNEU;
            geschwAlien.x=geschwXNEU;
            geschwAlien.y=geschwYNEU;
            schusswahrscheinlichkeit=schusswahrscheinlichkeitNEU;
            anzahlAlienNEU=19;
            spiellaeuft=true;
            lebenPUNKTE=lebenNEU;
}

void spiel::startNewRound(wxBitmap bBoss[],wxBitmap bAlien)
{
    mSpieler->setNameVorschlag();
    mSpieler[0].activate();
    mSpieler->setX(250-16);

    for(int i=0;i<100;i++)
    {
        mAlienschuss[i].deactivate();
    }
    for(int i=0;i<40;i++)
    {
        mAlien[i].deactivate();
    }
    for(int i=0;i<10;i++)
    {
        mSchuss[i].deactivate();
    }


        int spaltealien=0;
        int reihealien=1;
        int a=getAliensProRunde();
        if (a%3==0)
        {
            for (int i=0;i<((a-20)/3);i++)
            {

               if (spaltealien>10)
           {    spaltealien=1;
                reihealien+=3;   }
            mAlien[i].reseten(spaltealien*40,reihealien*40);
            mAlien[i].changeType(2,bBoss[0],5,schusswahrscheinlichkeit*10);
            spaltealien+=3;
            }

        }
        else
        {
            for (int i=0;i<a;i++)
        {
           spaltealien++;

           if (spaltealien>10)
           {    spaltealien=1;
                reihealien++;   }                               ///Aliens hinzufügen

            mAlien[i].reseten(spaltealien*40,reihealien*40);
            mAlien[i].changeType(1,bAlien,1,schusswahrscheinlichkeit);

        }
        }

    writeLog(wxT("started new round"));
}

void spiel::aliensGeschwindigkeitErhoehen(alien Alien[])
{
    int c=0;
    for (int j=0;j<=40;j++)
    if (!Alien[j].isActive())
    {
        c++;
    }

        if (c %5 ==0)
        {
        for (int i=0; i<40; i++)
        {
            Alien[i].speedUp();
        }
        }

}

void spiel::spawnReinigen()
{

            ///Schüsse beim Spawnpunkt entfernen
            for (int f=0; f<100; f++)
            {
                if ((mAlienschuss[f].getX()>(fensterBreite/2-mSpieler[0].getBreite()*4)) && (mAlienschuss[f].getX()<(fensterBreite/2+mSpieler[0].getBreite()*4)) && (mAlienschuss[f].getY()>fensterHoehe/3))
                {

                    mAlienschuss[f].deactivate();
                }
            }
}

void spiel::writeLog(wxString logtext)
{
    wxTextFile logTXT( wxT("log.txt") );
    logTXT.Create("log.txt");             ///erstellt nur, falls nicht vorhanden
    logTXT.Open("log.txt");
    logTXT.AddLine(logtext);
    logTXT.Write();
    logTXT.Close();
}

void spiel::highscore(spieler* Spieler,sf::Ftp* ftp)
{
    mkdir("Highscore");

    response = ftp->download("HighscoreOnline.txt", "Highscore", sf::Ftp::Ascii);
    writeLog(response.getMessage());
    writeLog(wxT("Downloaded Highscore"));

    bool hso=false,hs=false,chanceled=false;
                                   ///erstellt nur, falls nicht vorhanden
    wxTextFile highscoreTXT( wxT("Highscore/Highscore.txt") );

    highscoreTXT.Create("Highscore/Highscore.txt");             ///erstellt nur, falls nicht vorhanden
    highscoreTXT.Open("Highscore/Highscore.txt");




    ///Highscore
    int platz=0;
    wxString highscore;
    wxString tmp;
    tmp = highscoreTXT.GetFirstLine();
    for (int i=0; i<10; i++)
    {
        if (tmp[i] == ' ')
        {
          for (int c=0;c<i;c++)
          {
             highscore=highscore+tmp[c];
          }
          break;
        }
    }


    do {


        if ((Spieler->getPunkte()>wxAtoi(highscore) || highscore=="")    && !hs && Spieler->getPunkte()>0)
            {


            wxTextEntryDialog *dlg = new wxTextEntryDialog((wxFrame *)NULL,wxT("Gib bitte deinen Namen ein"),wxT("Highscore"),Spieler->getNameVorschalg());
                if ( dlg->ShowModal() == wxID_OK )
                {
                    dlg->Destroy();
                    wxString punktstand;
                    punktstand << Spieler->getPunkte();



                        if (Spieler->getPunkte()<100)
                        {
                        punktstand=punktstand+" ";
                        }
                        if (Spieler->getPunkte()<1000)
                        {
                        punktstand=punktstand+" ";
                        }
                        Spieler->setName(dlg->GetValue());
                    punktstand=punktstand+"   "+Spieler->getName();
                    highscoreTXT.InsertLine( punktstand, platz);
                    highscoreTXT.Write();
                    hs=true;


                }
                else {hs=true;chanceled=true;}

            dlg->Destroy();


            break;
            }
        highscore="";
        tmp = highscoreTXT.GetNextLine();
        for (int i=0; i<10; i++)
    {

        if (tmp[i] == ' ')
        {
          for (int c=0;c<i;c++)
          {
             highscore=highscore+tmp[c];
          }
          break;
        }
    }
        platz++;

    } while(!highscoreTXT.Eof() && platz<10);

    if (platz<10 && !hs && Spieler->getPunkte()>0)
    {

            wxTextEntryDialog *dlg = new wxTextEntryDialog((wxFrame *)NULL,wxT("Gib bitte deinen Namen ein"),wxT("Highscore"),Spieler->getNameVorschalg());
                if ( dlg->ShowModal() == wxID_OK )
                {
                    dlg->Destroy();
            wxString punktstand;
            punktstand << Spieler->getPunkte();                   ///Punktzahl ganz am ende
            if (Spieler->getPunkte()<100)
            {
            punktstand=punktstand+" ";
            }
            if (Spieler->getPunkte()<1000)
            {
            punktstand=punktstand+" ";
            }
            Spieler->setName(dlg->GetValue());
             punktstand=punktstand+"   "+Spieler->getName();
            highscoreTXT.InsertLine( punktstand, platz);
            highscoreTXT.Write();
                } else {chanceled=true;}
                dlg->Destroy();



    }



for(int i=10;i<highscoreTXT.GetLineCount();i++)
{
    highscoreTXT.RemoveLine(i);
}


    highscoreTXT.Write();
    highscoreTXT.Close();







    ///Highscore Online

if (!chanceled){
    wxTextFile highscoreTXTOnline( wxT("Highscore/HighscoreOnline.txt") );
    highscoreTXTOnline.Create("Highscore/HighscoreOnline.txt");
    highscoreTXTOnline.Open("Highscore/HighscoreOnline.txt");



    ///Zeilen wieder formatieren





    platz=0;
    highscore="",tmp="";

    tmp = highscoreTXTOnline.GetFirstLine();

    for (int i=0; i<10; i++)
    {
        if (tmp[i] == ' ')
        {
          for (int c=0;c<i;c++)
          {
             highscore=highscore+tmp[c];
          }
          break;
        }
    }


    do {


        if ((Spieler->getPunkte()>wxAtoi(highscore) || highscore=="")    && !hso && Spieler->getPunkte()>0)
            {



                    wxString punktstand;
                    punktstand << Spieler->getPunkte();



                        if (Spieler->getPunkte()<100)
                        {
                        punktstand=punktstand+" ";
                        }
                        if (Spieler->getPunkte()<1000)
                        {
                        punktstand=punktstand+" ";
                        }
                    punktstand=punktstand+"   "+Spieler->getName();
                    highscoreTXTOnline.InsertLine( punktstand, platz);
                    highscoreTXTOnline.Write();
                    hso=true;

                    break;
                }






        highscore="";
        tmp = highscoreTXTOnline.GetNextLine();
        for (int i=0; i<10; i++)
    {

        if (tmp[i] == ' ')
        {
          for (int c=0;c<i;c++)
          {
             highscore=highscore+tmp[c];
          }
          break;
        }
    }
        platz++;

    } while(!highscoreTXTOnline.Eof() && platz<10);


    if (platz<10 && !hso && Spieler->getPunkte()>0)
    {
            wxString punktstand;
            punktstand << Spieler->getPunkte();                   ///Punktzahl ganz am ende
            if (Spieler->getPunkte()<100)
            {
            punktstand=punktstand+" ";
            }
            if (Spieler->getPunkte()<1000)
            {
            punktstand=punktstand+" ";
            }
             punktstand=punktstand+"   "+Spieler->getName();
            highscoreTXTOnline.InsertLine( punktstand, platz);
            highscoreTXTOnline.Write();
                }








for(int i=10;i<highscoreTXTOnline.GetLineCount();i++)
{
    highscoreTXTOnline.RemoveLine(i);
}


    highscoreTXTOnline.Write();
    highscoreTXTOnline.Close();


    response = ftp->upload("Highscore/HighscoreOnline.txt", "", sf::Ftp::Ascii);
    writeLog(response.getMessage());
    writeLog(wxT("Uploaded Highscore"));
}

}

void spiel::einstellungen()
{
    wxTextEntryDialog *dlg = new wxTextEntryDialog((wxFrame *)NULL,wxT("Spielgeschwindigkeit (1-100)"),wxT("Einstellungen"),wxT("85"));
             if ( dlg->ShowModal() == wxID_OK )
                {

                 timerzeit = 100- wxAtoi(dlg->GetValue());
                }
                dlg->Destroy();


        wxTextEntryDialog *dlg1 = new wxTextEntryDialog((wxFrame *)NULL,wxT("Schwierigkeit (1-10)"),wxT("Einstellungen"),wxT("2"));
             if ( dlg1->ShowModal() == wxID_OK )
                {

                 int tmp =  wxAtoi(dlg1->GetValue());
                 if (tmp>0 && tmp<=10)
                 {
                     geschwXNEU=tmp/2;
                     geschwYNEU=tmp*5;
                 }

                }
                dlg1->Destroy();


                wxTextEntryDialog *dlg2 = new wxTextEntryDialog((wxFrame *)NULL,wxT("Schusswahrscheinlichkeit der Aliens (1-10)"),wxT("Einstellungen"),wxT("5"));
             if ( dlg2->ShowModal() == wxID_OK )
                {

                 int tmp =  wxAtoi(dlg2->GetValue());
                 if (tmp>0 && tmp<=10)
                 {
                    schusswahrscheinlichkeitNEU=tmp;
                 }

                }
                dlg2->Destroy();


                 wxTextEntryDialog *dlg3 = new wxTextEntryDialog((wxFrame *)NULL,wxT("Anzahl deiner Leben (1-10)"),wxT("Einstellungen"),wxT("3"));
             if ( dlg3->ShowModal() == wxID_OK )
                {

                 int tmp =  wxAtoi(dlg3->GetValue());
                 if (tmp>0 && tmp<=10)
                 {
                    lebenNEU=tmp;
                 }

                }
                dlg3->Destroy();

                 wxTextEntryDialog *dlg4 = new wxTextEntryDialog((wxFrame *)NULL,wxT("Geschwindigkeit der AlienSchuesse (1-10)"),wxT("Einstellungen"),wxT("2"));
             if ( dlg4->ShowModal() == wxID_OK )
                {

                 int tmp =  wxAtoi(dlg4->GetValue());
                 if (tmp>0 && tmp<=10)
                 {
                    schussgeschwAliens=tmp;
                 }

                }
                dlg4->Destroy();

                  wxTextEntryDialog *dlg5 = new wxTextEntryDialog((wxFrame *)NULL,wxT("Geschwindigkeit der Spielerschuesse (1-10)"),wxT("Einstellungen"),wxT("3"));
             if ( dlg5->ShowModal() == wxID_OK )
                {

                 int tmp =  wxAtoi(dlg5->GetValue());
                 if (tmp>0 && tmp<=10)
                 {
                    schussgeschwSpielerNEU=tmp;
                 }

                }
                dlg4->Destroy();



}

void spiel::highscoreZeigen(sf::Ftp* ftp)
{
        _mkdir("Highscore");
        response = ftp->download("HighscoreOnline.txt", "Highscore", sf::Ftp::Ascii);
        writeLog(response.getMessage());
        writeLog(wxT("Downloaded Highscore"));


        wxString tmp;
        //ftp.download("HighscoreOnline.txt", "Highscore", sf::Ftp::Ascii);

        wxTextFile highscoreTXT( wxT("Highscore/Highscore.txt") );
        highscoreTXT.Create("Highscore/Highscore.txt");
        highscoreTXT.Open();

        wxTextFile highscoreTXTOnline( wxT("Highscore/HighscoreOnline.txt") );
        highscoreTXTOnline.Create("Highscore/HighscoreOnline.txt");
        highscoreTXTOnline.Open();

        tmp="Lokaler Highscore: \n";
        tmp=tmp+highscoreTXT.GetFirstLine();
        while (!highscoreTXT.Eof())
        {
          tmp=tmp+" \n "+highscoreTXT.GetNextLine();
        }
        tmp=tmp+"\n";

        tmp=tmp+"Globaler Highscore: \n";
        tmp=tmp+highscoreTXTOnline.GetFirstLine();
        while (!highscoreTXTOnline.Eof())
        {
          tmp=tmp+" \n "+highscoreTXTOnline.GetNextLine();
        }


        wxMessageBox( tmp,"Highscore" ,wxICON_INFORMATION);


        highscoreTXT.Close();
}

