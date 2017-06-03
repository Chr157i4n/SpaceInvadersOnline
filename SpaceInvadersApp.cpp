#include <wx/dcbuffer.h>
#include <wx/textfile.h>
#include <thread>
#include <wx/sound.h>
#include <wx/wx.h>
#include <wx/timer.h>
#include <direct.h>
#include <wx/fileconf.h>

#include "alien.h"
#include "explosion.h"
#include "alienschuss.h"
#include "schuss.h"
#include "spieler.h"
#include "spiel.h"
#include "objekt.h"

#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>


class BasicDrawPane;

class RenderTimer : public wxTimer
{
BasicDrawPane* pane;
public:
    RenderTimer(BasicDrawPane* pane);
    void Notify();
    void start(int timerzeit);
    void stop();
};

RenderTimer::RenderTimer(BasicDrawPane* pane) : wxTimer()
{
   RenderTimer::pane = pane;
}
void RenderTimer::start(int timerzeit)
{
    wxTimer::Start(timerzeit);
}

void RenderTimer::stop()
{
    wxTimer::Stop();
}
RenderTimer* timer;


wxBitmap bHintergrund,bRaumschiff,bSchuss,bAlienschuss,bAlien,bLeben,bExplosion,bBoss[5];
sf::SoundBuffer bufferexplosion,bufferschuss;
sf::Sound Soundexplosion,Soundschuss;
sf::Music Hintergrundmusik;
bool mute=false;
sf::Ftp ftp;
sf::UdpSocket socket1;
bool clientSchuss=false,clientGetroffen;


explosion Explosion[10];
alienschuss Alienschuss[100];
alien Alien[40];
schuss Schuss[10];
spieler Spieler[4];
spiel Spiel(Alien,Alienschuss,Schuss,Explosion,Spieler);

void tastatureingaben()
{
    if (Spiel.isGameInForeground() && Spiel.isGameRunning() && Spieler[0].getLeben()>0)
    {

    if((wxGetKeyState((wxKeyCode)'a') || wxGetKeyState((wxKeyCode)'A') || wxGetKeyState(WXK_LEFT)) && (Spieler[0].getX()-5>=0) )
    {
        Spieler[0].bewegen(-2);

    }

    if ((wxGetKeyState((wxKeyCode)'d') || wxGetKeyState((wxKeyCode)'D') || wxGetKeyState(WXK_RIGHT)) && (Spieler[0].getX()+48<=Spiel.getBreite()))
    {
        Spieler[0].bewegen(+2);
    }

    if ((wxGetKeyState((wxKeyCode)' ') || wxGetKeyState((wxKeyCode)'w') || wxGetKeyState((wxKeyCode)'W') || wxGetKeyState(WXK_UP)) && (Spieler[0].ShootingAllowed()))
    {


        if (!mute) Soundschuss.play();

        if (Spiel.getGameMode()!=spiel::client)
        {
            Spieler[0].schiessen(Schuss);
        }


        if (Spiel.getGameMode()==spiel::client)
        {
            clientSchuss=true;
        }
        Spieler[0].pauseShooting();




    }

    }
}

void schussloeschen()
{
  for (int i=0;i<10;i++)
        {
          if (Schuss[i].getY()<0 && Schuss[i].isActive())       ///Schuss oben raus
          {
               Schuss[i].deactivate();
          }
        }

       for (int i=0;i<100;i++)
        {
          if (Alienschuss[i].getY()>Spiel.getHoehe()-40 && Alienschuss[i].isActive())       ///Alienschuss unten raus
          {
                Alienschuss[i].deactivate();
          }
        }


}

void alienBewegen()
{
    for (int i=0; i<40;i++)
    {
        Alien[i].bewegenY(Spiel.getAlienGeschw().y,500);       ///bewegen
        Alien[i].bewegenX(Spiel.getBreite());
    }
}

void trefferregistrieren()
{
    ///Spieler Schüsse -> Aliens
    for (int i=0; i<10; i++)
    {
         if (Schuss[i].isActive())
         {

            if (Schuss[i].trefferpruefen(Alien,Explosion,&Spiel,Spieler))   ///Eigentliche Trefferanalyse
            {
                if (!mute) Soundexplosion.play();
            }

         }
    }

    ///Alienschüsse -> Spieler
    for (int i=0; i<100; i++)
    {
        if (Alienschuss[i].isActive())
         {
            Alienschuss[i].trefferpruefen(Spieler,&Spiel,&clientGetroffen);     ///Eigentliche Trefferanalyse
         }
    }


}

