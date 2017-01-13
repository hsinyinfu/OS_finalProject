#include<iostream>
#include<stdlib.h>
#include<thread>
#include<mutex>
#include<random>
#include<chrono>
#include"SemaphoreClass.h"

using namespace std;
#define THREAD_NUM 6
#define TABACCO 1
#define PAPER 3
#define MATCH 5
#define MEAN 4
#define EMPTY 0

mutex mtx;	//lock which is used to prevent race condition between agents.
mutex mtx2; //lock which is used to prevent race condition between agents and smoker who can access the table.
int tableContent = EMPTY;
Semaphore agSem(0);		/*Semaphore used to wake up one of the Agents to suppy ingredients.*/
Semaphore table(0);
int smokerComingTime;
int smokingTime;
default_random_engine generator;
poisson_distribution<int> distribution(MEAN);

void createThread(thread[]);
void cleanTable();
void smkrTobacco(); //function of smoker with tobacco
void smkrPaper(); //function of smoker with paper
void smkrMatch(); //function of smoker with match
void agTobacco(); //function for supplied of tobacco and paper.
void agPaper(); //function for supplied of paper and match.
void agMatch(); //function for supplied of tobacco and match.

int main(int argc,char **argv){
	srand(time(NULL));
	thread myThread[THREAD_NUM];
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
