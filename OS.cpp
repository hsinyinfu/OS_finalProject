#include<iostream>
#include<thread>
#include<mutex>
#include"SemaphoreClass.h"
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

int round = 0;
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

int main(){
	thread myThread[THREAD_NUM];
	cout <<"How many round?";
	cin >> round;

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
	myThread[6] = thread(pusherA);
	myThread[7] = thread(pusherB);
	myThread[8] = thread(pusherC);
}

void smkrTobacco(){
	while(1){
		tobaccoSem.wait();
		if(passed < round){
			cout <<"tobacco\n";
			passed++;
		}
		if(passed == round)
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
		if(passed < round){
			cout <<"paper\n";
			passed++;
		}
		if(passed == round)
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
		if(passed < round){
			cout <<"match\n";
			passed++;
		}
		if(passed == round)
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
		if(passed < round)
			cout <<"sup to pa.\n";
		tobacco.signal();
		paper.signal();
		if( isEnd )
			break;
	}
}
void agB(){
	while(1){
		agSem.wait();
		if(passed < round)
			cout <<"sup pa ma.\n";
		paper.signal();
		match.signal();
		if( isEnd )
			break;
	}
}
void agC(){
	while(1){
		agSem.wait();
		if(passed < round)
			cout <<"sup ma to.\n";
		match.signal();
		tobacco.signal();
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
