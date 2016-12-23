/*
 * my client of Collage. 
 */

#include <co/co.h>

int main(int argc, char** argv)
{
    co::init(argc, argv);

    co::ConnectionDescriptionPtr connDesc = new co::ConnectionDescription;
    connDesc->type = co::CONNECTIONTYPE_TCPIP;
    connDesc->setHostname( "localhost" );

    co::LocalNodePtr client = new co::LocalNode();
    client->addConnectionDescription(connDesc);

    client->listen();
    
    connDesc = new co::ConnectionDescription;
    connDesc->type = co::CONNECTIONTYPE_TCPIP;
    connDesc->port = 12548;
    connDesc->setHostname( "localhost" );
    co::NodePtr serverProxy = new co::Node();
    serverProxy->addConnectionDescription(connDesc);

    client->connect(serverProxy);

    client->close();
    client = 0;

    co::exit();

    return EXIT_SUCCESS;
} 