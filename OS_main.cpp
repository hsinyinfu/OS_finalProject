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

void cleanTable();
void createThread(thread[]);
void smkrTobacco(); //function of smoker with tobacco
void smkrPaper(); //function of smoker with paper
void smkrMatch(); //function of smoker with match
void agTobacco(); //function for supplied of tobacco.
void agPaper(); //function for supplied of paper.
void agMatch(); //function for supplied of match.

int main(int argc,char **argv){/*{{{*/
	srand(time(NULL));
	tableContent = EMPTY;
	thread myThread[THREAD_NUM];
	createThread(myThread);
	this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
	agSem.signal();
	myThread[0].join();
	/*	myThread[1].join();
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
			this_thread::sleep_for(chrono::seconds(smokerComingTime));
			this_thread::sleep_for(chrono::milliseconds(smokerComingTime));
			smokingTime = distribution(generator);
			cout <<"smkrTobacco is smoking for " <<smokingTime <<"seconds." <<endl;
			this_thread::sleep_for(chrono::seconds(smokingTime));
			this_thread::sleep_for(chrono::milliseconds(smokingTime));
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
			this_thread::sleep_for(chrono::seconds(smokerComingTime));
			this_thread::sleep_for(chrono::milliseconds(smokerComingTime));
			smokingTime = distribution(generator);
			cout <<"smkrPaper is smoking for " <<smokingTime <<"seconds." <<endl;
			this_thread::sleep_for(chrono::seconds(smokingTime));
			this_thread::sleep_for(chrono::milliseconds(smokingTime));
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
			this_thread::sleep_for(chrono::seconds(smokerComingTime));
			this_thread::sleep_for(chrono::milliseconds(smokerComingTime));
			smokingTime = distribution(generator);
			cout <<"smkrMatch is smoking for " <<smokingTime <<"seconds." <<endl;
			this_thread::sleep_for(chrono::seconds(smokingTime));
			this_thread::sleep_for(chrono::milliseconds(smokingTime));
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
		this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
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
		this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
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
		this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
	}
}/*}}}*/
