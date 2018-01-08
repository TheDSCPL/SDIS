#include "../../headers/Network/SDISServer.hpp"

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
            case 1:
                c << "Computing HMAC: ";
            default:
                c << "Seleccionou " << i << "\n";
        }
    } while(true);
}

void SDISServer::displayHelpMenu(Connection &c) {
    
}