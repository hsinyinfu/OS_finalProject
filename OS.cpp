#include<iostream>
#include<stdlib.h>
#include<thread>
#include<mutex>
#include<unistd.h>
#include"SemaphoreClass.h"
#include"RGBpixmap.h"
#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

using namespace std;
#define THREAD_NUM 9

mutex mtx;
bool isTabacco = false;	//If tobacco is on the table(supplied by the Agent).
bool isPaper = false;	//If paper is on the table(supplied by Agent).
bool isMatch = false;	//If match is on the table(supplied by Agent).
Semaphore agSem(1);		/*Semaphore used to wake up one of the Agents
						  to suppy ingredients.*/
Semaphore tobacco(0);	//Tell that tobacco is supplied.
Semaphore paper(0);		//Tell that paper is supplied.
Semaphore match(0);		//Tell that match is supplied.
Semaphore tobaccoSem(0);//Tell the smoker with tobacco that he can smoke.
Semaphore paperSem(0);	//Tell the smoker with paper that he can smoke.
Semaphore matchSem(0);	//Tell the smoker with match that he can smoke.

int roundCase = 0;//modify to roundCase because I import math::round(It will cause errors with duplicate name)
int passed = 0;
bool isEnd = false;

void createThread(thread[]);
void smkrTobacco(); //function of smoker with tobacco
void smkrPaper(); //function of smoker with paper
void smkrMatch(); //function of smoker with match
void agA(); //function for supplied of tobacco and paper.
void agB(); //function for supplied of paper and match.
void agC(); //function for supplied of tobacco and match.
void pusherA();	//function that tobacco is supplied.
void pusherB();	//function that paper is supplied.
void pusherC();	//function that match is supplied.

int screenWidth=800; //default window size
int screenHeight=600;//default window size

