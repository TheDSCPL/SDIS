#include "../../headers/Network/SDISServer.hpp"
#include "../../headers/ParallelProcesses/PrimesParallelProcess.hpp"
#include "../../headers/SHA512.h"
#include "../../headers/ParallelProcesses/HMACParallelProcess.hpp"
#include "../../headers/MD5.hpp"

SDISServer::SDISServer(unsigned int port) :
        TCPServer(port,[this](Connection& c){handler(c);}),
        port(port) {}

void SDISServer::handler(Connection &c) {
    do {
        c << "Select the operation (0 for help): ";
        int i;
        bool invalidInput=false;
        try {
            c >> i;
        } catch(std::logic_error& e) {
            invalidInput=true;
        }
        if(invalidInput) {
            c << "You've inserted an invalid character (insert integers only)" << "\n";
            continue;
        }
        if(i<0) {
            c << "Are you sure you want to close the connection? (Y/n)";
            char response;
            try {
                do{
                    c >> response;
                } while(response==' '||response=='\n'||response=='\r'||response=='\t');
            } catch (std::logic_error& e) {continue;}
            if(response=='Y'||response=='y')
                break;
            continue;
        }
        switch(i) {
            case 0:
                displayHelpMenu(c);
            case 1: {
                c << "---Computing HMAC-SHA512 without FSM---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                Timer t;
                t.start();
                c << bytes2HexString( SHA512( concat( outputPadSHA512(key) , SHA512( concat( inputPadSHA512(key) , message ) ) ) ) ) << "\n";
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 2: {
                c << "---Computing HMAC-SHA512 in series---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACSHA512SeriesProcess hmacSHA512SeriesProcess(&c,key,message);
                Timer t;
                t.start();
                hmacSHA512SeriesProcess.run();
                hmacSHA512SeriesProcess.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 3: {
                c << "---Computing HMAC-SHA512 in parallel---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACSHA512ParallelProcess hmacSHA512ParallelProcess(&c,key,message);
                Timer t;
                t.start();
                hmacSHA512ParallelProcess.run();
                hmacSHA512ParallelProcess.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 4: {
                c << "---Computing HMAC-SHA512 in parallel (simplified)---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACSHA512ParallelProcessSimplified hmacSHA512ParallelProcessSimplified(&c,key,message);
                Timer t;
                t.start();
                hmacSHA512ParallelProcessSimplified.run();
                hmacSHA512ParallelProcessSimplified.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 5: {
                c << "---Computing HMAC-MD5 in without FSM---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                Timer t;
                t.start();
                c << bytes2HexString( MD5( concat( outputPadMD5(key) , MD5( concat( inputPadMD5(key) , message ) ) ) ) ) << "\n";
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 6: {
                c << "---Computing HMAC-MD5 in series---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACMD5SeriesProcess hmacMD5SeriesProcess(&c,key,message);
                Timer t;
                t.start();
                hmacMD5SeriesProcess.run();
                hmacMD5SeriesProcess.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 7: {
                c << "---Computing HMAC-MD5 in parallel---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACMD5ParallelProcess hmacMD5ParallelProcess(&c,key,message);
                Timer t;
                t.start();
                hmacMD5ParallelProcess.run();
                hmacMD5ParallelProcess.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 8: {
                c << "---Computing HMAC-MD5 in parallel (simplified)---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACMD5ParallelProcessSimplified hmacMD5ParallelProcessSimplified(&c,key,message);
                Timer t;
                t.start();
                hmacMD5ParallelProcessSimplified.run();
                hmacMD5ParallelProcessSimplified.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 9: {
                c << "---Computing primes without FSM---" << "\n";
                int number;
                do {
                    try {
                        c >> number;
                    } catch (std::logic_error &e) {
                        invalidInput = true;
                    }
                    if (invalidInput || number <= 0) {
                        c << "You've inserted an invalid character (insert positive integers only)" << "\n";
                        continue;
                    }
                } while (invalidInput);
                PrimesParallelProcess primesParallelProcess(&c,number);
                Timer t;
                t.start();
                primesParallelProcess.run();
                primesParallelProcess.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 10: {
                c << "---Computing primes series---" << "\n";
                int number;
                do {
                    try {
                        c >> number;
                    } catch (std::logic_error &e) {
                        invalidInput = true;
                    }
                    if (invalidInput || number <= 0) {
                        c << "You've inserted an invalid character (insert positive integers only)" << "\n";
                        continue;
                    }
                } while (invalidInput);
                PrimesParallelProcess primesParallelProcess(&c,number);
                Timer t;
                t.start();
                primesParallelProcess.run();
                primesParallelProcess.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            case 11: {
                c << "---Computing primes parallel---" << "\n";
                int number;
                do {
                    try {
                        c >> number;
                    } catch (std::logic_error &e) {
                        invalidInput = true;
                    }
                    if (invalidInput || number <= 0) {
                        c << "You've inserted an invalid character (insert positive integers only)" << "\n";
                        continue;
                    }
                } while (invalidInput);
                PrimesParallelProcess primesParallelProcess(&c,number);
                Timer t;
                t.start();
                primesParallelProcess.run();
                primesParallelProcess.join();
                t.end();
                c << "(took " << t.getTime() << "ns)" << "\n\n";
            }
                break;
            default:
                c << "You've selected " << i << " but that option doesn't exist." << "\n";
                displayHelpMenu(c);
        }
    } while(true);
}

void SDISServer::displayHelpMenu(Connection &c) {
    c << "Available options:" << "\n";
    c << "<0 -> Finish Connection" << "\n";
    c << " 1 -> Compute HMAC-512 (without FSM)" << "\n";
    c << " 2 -> Compute HMAC-512 (serie)" << "\n";
    c << " 3 -> Compute HMAC-512 (parallel)" << "\n";
    c << " 4 -> Compute HMAC-512 (parallel simplified)" << "\n";
    c << " 5 -> Compute HMAC-MD5 (without FSM)" << "\n";
    c << " 6 -> Compute HMAC-MD5 (serie)" << "\n";
    c << " 7 -> Compute HMAC-MD5 (parallel)" << "\n";
    c << " 8 -> Compute HMAC-MD5 (parallel simplified)" << "\n";
    c << " 9 -> Compute primes (without FSM)" << "\n";
    c << "10 -> Compute primes series" << "\n";
    c << "11 -> Compute primes parallel" << "\n";
}