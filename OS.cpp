#include<iostream>
#include<thread>
#include"SemaphoreClass.h"
using namespace std;
#define THREAD_NUM 6

Semaphore agSem(1);
Semaphore tobaccoSem(0);
Semaphore paperSem(0);
Semaphore matchSem(0);

void createThread(thread[]);
void smkrTobacco(); //function for smoker with tobacco
void smkrPaper(); //function for smoker with paper
void smkrMatch(); //function for smoker with match
void agA(); //function for supplied of tobacco and paper.
void agB(); //function for supplied of paper and match.
void agC(); //function for supplied of tobacco and match.

int main(){
	thread myThread[THREAD_NUM];
	createThread(myThread);
	for(int i=0; i<THREAD_NUM; i++){
		myThread[i].join();
	}
}

void createThread(thread myThread[THREAD_NUM]){
	myThread[0] = thread(smkrTobacco);
	myThread[1] = thread(smkrPaper);
	myThread[2] = thread(smkrMatch);
	myThread[3] = thread(agA);
	myThread[4] = thread(agB);
	myThread[5] = thread(agC);
}

void smkrTobacco(){
	while(1){
	paperSem.wait();
	matchSem.wait();
	cout <<"smoker with tobacco makes a cigarette.\n";
	agSem.signal();
	}
}
void smkrPaper(){
	while(1){
		tobaccoSem.wait();
		matchSem.wait();
		cout <<"smoker with paper makes a cigarette.\n";
		agSem.signal();
	}
}
void smkrMatch(){
	while(1){
		tobaccoSem.wait();
		paperSem.wait();
		cout <<"smoker with match makes a cigarette.\n";
		agSem.signal();
	}
}
void agA(){
	while(1){
		agSem.wait();
		tobaccoSem.signal();
		paperSem.signal();
	}
}
void agB(){
	while(1){
		agSem.wait();
		paperSem.signal();
		matchSem.signal();
	}
}
void agC(){
	while(1){
		agSem.wait();
		matchSem.signal();
		tobaccoSem.signal();
	}
}
