#include "mainwindow.h"
#include <QApplication>
#include<iostream>
#include<stdlib.h>
#include<thread>
#include<mutex>
#include<random>
#include<chrono>
#include<QThread>
#include<condition_variable>
#include<QWidget>

using namespace std;

#define THREAD_NUM 6
#define TOBACCO 1
#define PAPER 3
#define MATCH 5
#define MEAN 2 //for poisson random variable
#define EMPTY 0
#define TIME_FOR_ADJUSTMENT 100

MainWindow* gui;

class Semaphore
{
    public:
        Semaphore(int count_ = 0) : count(count_){}

        /*leaving critical section and wake up someone who is waiting*/
        void signal()
        {
            std::unique_lock<std::mutex> lck(mtx);	//acquire lock
            ++count;
            cv.notify_one();	//It wakes up another one sleeping
        }

        void signal(int n)
        {
            std::unique_lock<std::mutex> lck(mtx);
            count += n;
            cv.notify_all();
        }
        /*waiting for enter critical section*/
        void wait()
        {
            std::unique_lock<std::mutex> lck(mtx);	//acquire lock
            while(count == 0)	//if count==0,which means someone is in its
            {					//critical section,so that others have to
                cv.wait(lck);	//wait.When they in this line,they release
            }					//the lock.

            --count;			//If someone is notified so it leave the
        }						//while-loop,they reacquire the lock and
                                //count--.

    private:
        std::mutex mtx;
        std::condition_variable cv;
        int count;
};

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

class smkrTobacco : public QThread{
public:
    smkrTobacco(){}
protected:
    void run(){
        while(1){
            if(tableContent == (PAPER + MATCH)){
                unique_lock<mutex> lck(mtx2);
                this_thread::sleep_for(chrono::seconds(smokerComingTime));
                gui -> action( PAPER + MATCH);
                smokingTime = distribution(generator);
                cout <<"smkrTobacco is smoking for " <<smokingTime <<"seconds." <<endl;
                this_thread::sleep_for(chrono::seconds(smokingTime));
                gui -> reset();
                cout <<"Finish" <<endl;
                cout <<"Clean the table" <<endl;
                cleanTable();
                cout <<"Wake up the agent" <<endl <<endl;
                agSem.signal();
            }
        }
    }
};

class smkrPaper : public QThread{
public:
    smkrPaper(){}
protected:
    void run(){
        while(1){
            if(tableContent == (TOBACCO + MATCH)){
                unique_lock<mutex> lck(mtx2);
                this_thread::sleep_for(chrono::seconds(smokerComingTime));
                gui -> action( TOBACCO + MATCH);
                smokingTime = distribution(generator);
                cout <<"smkrPaper is smoking for " <<smokingTime <<"seconds." <<endl;
                this_thread::sleep_for(chrono::seconds(smokingTime));
                gui -> reset();
                cout <<"Finish" <<endl;
                cout <<"Clean the table" <<endl;
                cleanTable();
                cout <<"Wake up the agent" <<endl <<endl;
                agSem.signal();
            }
        }
    }
};

class smkrMatch : public QThread{
public:
    smkrMatch(){}
protected:
    void run(){
        while(1){
            if(tableContent == (TOBACCO + PAPER)){
                unique_lock<mutex> lck(mtx2);
                this_thread::sleep_for(chrono::seconds(smokerComingTime));
                gui -> action( TOBACCO + PAPER);
                smokingTime = distribution(generator);
                cout <<"smkrMatch is smoking for " <<smokingTime <<"seconds." <<endl;
                this_thread::sleep_for(chrono::seconds(smokingTime));
                gui -> reset();
                cout <<"Finish" <<endl;
                cout <<"Clean the table" <<endl;
                cleanTable();
                cout <<"Wake up the agent" <<endl <<endl;
                agSem.signal();
            }
        }
    }
};


