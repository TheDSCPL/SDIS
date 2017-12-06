#include <iostream>
#include <unistd.h>
#include "headers/Thread.hpp"
#include "headers/TCPserver.hpp"

using namespace std;

Mutex m;

void clientHandler(Connection& conn) {
    //cout << "Client Handler of socket " << conn.socket << endl;
    //conn << "You are socket " << conn.socket << endl;
    TCPServer::write(conn.socket,string("You are socket ") + conn.socket + "\n");
    //char receive;
    string receive;
    do {
        try {
            receive = conn.readLine();
            //conn >> receive;
        } catch (ReadError& e) {
            cerr << e.what() << endl << e.error << endl;
            break;
        }

        //cerr << "here1" << endl;
        if(receive=="\\quit")
            break;
        //cerr << (int)receive << endl << "here2" << endl;
        m.lock();
        cout << "Socket " << conn.socket << ": " << receive << endl;
        conn.getServer().sendToAll(string("Socket ")+ conn.socket + ": "+receive+"\n", {&conn});
        cout.flush();
        m.unlock();
    } while(true);
}

int main() {
//    if(!Thread::setThisThreadsRunningCore(3))
//        return 1;
//    for(;;);

    TCPServer server(12345, clientHandler);

    server.join();

    return 0;
}