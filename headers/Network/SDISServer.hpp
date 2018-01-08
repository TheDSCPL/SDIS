#pragma once

#include "TCPserver.hpp"

class SDISServer : public TCPServer {
    void handler(Connection& c);
    void displayHelpMenu(Connection& c);
public:
    const unsigned int port;

    SDISServer(unsigned int port=2018);
};