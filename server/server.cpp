/*
 * my Server of Collage. 
 */

#include "server.hpp"

int main(int argc, char** argv)
{
    co::init(argc, argv);

    co::ConnectionDescriptionPtr connDesc = new co::ConnectionDescription;
    connDesc->type = co::CONNECTIONTYPE_TCPIP;
    connDesc->port = 12548;
    connDesc->setHostname( "localhost" );

    ServerPtr server = new Server();
    server->addConnectionDescription(connDesc);

    server->listen();
    
    server->waitEQ(true);

    server = 0;

    co::exit();

    return EXIT_SUCCESS;
} 