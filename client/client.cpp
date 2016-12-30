/*
 * my client of Collage. 
 */

#include "client.hpp"


static const std::string payload( "Hi! I am your payload. " );

int main(int argc, char** argv)
{
    co::init(argc, argv);

    co::ConnectionDescriptionPtr connDesc = new co::ConnectionDescription;
    connDesc->type = co::CONNECTIONTYPE_TCPIP;
    connDesc->setHostname( "localhost" );

    ClientPtr client = new Client();
    client->addConnectionDescription(connDesc);

    client->listen();
    
    connDesc = new co::ConnectionDescription;
    connDesc->type = co::CONNECTIONTYPE_TCPIP;
    connDesc->port = 12548;
    connDesc->setHostname( "localhost" );
    co::NodePtr serverProxy = new co::Node();
    serverProxy->addConnectionDescription(connDesc);

    client->connect(serverProxy);

    std::cout<< "client id: " << client->getNodeID() << std::endl;
    std::cout<< "serverproxy id: "<< serverProxy->getNodeID()<< std::endl;

    // send  cmd for testing
    serverProxy->send(CMD_ASYNC);

    {
        // send for request 
        uint32_t request = client->registerRequest();
        serverProxy->send(CMD_SYNC) << request;
        // NOTE: 如果Server没有处理Request,则会导致waitRequest进行入阻塞.
        client->waitRequest(request);
    }

    {
        lunchbox::Request<void> request = client->registerRequest<void>();
        serverProxy->send(CMD_SYNC)<<request;
    }

    {
        lunchbox::Request<uint32_t> request = client->registerRequest<uint32_t>();
        serverProxy->send(CMD_DATA) <<request <<payload;
    }
    std::cout<< "end sending." << std::endl;
    client->disconnect(serverProxy);
    client->close();
    client = 0;

    co::exit();

    return EXIT_SUCCESS;
} 