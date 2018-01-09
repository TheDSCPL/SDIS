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
            c << "Inseriu um caracter inválido (insira apenas números inteiros)" << "\n";
            continue;
        }
        if(i<0) {
            c << "Tem a certeza que pretende terminar a ligação? (S/N)";
            char response;
            try {
                do{
                    c >> response;
                } while(response==' '||response=='\n'||response=='\r'||response=='\t');
            } catch (std::logic_error& e) {continue;}
            if(response=='S'||response=='s')
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
                c << bytes2HexString( SHA512( concat( outputPadSHA512(key) , SHA512( concat( inputPadSHA512(key) , message ) ) ) ) ) << "\n";
            }
                break;
            case 2: {
                c << "---Computing HMAC-SHA512 in parallel---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACSHA512ParallelProcess hmacSHA512ParallelProcess(&c,key,message);
                hmacSHA512ParallelProcess.run();
                hmacSHA512ParallelProcess.join();
            }
                break;
            case 3: {
                c << "---Computing HMAC-SHA512 in parallel---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACSHA512ParallelProcess hmacSHA512ParallelProcess(&c,key,message);
                hmacSHA512ParallelProcess.run();
                hmacSHA512ParallelProcess.join();
            }
                break;
            case 4: {
                c << "---Computing HMAC-SHA512 in parallel (simplified)---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACSHA512ParallelProcessSimplified hmacSHA512ParallelProcessSimplified(&c,key,message);
                hmacSHA512ParallelProcessSimplified.run();
                hmacSHA512ParallelProcessSimplified.join();
            }
                break;
            case 5: {
                c << "---Computing HMAC-MD5 in series---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                c << bytes2HexString( MD5( concat( outputPadMD5(key) , MD5( concat( inputPadMD5(key) , message ) ) ) ) ) << "\n";
            }
                break;
            case 6: {
                c << "---Computing HMAC-MD5 in series---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                c << bytes2HexString( MD5( concat( outputPadMD5(key) , MD5( concat( inputPadMD5(key) , message ) ) ) ) ) << "\n";
            }
                break;
            case 7: {
                c << "---Computing HMAC-MD5 in parallel---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACMD5ParallelProcess hmacMD5ParallelProcess(&c,key,message);
                hmacMD5ParallelProcess.run();
                hmacMD5ParallelProcess.join();
            }
                break;
            case 8: {
                c << "---Computing HMAC-MD5 in parallel (simplified)---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                HMACMD5ParallelProcessSimplified hmacMD5ParallelProcessSimplified(&c,key,message);
                hmacMD5ParallelProcessSimplified.run();
                hmacMD5ParallelProcessSimplified.join();
            }
                break;
            case 9: {
                c << "---Computing primes series---" << "\n";
                int number;
                do {
                    try {
                        c >> number;
                    } catch (std::logic_error &e) {
                        invalidInput = true;
                    }
                    if (invalidInput || number <= 0) {
                        c << "Inseriu um caracter inválido (insira apenas números inteiros positivos)" << "\n";
                        continue;
                    }
                } while (invalidInput);
                PrimesParallelProcess primesParallelProcess(&c,number);
                primesParallelProcess.run();
                primesParallelProcess.join();
            }
                break;
            case 10: {
                c << "---Computing primes parallel---" << "\n";
                int number;
                do {
                    try {
                        c >> number;
                    } catch (std::logic_error &e) {
                        invalidInput = true;
                    }
                    if (invalidInput || number <= 0) {
                        c << "Inseriu um caracter inválido (insira apenas números inteiros positivos)" << "\n";
                        continue;
                    }
                } while (invalidInput);
                PrimesParallelProcess primesParallelProcess(&c,number);
                primesParallelProcess.run();
                primesParallelProcess.join();
            }
                break;
            default:
                c << "Seleccionou " << i << "\n";
        }
    } while(true);
}

void SDISServer::displayHelpMenu(Connection &c) {
    c << "Available options:" << "\n";
    c << "<0 -> Finish Connection" << "\n";
    c << " 1 -> Compute HMAC (serie)" << "\n";
    c << " 2 -> Compute HMAC (parallel)" << "\n";
    c << " 3 -> Compute HMAC (simple)" << "\n";\



}