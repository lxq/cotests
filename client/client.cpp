/*
 * my client of Collage. 
 */

#include <co/co.h>

enum Commands
{
    CMD_ASYNC = co::CMD_NODE_CUSTOM,
    CMD_SYNC,
    CMD_DATA,
    CMD_DATA_REPLY,
    CMD_EXIT_SERVER
};

static const std::string payload( "Hi! I am your payload. " );

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

    // send  cmd for testing
    serverProxy->send(CMD_ASYNC);

    {
        // send for request 
        uint32_t request = client->registerRequest();
        serverProxy->send(CMD_SYNC) << request;
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

    client->disconnect(serverProxy);
    client->close();
    client = 0;

    co::exit();

    return EXIT_SUCCESS;
} 