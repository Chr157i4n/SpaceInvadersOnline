#ifndef SPIEL_H
#define SPIEL_H
#include <SFML/Network.hpp>

class spieler;
class alien;
class schuss;
class explosion;
class alienschuss;


class spiel
{
     public:
             enum gameMode {offline=1,server=2,client=3};
             enum multiplayerMode {lan=1,online=2};

            void writeLog(wxString logtext);

            spiel(alien Alien[], alienschuss Alienschuss[], schuss Schuss[],explosion Explosion[],spieler Spieler[]);
            virtual ~spiel();
            void startNewRound(wxBitmap bBoss[],wxBitmap bAlien);

            void highscore(spieler* Spieler,sf::Ftp* ftp);

            void einstellungen();
            void highscoreZeigen(sf::Ftp* ftp);

            void aliensGeschwindigkeitErhoehen(alien Alien[]);


            void spawnReinigen();
            void addPunkte();
            void werteuebernehmen();
            void saveForegroundWindow(){fensterImVordergrund=GetForegroundWindow();};

            int getSpielgeschwindigkeit(){return timerzeit;};
            int getAliensProRunde(){anzahlAlienNEU++; return anzahlAlienNEU;};
            int getlebenNEU(){return lebenNEU;};
            int getBreite(){return fensterBreite;};
            int getHoehe(){return fensterHoehe;};
            wxPoint getAlienGeschw(){return geschwAlien;};
            int getschusswahrscheinlichkeit(){return schusswahrscheinlichkeit;};
            int getschussgeschwAliens(){return schussgeschwAliens;};
            int getschussgeschwSpieler(){return schussgeschwSpieler;};

            bool isGameRunning(){return spiellaeuft;};
            void stopGame(){if (spiellaeuft)spiellaeuft=false;};
            void resumeGame(){if (!spiellaeuft) spiellaeuft=true;};
            bool isGameInForeground();

            int getGameMode(){return gameMode;};
            void setGameMode(spiel::gameMode mode){gameMode=mode;};

            int getMultiplayerMode(){return multiplayerMode;};
            void setMultiplayerMode(spiel::multiplayerMode mode){multiplayerMode=mode;};

            void setPortEigen(int port){mPortEigen=port;};
            int getPortEigen(){return mPortEigen;};
            void setPortConnect(int port){mPortConnect=port;};
            int getPortConnect(){return mPortConnect;};

            void setIpAdressEigen(wxString IpAdress){mIpAdressEigen=IpAdress;};
            wxString getIpAdressEigen(){return mIpAdressEigen;};

             void setIpAdressConnect(wxString IpAdress){mIpAdressConnect=IpAdress;};
            wxString getIpAdressConnect(){return mIpAdressConnect;};


    private:


    gameMode gameMode= offline;
    multiplayerMode multiplayerMode = lan;
    //int gameMode=2; ///1 = Offline, 2 = Server, 3 = Client
    wxString mIpAdressEigen="0",mIpAdressConnect="0";
    int mPortEigen=0,mPortConnect=0;;



    alien* mAlien;
    schuss* mSchuss;
    explosion* mExplosion;
    alienschuss* mAlienschuss;
    spieler* mSpieler;


        wxPoint geschwAlien;
        int schusswahrscheinlichkeit;           ///aktuelle Spielwerte
        int schussgeschwAliens=2, schussgeschwSpieler;


    sf::Ftp::Response response;
    bool spiellaeuft=false;
    HWND fensterImVordergrund;
    int geschwXNEU=1, geschwYNEU=15, schusswahrscheinlichkeitNEU=5;       ///Einstellungen
    int lebenNEU=3, anzahlAlienNEU=19, lebenPUNKTE=3, schussgeschwSpielerNEU=3;
    int timerzeit=15;
    int fensterHoehe=500, fensterBreite=500;
};

#endif // SPIEL_H
