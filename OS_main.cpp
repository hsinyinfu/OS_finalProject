#include<iostream>
#include<stdlib.h>
#include<thread>
#include<mutex>
#include<random>
#include<chrono>
#include"SemaphoreClass.h"
#include<unistd.h>
#include"RGBpixmap.h"
#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

using namespace std;

#define THREAD_NUM 6
#define TABACCO 1
#define PAPER 3
#define MATCH 5
#define MEAN 4 //for poisson random variable
#define EMPTY 0
#define TIME_FOR_ADJUSTMENT 100

mutex mtx;	/*lock which is used to prevent race condition between agents.*/
mutex mtx2; /*lock which is used to prevent race condition between agents and smoker who can access the table.*/
Semaphore agSem(0);	/*Semaphore used to wake up one of the Agents to suppy ingredients.*/
Semaphore smkrSem(0);
int tableContent;
int smokerComingTime;
int smokingTime;

/*
 * Used to generate poisson random number
 * */
default_random_engine generator;
poisson_distribution<int> distribution(MEAN);

/*
 * Variables used to set GUI window size & update
 * */
int screenWidth = 800;
int screenHeight = 600;
int deltaTime = 50000;	//default update time
long long waitingDelta = 1000000;

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
        Creature(){x=0,y=0;}
        Creature(double _x,double _y){x=_x,y=_y;}

        RGBApixmap pic;
        double x;
        double y;
};
//Object initialization
Creature tobacco(100,50);
Creature paper(100,250);
Creature match(100,450);
Creature agent(600,250);
Creature T(-150,-150);
Creature P(-150,-150);
Creature M(-150,-150);
void cleanTable();
void createThread(thread[]);
void smkrTobacco(); //function of smoker with tobacco
void smkrPaper(); //function of smoker with paper
void smkrMatch(); //function of smoker with match
void agTobacco(); //function for supplied of tobacco.
void agPaper(); //function for supplied of paper.
void agMatch(); //function for supplied of match.
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);//clean buffer
    //print people and resources' position(following 6 line)
    tobacco.pic.blendTex(tobacco.x,tobacco.y,0.5f,0.5f);
    paper.pic.blendTex(paper.x,paper.y,0.5f,0.5f);
    match.pic.blendTex(match.x,match.y,0.5f,0.5f);
    agent.pic.blendTex(agent.x,agent.y,0.5f,0.5f);
    T.pic.blendTex(T.x,T.y,0.5f,0.5f);
    P.pic.blendTex(P.x,P.y,0.5f,0.5f);
    M.pic.blendTex(M.x,M.y,0.5f,0.5f);
    CheckError(__LINE__);//print line number if GL occurs error
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
void setKeyStateDown(unsigned char key,int x,int y)
{
    switch(key)
    {
        case 'q':case 'Q':
            exit(0);
            break;
    }
}
void init(){
	glutDisplayFunc(display);
    glutReshapeFunc(reshape);	
	glShadeModel(GL_SMOOTH);
    glutKeyboardFunc(setKeyStateDown);

	glClearColor(1.0f,1.0f,0.4f,1.0);
}

void update(int n){
    tobacco.x+=20;
    glutTimerFunc(1000,update,n++);
    glutPostRedisplay();
}

int main(int argc,char **argv){/*{{{*/
	srand(time(NULL));
	tableContent = EMPTY;
	thread myThread[THREAD_NUM];

    glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(screenWidth,screenHeight);
	glutCreateWindow("OS_FinalProject");
	init();

	createThread(myThread);
	this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
	agSem.signal();

    tobacco.pic.readBMPFile("pic/Tobacco.bmp");
    tobacco.pic.setChromaKey(255,255,255);

    paper.pic.readBMPFile("pic/Paper.bmp");
    paper.pic.setChromaKey(255,255,255);

    match.pic.readBMPFile("pic/Match.bmp");
    match.pic.setChromaKey(255,255,255);

    agent.pic.readBMPFile("pic/Agent.bmp");
    agent.pic.setChromaKey(255,255,255);

    T.pic.readBMPFile("pic/T.bmp");
    T.pic.setChromaKey(255,255,255);

    P.pic.readBMPFile("pic/P.bmp");
    P.pic.setChromaKey(255,255,255);

    M.pic.readBMPFile("pic/M.bmp");
    M.pic.setChromaKey(255,255,255);
    update(0);
    glutMainLoop();
	/*myThread[0].join();
		myThread[1].join();
		myThread[2].join();
		myThread[3].join();
		myThread[4].join();
		myThread[5].join();*/
}/*}}}*/

void createThread(thread myThread[THREAD_NUM]){/*{{{*/
	myThread[0] = thread(smkrTobacco);
	myThread[1] = thread(smkrPaper);
	myThread[2] = thread(smkrMatch);
	myThread[3] = thread(agTobacco);
	myThread[4] = thread(agPaper);
	myThread[5] = thread(agMatch);
}/*}}}*/

void cleanTable(){/*{{{*/
	tableContent = 0;
}/*}}}*/


/*What do smoker threads do?
 *(1)wait until there are two resources on the table
 *(2)acquire lck2 with mtx2 to prevent agent entering its critical section when the smoker is doing its job => race condition
 *(3)come to table(i.e.,thread sleep) for "smokerComingTime"
 *(4)decide "smokingTime" and smoke(i.e.,sleep thread again)
 *(5)clean the table and wake up the agents
 * */

