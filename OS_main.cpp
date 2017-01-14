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

mutex mtx;	/*lock which is used to prevent race condition between agents.*/
mutex mtx2; /*lock which is used to prevent race condition between agents and smoker who can access the table.*/
Semaphore agSem(0);	/*Semaphore used to wake up one of the Agents to suppy ingredients.*/

/*
 * Share variable
 * */
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

/*
 * Thread related function declaration
 * */
void cleanTable();
void createThread(thread[]);
void smkrTobacco(); //function of smoker with tobacco
void smkrPaper(); //function of smoker with paper
void smkrMatch(); //function of smoker with match
void agTobacco(); //function for supplied of tobacco.
void agPaper(); //function for supplied of paper.
void agMatch(); //function for supplied of match.

/*
 * openGL related function declaration
 * */
void display();
static void CheckError(int);
void reshape(int,int);
void setKeyStateDown(unsigned char,int,int);
void action(int);
void init();

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


/*
 * Object initialization
 * */
Creature personWithTobacco(100,450);
Creature personWithPaper(100,250);
Creature personWithMatch(100,50);

/*
 * resource below
 * */
Creature rTobacco(-100,450);
Creature rPaper(-100,250);
Creature rMatch(-100,50);


int main(int argc,char **argv){
	srand(time(NULL));
	tableContent = EMPTY;
	thread myThread[THREAD_NUM];

	/*Initialize GUI*/
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

	/*Start threads*/
	createThread(myThread);
	agSem.signal();
	agSem.signal();
	myThread[0].join();
	myThread[1].join();
	myThread[2].join();
	myThread[3].join();
	myThread[4].join();
	myThread[5].join();
}

void createThread(thread myThread[THREAD_NUM]){
	myThread[0] = thread(smkrTobacco);
	myThread[1] = thread(smkrPaper);
	myThread[2] = thread(smkrMatch);
	myThread[3] = thread(agTobacco);
	myThread[4] = thread(agPaper);
	myThread[5] = thread(agMatch);
}

void cleanTable(){
	tableContent = 0;
}


/*What do smoker threads do?
 *(1)wait until there are two resources on the table
 *(2)acquire lck2 with mtx2 to prevent agent entering its critical section when the smoker is doing its job => race condition
 *(3)come to table(i.e.,thread sleep) for "smokerComingTime"
 *(4)decide "smokingTime" and smoke(i.e.,sleep thread again)
 *(5)clean the table and wake up the agents
 * */

void smkrTobacco(){
	while(1){
		if(tableContent == (PAPER + MATCH)){
			unique_lock<mutex> lck(mtx2);
			this_thread::sleep_for(chrono::seconds(smokerComingTime));
			smokingTime = distribution(generator);
			cout <<"smkrTobacco is smoking for " <<smokingTime <<"seconds." <<endl;
			this_thread::sleep_for(chrono::seconds(smokingTime));
			cout <<"Finish" <<endl;
			cout <<"Clean the table" <<endl;
			cleanTable();
			cout <<"Wake up the agent" <<endl <<endl;
			agSem.signal();
			agSem.signal();
		}
	}
}
void smkrPaper(){
	while(1){
		if(tableContent == (TABACCO + MATCH)){
			unique_lock<mutex> lck(mtx2);
			this_thread::sleep_for(chrono::seconds(smokerComingTime));
			smokingTime = distribution(generator);
			cout <<"smkrPaper is smoking for " <<smokingTime <<"seconds." <<endl;
			this_thread::sleep_for(chrono::seconds(smokingTime));
			cout <<"Finish" <<endl;
			cout <<"Clean the table" <<endl;
			cleanTable();
			cout <<"Wake up the agent" <<endl <<endl;
			agSem.signal();
			agSem.signal();
		}
	}
}
void smkrMatch(){
	while(1){
		if(tableContent == (TABACCO + PAPER)){
			unique_lock<mutex> lck(mtx2);
			this_thread::sleep_for(chrono::seconds(smokerComingTime));
			smokingTime = distribution(generator);
			cout <<"smkrMatch is smoking for " <<smokingTime <<"seconds." <<endl;
			this_thread::sleep_for(chrono::seconds(smokingTime));
			cout <<"Finish" <<endl;
			cout <<"Clean the table" <<endl;
			cleanTable();
			cout <<"Wake up the agent" <<endl <<endl;
			agSem.signal();
			agSem.signal();
		}
	}
}


/*What do agent threads do?
 *(1)acquire lck with mtx to ensure that no other agents are in critical section
 *(2)wait for smoker's signal to supply resource.
 *(3)After signal,acquire lck2 with mtx2 to prevent smoker entering its critical section
 *   when the agent finished supply ( tableContent += xxx ) but didn't make smokerComingTime. => race condition
 *(4)if any other agent put something on table, the second agent has to decide "smokerComingTime" before putting its own
 resource on the table.
 * */

void agTobacco(){
	while(1){
		unique_lock<mutex> lck(mtx);
		agSem.wait();
		unique_lock<mutex> lck2(mtx2);
		if(tableContent != EMPTY){
			smokerComingTime = distribution(generator);
		}
		tableContent += TABACCO;
		cout <<"sup Tobacco.\n";
		if(tableContent != TABACCO && tableContent != PAPER && tableContent != MATCH)
			cout <<"smokerComingTime = " <<smokerComingTime <<endl;
	}
}
void agPaper(){
	while(1){
		unique_lock<mutex> lck(mtx);
		agSem.wait();
		unique_lock<mutex> lck2(mtx2);
		if(tableContent != EMPTY){
			smokerComingTime = distribution(generator);
		}
		tableContent += PAPER;
		cout <<"sup Paper.\n";
		if(tableContent != TABACCO && tableContent != PAPER && tableContent != MATCH)
			cout <<"smokerComingTime = " <<smokerComingTime <<endl;
	}
}
void agMatch(){
	while(1){
		unique_lock<mutex> lck(mtx);
		agSem.wait();
		unique_lock<mutex> lck2(mtx2);
		if(tableContent != EMPTY){
			smokerComingTime = distribution(generator);
		}
		tableContent += MATCH;
		cout <<"sup Match.\n";
		if(tableContent != TABACCO && tableContent != PAPER && tableContent != MATCH)
			cout <<"smokerComingTime = " <<smokerComingTime <<endl;
	}
}

/*
 * openGL related function definition
 * */
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);//clean buffer
	//print people and resources' position(following 6 line)
	personWithTobacco.pic.blendTex(personWithTobacco.getX(),personWithTobacco.getY());
	personWithPaper.pic.blendTex(personWithPaper.getX(),personWithPaper.getY());
	personWithMatch.pic.blendTex(personWithMatch.getX(),personWithMatch.getY());
	rTobacco.pic.blendTex(rTobacco.getX(),rTobacco.getY());
	rPaper.pic.blendTex(rPaper.getX(),rPaper.getY());
	rMatch.pic.blendTex(rMatch.getX(),rMatch.getY());
	CheckError(__LINE__);//print line number if GL occurs error
	glutSwapBuffers();
}

static void CheckError(int line)
{
	GLenum err = glGetError();
	if (err)
	{
		printf("GL Error %s (0x%x) at line %d\n",gluErrorString(err), (int) err, line);
	}
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

void action(int Case){
	cout <<"hi" <<endl;
}
