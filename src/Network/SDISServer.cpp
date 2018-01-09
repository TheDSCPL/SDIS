#include "../../headers/Network/SDISServer.hpp"
#include "../../headers/ParallelProcesses/PrimesParallelProcess.hpp"
#include "../../headers/SHA512.h"

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
                c << "---Computing HMAC in series---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();
                c << bytes2HexString( SHA512( concat( outputPadSHA512(key) , SHA512( concat( inputPadSHA512(key) , message ) ) ) ) ) << "\n";
            }
                break;
            case 2: {
                c << "---Computing HMAC in parallel---" << "\n";
                c << "Insert the key: ";
                std::string key = c.readLine();
                c << "Insert the message: ";
                std::string message = c.readLine();

                c << bytes2HexString( SHA512( concat( outputPadSHA512(key) , SHA512( concat( inputPadSHA512(key) , message ) ) ) ) ) << "\n";
            }
                break;
            case 4: {
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