void alienschiessen()
{
    for (int i=0;i<40;i++)
    {
        if (Alien[i].isActive())   ///Schiessen
        {
           Alien[i].schiessen(Alienschuss);

        }

    }
}

int lastAlien()
{
   for (int i=39;i>=0;i--)
   {
       if (Alien[i].isActive())
        {return i;}
   }
   return -1;
}

void endeerkennug()
{
    if (Spieler[0].getLeben()<=0 && (Spieler[1].getLeben()<=0) && Spiel.isGameRunning())
    {
        Spiel.writeLog("Game lost, because the Player has no life left");
        Spiel.stopGame();
        Spiel.highscore(&Spieler[0],&ftp);
        Spieler[0].setPunkte(0);

    }

    if (Alien[lastAlien()].getY()>=360 && Spiel.isGameRunning())
    {

        Spiel.writeLog("Game lost, because the Aliens reached the Earth");
        Spiel.stopGame();
        Spieler[0].setLeben(0);
        Spiel.highscore(&Spieler[0],&ftp);
        Spieler[0].setPunkte(0);

    }


    if (lastAlien()==-1)
    {
       /* wxString meldung="LastAlien Index";
        meldung << lastAlien();
        meldung+="     ";
        meldung << Alien[0].getLeben();
        meldung << Alien[0].isActive();
        meldung+="     ";
        meldung << Alien[1].getLeben();
        meldung << Alien[1].isActive();
        timer->stop();
        wxMessageBox(meldung);
        timer->start(Spiel.getSpielgeschwindigkeit());*/

        Spiel.writeLog("one Wave finished");
        Spiel.startNewRound(bBoss,bAlien);
    }


}

void explosionenentfernen()
{
    for (int i=0;i<10;i++)
    {

       Explosion[i].ueberpruefen();
    }
}

void schiessenerlauben()
{
    Spieler[0].schiessenerlauben();

}

void schussbewegen()
{
    for (int i=0; i<10;i++)
    {
        if (Schuss[i].isActive())
        Schuss[i].bewegen(Spiel.getschussgeschwSpieler());
    }

    for (int i=0; i<100;i++)
    {
         if (Alienschuss[i].isActive())
        Alienschuss[i].bewegen(Spiel.getschussgeschwAliens());
    }
}




///Server
void sendPositions()
{
    ///alle Positionen und leben etc. aller Objekte an Clienten senden
  // char data[100]="test";

// UDP socket:
sf::IpAddress recipient(Spiel.getIpAdressConnect());
unsigned short port = Spiel.getPortConnect();


    /*wxString meldung5;
    meldung5 << port;
    wxMessageBox("Sende an PORT" +meldung5);
*/

sf::Packet packet;
for (int i=0; i<40;i++)
{
  packet << Alien[i].getX() << Alien[i].getY() << Alien[i].isActive();
}
for (int i=0; i<10;i++)
{
  packet << Schuss[i].getX() << Schuss[i].getY() << Schuss[i].isActive();
}
for (int i=0; i<100;i++)
{
  packet << Alienschuss[i].getX() << Alienschuss[i].getY() << Alienschuss[i].isActive();
}
for (int i=0; i<10;i++)
{
  packet << Explosion[i].getX() << Explosion[i].getY() << Explosion[i].isActive();
}

  packet << Spieler[0].getX() << Spieler[0].getY() << Spieler[0].isActive();
  packet << Spieler[0].getPunkte();

  wxString mystring=Spieler[0].getNameVorschalg();
    std::string stlstring = std::string(mystring.mb_str());
    packet << stlstring;


if (socket1.send(packet, recipient, port) != sf::Socket::Done)
{
    // error...
    //timer->stop();
    wxMessageBox("failed to send");
}
}