void smkrTobacco(){/*{{{*/
	while(1){
		if(tableContent == (PAPER + MATCH)){
			unique_lock<mutex> lck(mtx2);
            cout<<"1"<<endl;
			//this_thread::sleep_for(chrono::seconds(smokerComingTime));
			this_thread::sleep_for(chrono::milliseconds(smokerComingTime));
            //for(long long i=0;i<smokerComingTime*waitingDelta;i++);
            cout<<"HI1"<<endl;
			smokingTime = distribution(generator);
			cout <<"smkrTobacco is smoking for " <<smokingTime <<"seconds." <<endl;
			//this_thread::sleep_for(chrono::seconds(smokingTime));
			this_thread::sleep_for(chrono::milliseconds(smokingTime));
            //for(long long i=0;i<smokerComingTime*waitingDelta;i++);
			cout <<"Finish" <<endl;
			cout <<"Clean the table" <<endl;
			cleanTable();
			cout <<"Wake up the agent" <<endl <<endl;
//			this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
			agSem.signal();
		}
	}
}/*}}}*/
void smkrPaper(){/*{{{*/
	while(1){
		if(tableContent == (TABACCO + MATCH)){
			unique_lock<mutex> lck(mtx2);
            cout<<"2"<<endl;
			//this_thread::sleep_for(chrono::seconds(smokerComingTime));
			this_thread::sleep_for(chrono::milliseconds(smokerComingTime));
            //for(long long i=0;i<smokerComingTime*waitingDelta;i++);
            cout<<"HI2"<<endl;
			smokingTime = distribution(generator);
			cout <<"smkrPaper is smoking for " <<smokingTime <<"seconds." <<endl;
			//this_thread::sleep_for(chrono::seconds(smokingTime));
			this_thread::sleep_for(chrono::milliseconds(smokingTime));
            //for(long long i=0;i<smokerComingTime*waitingDelta;i++);
			cout <<"Finish" <<endl;
			cout <<"Clean the table" <<endl;
			cleanTable();
			cout <<"Wake up the agent" <<endl <<endl;
//			this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
			agSem.signal();
		}
	}
}/*}}}*/
void smkrMatch(){/*{{{*/
	while(1){
		if(tableContent == (TABACCO + PAPER)){
			unique_lock<mutex> lck(mtx2);
            cout<<"3"<<endl;
			//this_thread::sleep_for(chrono::seconds(smokerComingTime));
			this_thread::sleep_for(chrono::milliseconds(smokerComingTime));
            //for(long long i=0;i<smokerComingTime*waitingDelta;i++);
            cout<<"HI3"<<endl;
			smokingTime = distribution(generator);
			cout <<"smkrMatch is smoking for " <<smokingTime <<"seconds." <<endl;
//			this_thread::sleep_for(chrono::seconds(smokingTime));
			this_thread::sleep_for(chrono::milliseconds(smokingTime));
            //for(long long i=0;i<smokerComingTime*waitingDelta;i++);
			cout <<"Finish" <<endl;
			cout <<"Clean the table" <<endl;
			cleanTable();
			cout <<"Wake up the agent" <<endl <<endl;
//			this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
			agSem.signal();
		}
	}
}/*}}}*/


/*What do agent threads do?
 *(1)acquire lck with mtx to ensure that no other agents are in critical section
 *(2)wait for smoker's signal to supply resource.
 *(3)After signal,acquire lck2 with mtx2 to prevent smoker entering its critical section
 *   when the agent finished supply ( tableContent += xxx ) but didn't make smokerComingTime. => race condition
 *(4)if any other agent put something on table, the second agent has to decide "smokerComingTime" before putting its own
 resource on the table.
 * */

void agTobacco(){/*{{{*/
	while(1){
		agSem.wait();
		unique_lock<mutex> lck(mtx);
		unique_lock<mutex> lck2(mtx2);
		if(tableContent != EMPTY){
			smokerComingTime = distribution(generator);
		}
		tableContent += TABACCO;
		cout <<"sup Tobacco.\n";
		if(tableContent == TABACCO || tableContent == PAPER || tableContent == MATCH){
			agSem.signal();
		}
		if(tableContent != TABACCO && tableContent != PAPER && tableContent != MATCH){
			cout <<"smokerComingTime = " <<smokerComingTime <<endl;
            smkrSem.signal(3);

		}
		lck.unlock();
	//	this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
	}
}/*}}}*/
void agPaper(){/*{{{*/
	while(1){
		agSem.wait();
		unique_lock<mutex> lck(mtx);
		unique_lock<mutex> lck2(mtx2);
		if(tableContent != EMPTY){
			smokerComingTime = distribution(generator);
		}
		tableContent += PAPER;
		cout <<"sup Paper.\n";
		if(tableContent == TABACCO || tableContent == PAPER || tableContent == MATCH){
			agSem.signal();
		}
		if(tableContent != TABACCO && tableContent != PAPER && tableContent != MATCH){
			cout <<"smokerComingTime = " <<smokerComingTime <<endl;
            smkrSem.signal(3);
		}
		lck.unlock();
	//	this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
	}
}/*}}}*/
void agMatch(){/*{{{*/
	while(1){
		agSem.wait();
		unique_lock<mutex> lck(mtx);
		unique_lock<mutex> lck2(mtx2);
		if(tableContent != EMPTY){
			smokerComingTime = distribution(generator);
		}
		tableContent += MATCH;
		cout <<"sup Match.\n";
		if(tableContent == TABACCO || tableContent == PAPER || tableContent == MATCH){
			agSem.signal();
		}
		if(tableContent != TABACCO && tableContent != PAPER && tableContent != MATCH){
			cout <<"smokerComingTime = " <<smokerComingTime <<endl;
            smkrSem.signal(3);
		}
		lck.unlock();
	//	this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
	}
}/*}}}*/