/*What do agent threads do?
 *(1)acquire lck with mtx to ensure that no other agents are in critical section
 *(2)wait for smoker's signal to supply resource.
 *(3)After signal,acquire lck2 with mtx2 to prevent smoker entering its critical section
 *   when the agent finished supply ( tableContent += xxx ) but didn't make smokerComingTime. => race condition
 *(4)if any other agent put something on table, the second agent has to decide "smokerComingTime" before putting its own
 resource on the table.
 * */

class agTobacco : public QThread{
public:
    agTobacco(){}
protected:
    void run(){
        while(1){
            agSem.wait();
            unique_lock<mutex> lck(mtx);
            unique_lock<mutex> lck2(mtx2);
            if(tableContent != EMPTY){
                smokerComingTime = distribution(generator);
            }
            tableContent += TOBACCO;
            cout <<"sup Tobacco.\n";
            gui -> action(TOBACCO);
            if(tableContent == TOBACCO || tableContent == PAPER || tableContent == MATCH){
                this_thread::sleep_for(chrono::seconds(1));
                agSem.signal();
            }
            if(tableContent != TOBACCO && tableContent != PAPER && tableContent != MATCH){
                cout <<"smokerComingTime = " <<smokerComingTime <<endl;
                smkrSem.signal(3);
            }
            lck.unlock();
            this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
        }
    }
};

class agPaper : public QThread{
public:
    agPaper(){}
protected:
    void run(){
        while(1){
            agSem.wait();
            unique_lock<mutex> lck(mtx);
            unique_lock<mutex> lck2(mtx2);
            if(tableContent != EMPTY){
                smokerComingTime = distribution(generator);
            }
            tableContent += PAPER;
            cout <<"sup Paper.\n";
            gui -> action(PAPER);
            if(tableContent == TOBACCO || tableContent == PAPER || tableContent == MATCH){
                this_thread::sleep_for(chrono::seconds(1));
                agSem.signal();
            }
            if(tableContent != TOBACCO && tableContent != PAPER && tableContent != MATCH){
                cout <<"smokerComingTime = " <<smokerComingTime <<endl;
                smkrSem.signal(3);
            }
            lck.unlock();
            this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
        }
    }
};

class agMatch : public QThread{
public:
    agMatch(){}
protected:
    void run(){
        while(1){
            agSem.wait();
            unique_lock<mutex> lck(mtx);
            unique_lock<mutex> lck2(mtx2);
            if(tableContent != EMPTY){
                smokerComingTime = distribution(generator);
            }
            tableContent += MATCH;
            cout <<"sup Match.\n";
            gui -> action(MATCH);
            if(tableContent == TOBACCO || tableContent == PAPER || tableContent == MATCH){
                this_thread::sleep_for(chrono::seconds(1));
                agSem.signal();
            }
            if(tableContent != TOBACCO && tableContent != PAPER && tableContent != MATCH){
                cout <<"smokerComingTime = " <<smokerComingTime <<endl;
                smkrSem.signal(3);
            }
            lck.unlock();
            this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QObject::tr("Tobacco Smoker Problem")); //設定窗口標題
    gui = &w;
    gui -> reset();
    w.show();

    srand(time(NULL));
    tableContent = EMPTY;

    agTobacco *AgTobacco = new agTobacco();
    agPaper *AgPaper = new agPaper();
    agMatch *AgMatch = new agMatch();
    smkrTobacco *SmkrTobacco = new smkrTobacco();
    smkrPaper *SmkrPaper = new smkrPaper();
    smkrMatch *SmkrMatch = new smkrMatch();

    AgTobacco -> start();
    AgPaper -> start();
    AgMatch -> start();
    SmkrTobacco -> start();
    SmkrPaper -> start();
    SmkrMatch -> start();

    this_thread::sleep_for(chrono::milliseconds(TIME_FOR_ADJUSTMENT));
    agSem.signal();


    return a.exec();
}