void receiveCommands()
{
    /*timer->stop();
    wxString meldung5;
    meldung5 << socket1.getLocalPort();
    wxMessageBox("Emfpange auf PORT" +meldung5);
*/
// UDP socket:
sf::IpAddress sender;
unsigned short port;

sf::Packet packet;
if (socket1.receive(packet, sender, port) != sf::Socket::Done)
{
    // error...
    //timer->stop();

    //wxMessageBox("failed to receive");
} else {
int x,y;
bool isactive=false;

std::string clientIP;
int clientPORT;

packet >> clientIP >> clientPORT;
wxString clientIPwx(clientIP);
Spiel.setIpAdressConnect(clientIPwx);
Spiel.setPortConnect(clientPORT);

//timer->stop();
//wxMessageBox(Spiel.getIpAdressConnect());

packet >> x >> y >> isactive >> clientSchuss >> clientGetroffen;
Spieler[1].setX(x);
Spieler[1].setY(y);
Spieler[1].setActive(isactive);

std::string stlstring;
packet >> stlstring;
// assuming your string is encoded as UTF-8, change the wxConv* parameter as needed
wxString mystring(stlstring.c_str(), wxConvUTF8);
Spieler[1].setName(mystring);


if (clientSchuss)
{
    Spieler[1].schiessen(Schuss);
    clientSchuss=false;

}

if (clientGetroffen)
{
    Spiel.spawnReinigen();
    clientGetroffen=false;
}



}
}




///Client
void receivePositions()
{
    char data[100];
std::size_t received;

// UDP socket:
sf::IpAddress sender;
unsigned short port;

    /*wxString meldung5;
    meldung5 << socket1.getLocalPort();
    wxMessageBox("Empfange auf Port " +meldung5);
*/
sf::Packet packet;
if (socket1.receive(packet, sender, port) != sf::Socket::Done)
{

} else {
int x,y;
bool isactive=false;

for (int i=0; i<40;i++)
{

  packet >> x >> y >> isactive;
  Alien[i].setX(x);
  Alien[i].setY(y);
  Alien[i].setActive(isactive);

}
for (int i=0; i<10;i++)
{
  packet >> x >> y >> isactive;
  Schuss[i].setX(x);
  Schuss[i].setY(y);
  Schuss[i].setActive(isactive);

}
for (int i=0; i<100;i++)
{
  packet >> x >> y >> isactive;
  Alienschuss[i].setX(x);
  Alienschuss[i].setY(y);
  Alienschuss[i].setActive(isactive);

}
for (int i=0; i<10;i++)
{
  packet >> x >> y >> isactive;
  Explosion[i].setX(x);
  Explosion[i].setY(y);
  Explosion[i].setActive(isactive);

}
packet >> x >> y >> isactive;
  Spieler[1].setX(x);
  Spieler[1].setY(y);
  Spieler[1].setActive(isactive);

packet >> x;
Spieler[0].setPunkte(x);

std::string stlstring;
packet >> stlstring;
// assuming your string is encoded as UTF-8, change the wxConv* parameter as needed
wxString mystring(stlstring.c_str(), wxConvUTF8);
Spieler[1].setName(mystring);
}
}

void sendCommands()
{
  ///Befehle senden
  sf::IpAddress recipient(Spiel.getIpAdressConnect());
    std::string clientIP;

  if (Spiel.getMultiplayerMode()==spiel::lan)
  clientIP = recipient.getLocalAddress().toString();

   if (Spiel.getMultiplayerMode()==spiel::online)
  clientIP = recipient.getPublicAddress().toString();

  int clientPORT = Spiel.getPortEigen();
unsigned short port = Spiel.getPortConnect();

    /*timer->stop();
    wxString meldung5;
    meldung5 << port;
    wxMessageBox("Sende an Port "+meldung5);
*/

sf::Packet packet;

  packet << clientIP << clientPORT << Spieler[0].getX() << Spieler[0].getY() << Spieler[0].isActive() << clientSchuss << clientGetroffen;

    wxString mystring=Spieler[0].getNameVorschalg();
    std::string stlstring = std::string(mystring.mb_str());
    packet << stlstring;


if (socket1.send(packet, recipient, port) != sf::Socket::Done)
{
    // error...
    timer->stop();
    wxMessageBox("failed to send");
}
clientSchuss=false;
clientGetroffen=false;
}




class BasicDrawPane : public wxPanel
{

public:
    BasicDrawPane(wxFrame* parent);

    void paintEvent(wxPaintEvent& evt);
    void paintNow();
    void render( wxDC& dc );

    DECLARE_EVENT_TABLE()
};

class MyFrame;

class MyApp: public wxApp
{

    bool OnInit();


    MyFrame* frame;
public:

};


IMPLEMENT_APP(MyApp)

class MyFrame : public wxFrame
{

