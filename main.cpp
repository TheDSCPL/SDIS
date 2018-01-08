#include <iostream>
#include <unistd.h>
#include "headers/Thread.hpp"
#include "headers/TCPserver.hpp"
#include "headers/ParallelizableProcess.h"

using namespace std;

Mutex m;

void clientHandler(Connection& conn) {
    //cout << "Client Handler of socket " << conn.socket << endl;
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

    void onReady() override final {
        cout << "Process1 finished" << endl;
    }
};

int main() {
    /*TCPServer server("192.168.2.12",12345, clientHandler);

    server.join();*/

    Process1 p;
    p.run();

    Thread:usleep(100*1000*1000);

//    Thread a([](){for(;;);});
//    a.start();
//    cout << (a.setRunningCore(0)?"true":"false") << endl;
//    a.join();

    return 0;
}