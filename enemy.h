#include<graphics.h>
#include<math.h>

#ifndef ENEMY_H
#define ENEMY_H

class enemy
{
    public:

        int DMGP;
        double x, y, speedPerSec, cooldown, trecut;

        void calcProjection(persoana p, double &x1, double &y1, double &x2, double &y2)
        {
            double deltaX=x-p.x, deltaY=y-p.y;
            x1=x;
            y2=y;
            y1=sin(p.angle)*deltaX/cos(p.angle)+p.y;
            x2=cos(p.angle)*deltaY/sin(p.angle)+p.x;
        }

        void show()
        {
            setcolor(COLOR(20, 200, 20));
            setfillstyle(1, COLOR(20, 200, 20));
            fillellipse(x, y, 4, 4);
        }

        int move(persoana p)
        {
            double deltaY=y-p.y, deltaX=x-p.x;
            if(deltaX*deltaX+deltaY*deltaY<16)
                return DMGP;
            double modulVector=sqrt(deltaX*deltaX+deltaY*deltaY);
            x-=speedPerSec*deltaX/modulVector;
            y-=speedPerSec*deltaY/modulVector;
            show();
            return 0;
        }
};

#endif // ENEMY_H