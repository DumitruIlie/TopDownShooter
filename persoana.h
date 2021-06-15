#include<graphics.h>
#include<cmath>
#include<cstdio>

const double grad=M_PI/180;

double modul(double x)
{
    return x>0?x:0-x;
}

void prop(double &x, double &y, double origineX, double origineY, double MAXW, double MAXH)
{
    double deltaX=x-origineX, deltaY=y-origineY;
    if(deltaX==0)
    {
        if(deltaY!=0)
        {
            if(deltaY>0)
                y=MAXH;
            else
                y=0;
        }
    }
    else
    {
        if(modul(deltaX)>modul(deltaY))
        {
            if(deltaX>0)
            {
                if(deltaY>=0)
                {
                    x=MAXW;
                    y=deltaY*(MAXH-origineX)/deltaX+origineY;
                }
                else
                {
                    y=0;
                    x=-deltaX*origineY/deltaY+origineX;
                }
            }
            else
            {
                if(deltaY>=0)
                {
                    x=0;
                    y=-deltaY*origineX/deltaX+origineY;
                }
                else
                {
                    x=0;
                    y=origineY-origineX*deltaY/deltaX;
                }
            }
        }
        else
        {
            if(deltaY>0)
            {
                if(deltaX>=0)
                {
                    y=MAXH;
                    x=(MAXH-origineY)*deltaX/deltaY+origineX;
                }
                else
                {
                    x=0;
                    y=-origineX*deltaY/deltaX+origineY;
                }
            }
            else
            {
                if(deltaX>=0)
                {
                    x=-origineY*deltaX/deltaY+origineX;
                    y=0;
                }
                else
                {
                    y=0;
                    x=origineX-deltaX*origineY/deltaY;
                }
            }
        }
    }
}

#ifndef PERSOANA_H
#define PERSOANA_H


class persoana
{
    public:
        double x, y, lastX, lastY, angle, lastAngle, cooldown, trecut, aux1, aux2;
        int HP, HP_pas;
        int HEALTHHEIGHT, MAXH, MAXW;

        void damage(int DMGP)
        {
            HP-=DMGP;
            setfillstyle(9, COLOR(200, 20, 20));
            setcolor(BLACK);
            bar(0, MAXH, HP_pas*HP, MAXH+HEALTHHEIGHT);
        }

        void show()
        {
            lastAngle=angle;
            setcolor(COLOR(20, 20, 200));
            setfillstyle(1, COLOR(20, 20, 200));
            fillellipse(x, y, 4, 4);
            aux1=cos(angle)*10+x;
            aux2=sin(angle)*10+y;
            prop(aux1, aux2, x, y, MAXW, MAXH);
            if(trecut<=0)
                setcolor(COLOR(200, 20, 20));
            else
                setcolor(COLOR(200, 200, 20));
            line(x, y, aux1, aux2);

            setcolor(COLOR(0, 0, 0));
            setfillstyle(9, COLOR(200, 20, 20));
            bar(0, MAXH, HP*HP_pas, MAXH+HEALTHHEIGHT);
            setfillstyle(1, COLOR(0, 0, 0));
            bar(HP_pas*HP, MAXH, MAXW, MAXH+HEALTHHEIGHT);
        }

        void move(double dir, double norm)
        {
            ///1=W, -1=S
            double s=sin(angle), c=cos(angle);
            x+=c*dir*3-s*norm*3;
            y+=s*dir*3+c*norm*3;
            if(x>MAXW-4)
                x=MAXW-4;
            if(y>MAXH-4)
                y=MAXH-4;
            if(y<4)
                y=4;
            if(x<4)
                x=4;
        }

        inline void rotate(double newAngle) {angle=newAngle;}
};

#endif // PERSOANA_H