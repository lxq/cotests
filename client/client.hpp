#pragma once

#include <co/co.h>


enum Commands
{
    CMD_ASYNC = co::CMD_NODE_CUSTOM,
    CMD_SYNC,
    CMD_DATA,
    CMD_DATA_REPLY,
    CMD_EXIT_SERVER
};

class Client: public co::LocalNode
{
    typedef co::CommandFunc< Client > CmdFunc;
public:
    Client()
    {
        co::CommandQueue* q = getCommandThreadQueue();
        registerCommand( CMD_DATA_REPLY,CmdFunc( this, &Client::_cmdDataReply ), q );      
    }

private:
    bool _cmdDataReply( co::ICommand& command )
    {
        std::cout<< "starting CMD_REPLY."<< std::endl;
        const uint32_t request = command.read< uint32_t >();
        const uint32_t result = command.read< uint32_t >();
        serveRequest( request, result );
        std::cout<< "ending CMD_REPLY"<< std::endl;
        return true;
    }

};

typedef lunchbox::RefPtr<Client> ClientPtr;


