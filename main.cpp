#include <iostream>
#include <unistd.h>
#include <fstream>
#include "headers/Thread.hpp"
#include "headers/Network/TCPserver.hpp"
#include "headers/ParallelProcesses/ParallelizableProcess.h"
#include "headers/SHA512.h"
#include "headers/MD5.hpp"
#include "headers/ParallelProcesses/HMACParallelProcess.hpp"
#include "headers/ParallelProcesses/PrimesParallelProcess.hpp"
#include "headers/Network/SDISServer.hpp"

using namespace std;

Mutex m;

void clientHandler(Connection& conn) {
    //cout << "SDISServer Handler of socket " << conn.socket << endl;
    conn << "You are socket " << conn.getSocket() << "\n";
    //TCPServer::write(conn.socket,string("You are socket ") + conn.socket + "\n");
    //char receive;
    string receive;
    do {
        try {
            receive = conn.readLine();
            //conn >> receive;
        } catch (ReadError& e) {
            if(e.error==EBADF)
                break;
            cerr << e.what() << endl << e.error << endl;
            break;
        } catch (ConnectionEOF& e) {
            conn.close();
            break;
        }

        //cerr << "here1" << endl;
        if(receive=="\\quit")
            break;
        //cerr << (int)receive << endl << "here2" << endl;
        m.lock();
        cout << "Socket " << conn.getSocket() << ": " << receive << endl;
        conn.getServer().sendToAll(string("Socket ")+ conn.getSocket() + ": "+receive+"\n", {&conn});
        cout.flush();
        m.unlock();
    } while(true);
}

class Process1 : public ParallelizableProcess {
public:
    Process1() {
        SimpleTask* A = new SimpleTask(this,[](){
            for(int i=0;i<10;i++) {
                cout<<"A:"<<i<<endl;
                Thread::usleep(300*1000);
            }
        });
        SimpleTask* B = new SimpleTask(this,[](){
            for(int i=0;i<10;i++) {
                cout<<"B:"<<i<<endl;
                Thread::usleep(300*1000);
            }
        },{A});
        SimpleTask* C = new SimpleTask(this,[](){
            for(int i=0;i<10;i++) {
                cout<<"C:"<<i<<endl;
                Thread::usleep(300*1000);
            }
        },{A});
        SimpleTask* D = new SimpleTask(this,[](){
            for(int i=0;i<5;i++) {
                cout<<"D:"<<i<<endl;
                Thread::usleep(250*1000);
            }
        },{A});
        SimpleTask* E = new SimpleTask(this,[](){
            for(int i=0;i<10;i++) {
                cout<<"E:"<<i<<endl;
                Thread::usleep(300*1000);
            }
        },{D});
        SimpleTask* F = new SimpleTask(this,[](){
            for(int i=0;i<10;i++) {
                cout<<"F:"<<i<<endl;
                Thread::usleep(300*1000);
            }
        },{D});
        SimpleTask* G = new SimpleTask(this,[](){
            for(int i=0;i<10;i++) {
                cout<<"G:"<<i<<endl;
                Thread::usleep(300*1000);
            }
        },{B,C,E,F});
        SimpleTask* H = new SimpleTask(this,[](){
            for(int i=0;i<10;i++) {
                cout<<"H:"<<i<<endl;
                Thread::usleep(300*1000);
            }
        },{B,C,E,F});
        entryTasks.insert(A);
        exitTasks.insert({G,H});
    }

    void onReady() final {
        cout << "Process1 finished" << endl;
    }
};

int main() {
    std::ofstream log_file;
    log_file.open("SDIS_proj.log", ofstream::out | ofstream::app);
    clog.rdbuf(log_file.rdbuf());
//    PrimesParallelProcess primes(1000000);
//    primes.run();
//    primes.join();

    SDISServer server;
    server.join();
    /*TCPServer server("192.168.2.12",12345, clientHandler);

    server.join();*/

//    Process1 p;
//    cout << p.isRunning() << endl;
//    p.run();
//    Thread::usleep(1000);
//    cout << p.isRunning() << endl;
//
//    p.join();

    //cout << bytes2HexString(SHA512("ola"))<<endl;
    //std::vector<byte> t = concat({'a','b','c'},{'a','b','c'});
    //for(char c : t)
    //    cout << c;
    //cout << endl;

    string key = "ola",message="message";

    //cout << bytes2HexString( SHA512( concat( outputPadSHA512(key) , SHA512( concat( inputPadSHA512(key) , message ) ) ) ) ) << endl;

    //cout << bytes2HexString(MD5("ola")) << endl;

    /*for(auto i : key) {
        cout << (int)i << " ";
    }
    cout << endl;

    auto t = MD5(key);
    for(auto i : t) {
        cout << (int)i << " ";
    }
    cout << endl;

    t = inputPadMD5(key);
    for(auto i : t) {
        cout << (int)i << " ";
    }
    cout << endl;

    t = concat( inputPadMD5(key) , message );
    for(auto i : t) {
        cout << (int)i << " ";
    }
    cout << endl;*/

    /*SHA512ParallelProcess sha512ParallelProcess(key,message);

    //cout << bytes2HexString( MD5( concat( outputPadMD5(key) , MD5( concat( inputPadMD5(key) , message ) ) ) ) ) << endl;
    struct timespec startTime={0},endTime={0};
    clock_gettime(CLOCK_MONOTONIC,&startTime);
    cout << bytes2HexString( SHA512( concat( outputPadSHA512(key) , SHA512( concat( inputPadSHA512(key) , message ) ) ) ) ) << endl;
    clock_gettime(CLOCK_MONOTONIC,&endTime);
    cout << (endTime.tv_sec-startTime.tv_sec)*1000000000UL + endTime.tv_nsec - startTime.tv_nsec << " nanoseconds" << endl;

    HMACSHA512ParallelProcessSimplified sha512ParallelProcessSimplified(key,message);

    startTime={0};
    endTime={0};
    clock_gettime(CLOCK_MONOTONIC,&startTime);
    sha512ParallelProcessSimplified.run();
    sha512ParallelProcessSimplified.join();
    clock_gettime(CLOCK_MONOTONIC,&endTime);
    cout << (endTime.tv_sec-startTime.tv_sec)*1000000000UL + endTime.tv_nsec - startTime.tv_nsec << " nanoseconds" << endl;

    startTime={0};
    endTime={0};
    clock_gettime(CLOCK_MONOTONIC,&startTime);
    sha512ParallelProcess.run();
    sha512ParallelProcess.join();
    clock_gettime(CLOCK_MONOTONIC,&endTime);
    cout << (endTime.tv_sec-startTime.tv_sec)*1000000000UL + endTime.tv_nsec - startTime.tv_nsec << " nanoseconds" << endl;*/

    //inputPadSHA512("abcdefg");

//    Thread a([](){for(;;);});
//    a.start();
//    cout << (a.setRunningCore(0)?"true":"false") << endl;
//    a.join();

    return 0;
}