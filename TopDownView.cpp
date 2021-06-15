#include<cstdio>
#include<graphics.h>
#include<ctime>
#include"persoana.h"
#include"enemy.h"

struct button
{
    int x, y, w, h;
    void *img;
};

int MAXH=600, MAXW=600, HEALTHHEIGHT=20, spawnRate=200;

persoana om;
const int maxEnemy=20;
enemy vect[maxEnemy];
const int startGame=time(NULL);
int bonus=0, nrE=0, state=0;
button close_button, unpause_button, start_button;

void simEnemy()
{
    int x;
    for(int i=0;i<nrE;++i)
    {
        x=vect[i].move(om);
        if(x && vect[i].trecut<=0)
        {
            om.damage(x);
            vect[i].trecut=vect[i].cooldown;
        }
        else if(vect[i].trecut>0)
            vect[i].trecut-=0.5;
    }
}

void addEnemy()
{
    vect[nrE].x=rand()%MAXW;
    vect[nrE].y=rand()%MAXH;
    vect[nrE].speedPerSec=double(time(NULL)-startGame)/100+0.05;
    vect[nrE].DMGP=(time(NULL)-startGame)/30+1;
    vect[nrE].cooldown=240/(time(NULL)-startGame+1)+4;
    vect[nrE].trecut=0;
    ++nrE;
}

void afisareE()
{
    for(int i=0;i<nrE;++i)
        vect[i].show();
}

bool isPressed(int key) {return GetKeyState(key) & 0x80000;}

inline int sgn(double x) {return (x>0)-(x<0);}

void simShoot()
{
    double p2e, c=cos(om.angle), s=sin(om.angle);
    int i, j;
    for(i=0;i<nrE;++i)
    {
        p2e=abs(s*(vect[i].x-om.x)-c*(vect[i].y-om.y));
        if(p2e<=4)
        {
            for(j=1+i;j<nrE;++j)
                vect[j-1]=vect[j];
            --nrE;
            --i;
        }
    }
    om.trecut=om.cooldown;
}

bool verifButton(button b) {return isPressed(VK_LBUTTON) && mousex()>=b.x-(b.w>>1) && mousey()>=b.y-(b.h>>1) && mousex()<=b.x+(b.w>>1) && mousey()<=b.y+(b.h>>1);}

void preload()
{
    initwindow(80, 40, "", GetSystemMetrics(SM_CXSCREEN));
    close_button.w=40;
    close_button.h=40;
    close_button.x=3*MAXW/4;
    close_button.y=MAXH/2;
    unpause_button.w=40;
    unpause_button.h=40;
    unpause_button.x=MAXW/4;
    unpause_button.y=MAXH/2;
    start_button.w=80;
    start_button.h=40;
    start_button.x=MAXW/2;
    start_button.y=MAXH/2;
    readimagefile("close_button.bmp", 0, 0, 39, 39);
    close_button.img=malloc(imagesize(0, 0, 39, 39));
    getimage(0, 0, 39, 39, close_button.img);
    readimagefile("unpause_button.bmp", 0, 0, 39, 39);
    unpause_button.img=malloc(imagesize(0, 0, 39, 39));
    getimage(0, 0, 39, 39, unpause_button.img);
    readimagefile("start_button.bmp", 0, 0, 79, 39);
    start_button.img=malloc(imagesize(0, 0, 79, 39));
    getimage(0, 0, 79, 39, start_button.img);
    closegraph();
}

void mainMenu()
{
    putimage(start_button.x-(start_button.w>>1), start_button.y-(start_button.h>>1), start_button.img, COPY_PUT);
    if(verifButton(start_button))
        state=1;
}

void pauseMenu()
{
    putimage(close_button.x-(close_button.w>>1), close_button.y-(close_button.h>>1), close_button.img, COPY_PUT);
    putimage(unpause_button.x-(unpause_button.w>>1), unpause_button.y-(unpause_button.h>>1), unpause_button.img, COPY_PUT);
    if(verifButton(close_button)) state=-1;
    if(verifButton(unpause_button)) state=1;
}

int main()
{
    preload();
    bool wait=false, done=false;
    int moveDir, normDir;
    srand(time(NULL)%10000);
    om.HEALTHHEIGHT=HEALTHHEIGHT=30;
    om.MAXH=MAXH;
    om.MAXW=MAXW;
    initwindow(MAXW, MAXH+HEALTHHEIGHT, "Top Down View Shooter", (GetSystemMetrics(SM_CXSCREEN)-MAXW)>>1, (GetSystemMetrics(SM_CYSCREEN)-MAXH-HEALTHHEIGHT)>>1, true);
    om.HP=100;
    om.HP_pas=MAXW/om.HP;
    om.lastX=om.x=MAXW>>1;
    om.lastY=om.y=(MAXH-HEALTHHEIGHT)>>1;
    om.lastAngle=om.angle=0;
    om.trecut=0;
    om.cooldown=2.4;
    setfillstyle(9, COLOR(200, 20, 20));
    setcolor(BLACK);
    bar(0, MAXH, MAXW, MAXH+HEALTHHEIGHT);
    while(om.HP>0 && !done)
    {
        cleardevice();
        if(GetKeyState(VK_ESCAPE)&0x80000)
        {
            if(wait)
                state=3-state;
            wait=false;
        }
        else
            wait=true;
        if(state==-1)
            done=true;
        else if(state==0)//Main menu
            mainMenu();
        else if(state==1)//Playing game
        {
            setcolor(COLOR(0, 0, 0));
            setlinestyle(0, 0, 1);
            om.rotate(atan2(mousey()-om.y, mousex()-om.x));
            moveDir=isPressed('W')-isPressed('S');
            normDir=isPressed('D')-isPressed('A');
            if(moveDir || normDir)
                om.move(moveDir, normDir);
            Sleep(10);
            simEnemy();
            afisareE();
            if(rand()%spawnRate==0 && nrE<maxEnemy)
                addEnemy();
            if(isPressed(VK_LBUTTON) && om.trecut<=0)
                simShoot();
            else if(om.trecut>0)
                om.trecut-=0.05;
            om.show();
        }
        else if(state==2)
            pauseMenu();
        swapbuffers();
    }
    closegraph();
    return 0;
}