static void CheckError(int line)
{
    GLenum err = glGetError();
    if (err) 
    {
        printf("GL Error %s (0x%x) at line %d\n",gluErrorString(err), (int) err, line);
    }
}
class Creature{
    public:
        Creature(){}
        Creature(double _x,double _y){x=_x,y=_y;}
        void setCreature(int n){
            if(n==0)
            {
                pic.readBMPFile("image/R.bmp");
                pic.setChromaKey(1.0,1.0,1.0);
            }
            else if(n==1)
            {
                pic.readBMPFile("image/G.bmp");
                pic.setChromaKey(1.0,1.0,1.0);
            }
            else
            {
                pic.readBMPFile("image/B.bmp");
                pic.setChromaKey(1.0,1.0,1.0);
            }
        }
        RGBApixmap pic;
        void setXY(double _x,double _y){x=_x,y=_y;}
        double getX(){return x;}
        double getY(){return y;}
    private:
        double x;
        double y;
};
//Object initialization
Creature personWithTobacco(100,450);
Creature personWithPaper(100,250);
Creature personWithMatch(100,50);
//resource below
Creature rTobacco(-100,450);
Creature rPaper(-100,250);
Creature rMatch(-100,50);
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);//clean buffer
    personWithTobacco.pic.blendTex(personWithTobacco.getX(),personWithTobacco.getY());
    personWithPaper.pic.blendTex(personWithPaper.getX(),personWithPaper.getY());
    personWithMatch.pic.blendTex(personWithMatch.getX(),personWithMatch.getY());
    rTobacco.pic.blendTex(rTobacco.getX(),rTobacco.getY());
    rPaper.pic.blendTex(rPaper.getX(),rPaper.getY());
    rMatch.pic.blendTex(rMatch.getX(),rMatch.getY());
    CheckError(__LINE__);
	glutSwapBuffers();
}
void reshape(int w,int h)
{
    screenWidth=w;
    screenHeight=h;
    glViewport(0,0,screenWidth,screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0,(GLfloat)screenWidth,0.0,(GLfloat)screenHeight,-1.0,1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void init(){
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);	
	glShadeModel(GL_SMOOTH);

	glClearColor(1.0f,1.0f,0.4f,1.0);
}
void action(int Case){
    switch(Case)
    {
        case 1:
            rPaper.setXY(650,rPaper.getY());
            rMatch.setXY(650,rMatch.getY());
            glutPostRedisplay();
            usleep(50000);
            break;
        case 2:
            rTobacco.setXY(650,rTobacco.getY());
            rMatch.setXY(650,rMatch.getY());
            glutPostRedisplay();
            usleep(50000);
            break;
        case 3:
            rPaper.setXY(650,rPaper.getY());
            rTobacco.setXY(650,rTobacco.getY());
            glutPostRedisplay();
            usleep(50000);
            break;
        case 4:
        {
            for(int i=0;i<80;i++)
            {
                if(i<40)
                    personWithTobacco.setXY(personWithTobacco.getX()+10,personWithTobacco.getY());
                else
                    personWithTobacco.setXY(personWithTobacco.getX()-10,personWithTobacco.getY());
                if(i==39)
                {
                    rPaper.setXY(-100,rPaper.getY());
                    rMatch.setXY(-100,rMatch.getY());
                }
                glutPostRedisplay();
                usleep(50000);
            }
            break;
        }
        case 5:
        {
            for(int i=0;i<80;i++)
            {
                if(i<40)
                    personWithPaper.setXY(personWithPaper.getX()+10,personWithPaper.getY());
                else
                    personWithPaper.setXY(personWithPaper.getX()-10,personWithPaper.getY());
                if(i==39)
                {
                    rTobacco.setXY(-100,rTobacco.getY());
                    rMatch.setXY(-100,rMatch.getY());
                }
                glutPostRedisplay();
                usleep(50000);
            }
            break;
        }
        case 6:
        {
            for(int i=0;i<80;i++)
            {
                if(i<40)
                    personWithMatch.setXY(personWithMatch.getX()+10,personWithMatch.getY());
                else
                    personWithMatch.setXY(personWithMatch.getX()-10,personWithMatch.getY());
                if(i==39)
                {
                    rPaper.setXY(-100,rPaper.getY());
                    rTobacco.setXY(-100,rTobacco.getY());
                }
                glutPostRedisplay();
                usleep(50000);
            }
            break;
        }
    }
}
int main(int argc,char **argv){
	srand(time(NULL));
	thread myThread[THREAD_NUM];
    //window initialization
    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(screenWidth,screenHeight);
	glutCreateWindow("OS_FinalProject");
	init();
    personWithTobacco.setCreature(0);
    personWithPaper.setCreature(1);
    personWithMatch.setCreature(2);
    rTobacco.setCreature(0);
    rPaper.setCreature(1);
    rMatch.setCreature(2);


	//cout <<"How many round?";
	//cin >> roundCase;
    roundCase=10;
	createThread(myThread);
	/*for(int i=0; i<THREAD_NUM; i++){
		myThread[i].join();
	}*/
    glutMainLoop();
    cout<<roundCase<<endl;  
}

void createThread(thread myThread[THREAD_NUM]){
	myThread[0] = thread(smkrTobacco);
	myThread[1] = thread(smkrPaper);
	myThread[2] = thread(smkrMatch);
	myThread[3] = thread(agA);
	myThread[4] = thread(agB);
	myThread[5] = thread(agC);
	myThread[6] = thread(pusherA);
	myThread[7] = thread(pusherB);
	myThread[8] = thread(pusherC);
}

void smkrTobacco(){
	while(1){
		tobaccoSem.wait();
		if(passed < roundCase){
			cout <<"tobacco\n";
            action(4);
			passed++;
		}
		if(passed == roundCase)
			isEnd = true;
		agSem.signal();
		if( isEnd ){
			paperSem.signal();
			matchSem.signal();
			break;
		}
	}
}
void smkrPaper(){
	while(1){
		paperSem.wait();
		if(passed < roundCase){
			cout <<"paper\n";
            action(5);
			passed++;
		}
		if(passed == roundCase)
			isEnd = true;
		agSem.signal();
		if( isEnd ){
			tobaccoSem.signal();
			matchSem.signal();
			break;
		}
	}
}
void smkrMatch(){
	while(1){
		matchSem.wait();
		if(passed < roundCase){
			cout <<"match\n";
            action(6);
			passed++;
		}
		if(passed == roundCase)
			isEnd = true;
		agSem.signal();
		if( isEnd ){
			tobaccoSem.signal();
			paperSem.signal();
			break;
		}
	}
}
void agA(){
	while(1){
		agSem.wait();
		if(passed < roundCase)
			cout <<"sup to pa.\n";
        action(3);
        tobacco.signal();
		paper.signal();
//        action(3);
		if( isEnd )
			break;
	}
}
void agB(){
	while(1){
		agSem.wait();
		if(passed < roundCase)
			cout <<"sup pa ma.\n";
        action(1);
		paper.signal();
		match.signal();
       // action(1);
		if( isEnd )
			break;
	}
}
void agC(){
	while(1){
		agSem.wait();
		if(passed < roundCase)
			cout <<"sup ma to.\n";
		action(2);
        match.signal();
		tobacco.signal();
        //action(2);
		if( isEnd )
			break;
	}
}
void pusherA(){
	while(1){
		tobacco.wait();
		unique_lock<mutex> lck(mtx);
		if(isPaper){
			isPaper = false;
			matchSem.signal();
		}
		else if(isMatch){
			isMatch = false;
			paperSem.signal();
		}
		else
			isTabacco = true;
		if( isEnd )
			break;
	}
}
void pusherB(){
	while(1){
		paper.wait();
		unique_lock<mutex> lck(mtx);
		if(isTabacco){
			isTabacco = false;
			matchSem.signal();
		}
		else if(isMatch){
			isMatch = false;
			tobaccoSem.signal();
		}
		else
			isPaper = true;
		if( isEnd )
			break;
	}
}
void pusherC(){
	while(1){
		match.wait();
		unique_lock<mutex> lck(mtx);
		if(isTabacco){
			isTabacco = false;
			paperSem.signal();
		}
		else if(isPaper){
			isPaper = false;
			tobaccoSem.signal();
		}
		else
			isMatch = true;
		if( isEnd )
			break;
	}
}