    BasicDrawPane* drawPane;

public:
    MyFrame() : wxFrame((wxFrame *)NULL, -1,  wxT("Space Invaders"), wxPoint(50,50), wxSize(500,500),wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
    {


        drawPane = new BasicDrawPane( this );
        drawPane->Bind(wxEVT_CHAR_HOOK, &MyFrame::KeyDown, this);

        SetMinSize(GetSize());
        SetMaxSize(GetSize());

        SetIcon(wxICON(spaceinvadersicon));


        timer = new RenderTimer(drawPane);
        Show();
        timer->start(Spiel.getSpielgeschwindigkeit());

    }
    ~MyFrame()
    {
        delete timer;
    }


    void onClose(wxCloseEvent& evt)
    {
        timer->Stop();
        evt.Skip();
        Spiel.writeLog("Game closed");
    }
    void KeyDown(wxKeyEvent& event)
    {
    if (Spiel.isGameInForeground())
    {


        if ((event.GetKeyCode()==80))           ///P        Pause
        {
            Spiel.writeLog(wxT("Player pressed P"));
           if (Spiel.isGameRunning()) {timer->Stop(); Spiel.stopGame();}
           else {timer->start(Spiel.getSpielgeschwindigkeit()); Spiel.resumeGame();}
        }

        if ((event.GetKeyCode()==82))           ///R        Neustart
        {
            Spiel.writeLog(wxT("Player pressed R"));
            Spiel.werteuebernehmen();
            Spieler[0].setPunkte(0);
            Spieler[0].allowShooting();
            Spieler[0].setLeben(Spiel.getlebenNEU());
            Spieler[0].activate();
            if (Spiel.isGameRunning()) timer->start(Spiel.getSpielgeschwindigkeit());
            Spiel.startNewRound(bBoss,bAlien);



        }

        if (event.GetKeyCode()==WXK_F1)         /// F1      Hilfe
        {
            Spiel.writeLog(wxT("Player pressed F1"));
        timer->stop();
        wxMessageBox("A / Pfeiltaste Links                          Raumschiff nach links bewegen \nD / Pfeiltaste Rechts                       Raumschiff nach rechts bewegen \nW / Leertaste / Pfeiltaste oben     schiessen\nR                                                        Neustart \nF1                                                       Hilfe \nH                                                        Highscore","Hilfe" ,wxICON_QUESTION);
        if (Spiel.isGameRunning()) timer->start(Spiel.getSpielgeschwindigkeit());
        }

        if (event.GetKeyCode()==72)            ///H        Highscore
        {
            Spiel.writeLog(wxT("Player pressed H"));
            timer->stop();
            Spiel.highscoreZeigen(&ftp);
            if (Spiel.isGameRunning()) timer->start(Spiel.getSpielgeschwindigkeit());
        }

        if ((event.GetKeyCode()==27))           ///ESC      Beenden
        {
           Close();
        }

        if (event.GetKeyCode()==69)         ///E        Einstellungen
        {
            Spiel.writeLog(wxT("Player pressed E"));
            timer->stop();

            Spiel.einstellungen();

            if (Spiel.isGameRunning()) timer->start(Spiel.getSpielgeschwindigkeit());
        }

        if (event.GetKeyCode()==77)
        {
            Spiel.writeLog(wxT("Player pressed M"));
           if (!mute) {mute=true;} else {mute=false;}
           if (mute) {Hintergrundmusik.stop();} else {Hintergrundmusik.play();}
        }

    }

        event.Skip();
    }

    DECLARE_EVENT_TABLE()
};



BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE(MyFrame::onClose)
EVT_KEY_DOWN(MyFrame::KeyDown)
END_EVENT_TABLE()


void loadAllFiles()
{
    wxInitAllImageHandlers();
    bHintergrund.LoadFile("Images\\Hintergrund.png",wxBITMAP_TYPE_PNG);
    bRaumschiff.LoadFile("Images\\Raumschiff.png",wxBITMAP_TYPE_PNG);
    for (int i=0;i<4;i++)
    {
        Spieler[i].setHoehe(bRaumschiff.GetHeight());
        Spieler[i].setBreite(bRaumschiff.GetWidth());
    }
    bSchuss.LoadFile("Images\\Munition.png",wxBITMAP_TYPE_PNG);
    for (int i=0;i<10;i++)
    {
        Schuss[i].setHoehe(bSchuss.GetHeight());
        Schuss[i].setBreite(bSchuss.GetWidth());
    }
    bAlienschuss.LoadFile("Images\\Alienmunition.png",wxBITMAP_TYPE_PNG);
      for (int i=0;i<100;i++)
    {
        Alienschuss[i].setHoehe(bAlienschuss.GetHeight());
        Alienschuss[i].setBreite(bAlienschuss.GetWidth());
    }
    bAlien.LoadFile("Images\\Alien.png",wxBITMAP_TYPE_PNG);
       for (int i=0;i<40;i++)
    {
        Alien[i].setHoehe(bAlien.GetHeight());
        Alien[i].setBreite(bAlien.GetWidth());
    }
    bLeben.LoadFile("Images\\Leben.png",wxBITMAP_TYPE_PNG);

    bExplosion.LoadFile("Images\\Explosion.png",wxBITMAP_TYPE_PNG);
       for (int i=0;i<10;i++)
    {
        Explosion[i].setHoehe(bExplosion.GetHeight());
        Explosion[i].setBreite(bExplosion.GetWidth());
    }

    bBoss[0].LoadFile("Images\\boss\\boss1.png",wxBITMAP_TYPE_PNG);
    bBoss[1].LoadFile("Images\\boss\\boss2.png",wxBITMAP_TYPE_PNG);
    bBoss[2].LoadFile("Images\\boss\\boss3.png",wxBITMAP_TYPE_PNG);
    bBoss[3].LoadFile("Images\\boss\\boss4.png",wxBITMAP_TYPE_PNG);
    bBoss[4].LoadFile("Images\\boss\\boss5.png",wxBITMAP_TYPE_PNG);


    Hintergrundmusik.openFromFile("music\\SpaceInvadersSoundtrack.wav");
    bufferexplosion.loadFromFile("music\\explosion.wav");
    bufferschuss.loadFromFile("music\\laser.wav" );

    Soundexplosion.setBuffer(bufferexplosion);
    Soundschuss.setBuffer(bufferschuss);

    Soundexplosion.setVolume(10);
    Soundschuss.setVolume(10);

    Hintergrundmusik.setLoop(true);
    Hintergrundmusik.play();

    _mkdir("Highscore");
    ftp.connect("staacraft.square7.ch", 21, sf::seconds(2));
    ftp.login("staacraft_SpaceInvaders", "1324");
    //ftp.login();
    ftp.download("HighscoreOnline.txt", "Highscore", sf::Ftp::Ascii);

    Spiel.writeLog("all Files succesfully loaded");


}

bool MyApp::OnInit()
{
     Spiel.writeLog("");
     Spiel.writeLog("");
    Spiel.writeLog("Game has started");

    wxFileConfig* configFile = new wxFileConfig(wxT(""),wxT(""),wxT("config.ini"),wxT(""),wxCONFIG_USE_RELATIVE_PATH);
    wxString str;
    configFile->Read("GameMode",&str);
   if (str=="server")
   {Spiel.setGameMode(spiel::server);}
    else if (str=="client")
   {Spiel.setGameMode(spiel::client);}
   else
    {Spiel.setGameMode(spiel::offline);}

     configFile->Read("MultiplayerMode",&str);
   if (str=="lan")
   {Spiel.setMultiplayerMode(spiel::lan);}
    else if (str=="online")
   {Spiel.setMultiplayerMode(spiel::online);}


     if (Spiel.getGameMode()==spiel::client)
     {configFile->Read("Server",&str);
        Spiel.setIpAdressConnect(str);}

        configFile->Read("Server",&str);
     Spiel.setIpAdressConnect(str);



      configFile->Read("Port",&str);
      Spiel.setPortEigen(wxAtoi(str));
    Spiel.writeLog("Gameconfig loaded");

    if (Spiel.getGameMode()==spiel::server)
    {

        // bind the socket to a port
        if (socket1.bind(Spiel.getPortEigen()) != sf::Socket::Done)
        {
        // error...
        wxMessageBox("Überprüfe deine Ports");
        }
        else
        {
            Spiel.setPortEigen(socket1.getLocalPort());
        }
    }
    if (Spiel.getGameMode()==spiel::client)
    {
        Spiel.setPortConnect(wxAtoi(str));
        // bind the socket to a port
        if (socket1.bind(sf::Socket::AnyPort) != sf::Socket::Done)
        {
        // error...
        wxMessageBox("Überprüfe deine Ports");
        }
        else
        {
         Spiel.setPortEigen(socket1.getLocalPort());
        }
    }


        double value;
        wxTextFile versionInfoTXT( wxT("version.txt") );
        versionInfoTXT.Create("version.txt");
        versionInfoTXT.Open();
        wxString tmp=versionInfoTXT.GetFirstLine();
        if(!tmp.ToDouble(&value)){ /* error! */ }
            value=value/10;
            tmp = wxString::Format(wxT("%.1lf"), value);

        Spiel.writeLog( "Gameversion loaded: " +  tmp);



          /*  tmp=versionInfoTXT.GetNextLine();
            if(!tmp.ToDouble(&value1)){ }
            value1=value1/10;
            tmp = wxString::Format(wxT("%.1lf"), value1);
        lbupdate->Append( "Gamefiles:  " + tmp );*/

        Spieler[0].activate();


    std::srand(std::time(0));   ///Zufallszahlen generieren


        ///Bilder+Musik+HighscoreOnline Laden
     std::thread tLoadAllFiles(loadAllFiles);


    frame = new MyFrame();             ///Fenster wird erstellt
    frame->Show();                      ///Fenster wird angezeigt


    Spiel.saveForegroundWindow();                            ///Speichert (hoffentlich) SpaceInvaders, damit Tastatureingaben nur
                                                            ///funktionieren, wenn SpaceInvaders im Vordergrund ist

    tLoadAllFiles.join();

    wxString mystring;
    mystring << Spiel.getPortEigen();
   // wxMessageBox("Eigener Port:"+ mystring);




    return true;
}




BEGIN_EVENT_TABLE(BasicDrawPane, wxPanel)
EVT_PAINT(BasicDrawPane::paintEvent)
END_EVENT_TABLE()


void zeichnen(BasicDrawPane* pane)
{
 pane->Refresh();
}

void RenderTimer::Notify()
{

   if (Spiel.isGameRunning() || Spiel.getGameMode()==spiel::server)
   {


    if (Spiel.getGameMode()==spiel::offline)
    {
    std::thread t0(schussbewegen);
    std::thread t1(schiessenerlauben);
    std::thread t2(tastatureingaben);               ///Startet alle Funktionen als Threads
    std::thread t3(schussloeschen);                 ///Threads können gleichzeitig ablaufen
    std::thread t4(trefferregistrieren);
    std::thread t5(explosionenentfernen);
    std::thread t6(alienBewegen);


    std::thread t7(zeichnen,pane);

              //hier laufen alle Funktionen parallel
    t0.detach();
    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();                                  ///Hier wird auf alle Funktionen gewartet
    t5.detach();
    t6.detach();


    endeerkennug();     ///bei den zwei gab es Probleme als Threads
    alienschiessen();

    t7.detach();       ///Das Bild wird neu gemalt
    }

      if (Spiel.getGameMode()==spiel::server)
    {
        std::thread t0(schussbewegen);
        std::thread t1(schiessenerlauben);
        std::thread t2(tastatureingaben);               ///Startet alle Funktionen als Threads
        std::thread t3(schussloeschen);                 ///Threads können gleichzeitig ablaufen
        std::thread t4(trefferregistrieren);
        std::thread t5(explosionenentfernen);
        std::thread t6(alienBewegen);

        std::thread t8(sendPositions);
        std::thread t9(receiveCommands);


        std::thread t7(zeichnen,pane);

              //hier laufen alle Funktionen parallel
    t0.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();                                  ///Hier wird auf alle Funktionen gewartet
    t5.join();
    t6.join();
    t8.detach();
    t9.detach();

    endeerkennug();     ///bei den zwei gab es Probleme als Threads
    alienschiessen();

    t7.join();       ///Das Bild wird neu gemalt
    }

      if (Spiel.getGameMode()==spiel::client)
    {
    std::thread t2(tastatureingaben);               ///Startet alle Funktionen als Threads
    std::thread t1(schiessenerlauben);
    std::thread t7(zeichnen,pane);
    std::thread t4(trefferregistrieren);
    std::thread t8(receivePositions);
    std::thread t9(sendCommands);

              //hier laufen alle Funktionen parallel
    t1.join();
    t2.join();
    t4.join();
    t8.detach();
    t9.detach();
    t7.join();       ///Das Bild wird neu gemalt
    }


   }
}



BasicDrawPane::BasicDrawPane(wxFrame* parent) :
wxPanel(parent)
{
}


void BasicDrawPane::paintEvent(wxPaintEvent& evt)
{

    wxBufferedPaintDC dc(this);
    render(dc);
}


void BasicDrawPane::render( wxDC& dc )
{


    ///Rendering während das Spiel läuft

    if (Spiel.isGameRunning() || Spiel.getGameMode()!=spiel::offline)
    {

        SetBackgroundStyle(wxBG_STYLE_PAINT);

        if (bHintergrund.IsOk())
        {
            dc.DrawBitmap(bHintergrund,0,0);

        }

        if (bSchuss.IsOk())
        {
            for (int i=0; i<10; i++)
            {
                 if (Schuss[i].isActive())
                dc.DrawBitmap(bSchuss,Schuss[i].getX(),Schuss[i].getY());
            }
        }

        if (bAlienschuss.IsOk())
        {
            for (int i=0; i<100; i++)
            {
                 if (Alienschuss[i].isActive())
                dc.DrawBitmap(bAlienschuss,Alienschuss[i].getX(),Alienschuss[i].getY());
            }
        }


        if (bRaumschiff.IsOk())
        {
             for (int i=0; i<2; i++)
            {
               if (Spieler[i].isActive())
                {
                    dc.DrawBitmap(bRaumschiff,Spieler[i].getX(),Spieler[i].getY());
                }
            }
        }
            if (Spiel.getGameMode()!=spiel::offline)
            {

                if (Spieler[0].isActive())
                {
                    dc.SetTextForeground( *wxRED );
                    dc.SetFont(wxFontInfo(5).FaceName("Distant Galaxy").Light());
                    dc.DrawText(Spieler[0].getNameVorschalg(),Spieler[0].getX(),Spieler[0].getY()+50);
                }
                  if (Spieler[1].isActive())
                {
                    dc.SetTextForeground( *wxYELLOW );
                    dc.SetFont(wxFontInfo(5).FaceName("Distant Galaxy").Light());
                    dc.DrawText(Spieler[1].getName(),Spieler[1].getX(),Spieler[1].getY()+50);
                }
            }

            for (int i=0; i<40; i++)
            {
                if (Alien[i].isActive())
                {



                 if (bAlien.IsOk() && Alien[i].getType()==1)
                    dc.DrawBitmap(bAlien,Alien[i].getX(),Alien[i].getY());


                    if (Alien[i].getType()==2 && bBoss[0].IsOk())
                    {
                      dc.DrawBitmap(bBoss[Alien[i].getLeben()-1],Alien[i].getX(),Alien[i].getY());
                    }

                }
            }

            for (int i=1; i<=Spieler[0].getLeben(); i++)
            {
                dc.DrawBitmap(bLeben,i*50-40,430);
            }





        ///Punktestand
        dc.SetTextForeground( *wxRED );
        dc.SetFont(wxFontInfo(12).FaceName("Distant Galaxy").Light());

        wxString punktstand="Punkte: ";
        punktstand << Spieler[0].getPunkte();

        dc.DrawText(punktstand, 380, 10);



        if (bExplosion.IsOk())
        {
           for (int i=0; i<10; i++)
            {
                if (Explosion[i].isActive())
                dc.DrawBitmap(bExplosion,Explosion[i].getX(),Explosion[i].getY());
            }
        }

    }

    ///Game Over
 if (Spieler[0].getLeben()==0)
 {
    dc.SetTextForeground( *wxRED );
    dc.SetFont(wxFontInfo(28).FaceName("Distant Galaxy").Light());
    dc.DrawText(wxT("Du hast Verloren"), 50, 200);
    dc.SetFont(wxFontInfo(15).FaceName("Distant Galaxy").Light());
    dc.DrawText(wxT("Neustart mit R"), 150, 250);

 }

 ///Spielstart Anzeige
 if (Spieler[0].getLeben()==-100)
 {
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    dc.SetTextForeground( *wxRED );
    dc.SetFont(wxFontInfo(40).FaceName("Distant Galaxy").Light());
    dc.DrawText(wxT("Space Invaders"), 20, 200);
    dc.SetFont(wxFontInfo(20).FaceName("Distant Galaxy").Light());
    dc.DrawText(wxT("Start mit R"), 160, 280);
 }


}
