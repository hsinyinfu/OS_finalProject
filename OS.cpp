#include<iostream>
#include<thread>
using namespace std;

int tobaLock=0;
int papLock=0;
int matLock=0;

void smkrTobacco();
void smkrPaper();
void smkrMatch();
void toAgnt();
void paAgnt();
void maAgnt();

int main(){
	thread skTbThread(smkrTobacco);
	thread skPpThread(smkrPaper);
	thread skMchThread(smkrMatch);
	thread toThread(toAgnt);
	thread paThread(paAgnt);
	thread maThread(maAgnt);
}
