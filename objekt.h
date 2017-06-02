#ifndef OBJEKT_H
#define OBJEKT_H


class objekt
{
    public:
        objekt();
        virtual ~objekt()=0;


        int getX(){return mX;};
        int getY(){return mY;};
        int getBreite(){return mBreite;};
        int getHoehe(){return mHoehe;};

        void activate(){if (!active) active=true;};
         void deactivate(){if (active) active=false;};
         bool isActive(){return active;};
         void setActive(bool state){active=state;};

        void decleben();
        bool isAlive(){return mLeben>0;};

        int getLeben(){return mLeben;};

        void setBreite(int breite)
        {if (breite>0 && breite<200) mBreite=breite;};

        void setHoehe(int hoehe)
        {if (hoehe>0 && hoehe<200) mHoehe=hoehe;};

        void setX(int x)
        {if (x>0 && x<500) mX=x;};
        void setY(int y)
        {if (y>0 && y<500) mY=y;};

        int getType(){return mObjektType;};
        void changeType(int type,wxBitmap,int leben,int schusswahrscheinlichkeit);




    protected:
        bool active=false;
        int mBreite,mHoehe;
        int mX;
        int mY;
        int mLeben,mObjektType;
        int mWahrscheinlichkeit=1;
    private:


};

#endif // OBJEKT_H
