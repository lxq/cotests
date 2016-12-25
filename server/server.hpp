#pragma once


#include <co/co.h>
#include <lunchbox/monitor.h>

enum Commands
{
    CMD_ASYNC = co::CMD_NODE_CUSTOM,
    CMD_SYNC,
    CMD_DATA,
    CMD_DATA_REPLY,
    CMD_EXIT_SERVER
};

class Server: public co::LocalNode
{
    typedef co::CommandFunc< Server > CmdFunc;

public:
    Server()
        : _gotAsync( false )
        , _counter( 0 )
        , _monitor(false)
    {
        co::CommandQueue* q = getCommandThreadQueue();
        registerCommand( CMD_ASYNC, CmdFunc( this, &Server::_cmdAsync ), q );
        registerCommand( CMD_SYNC, CmdFunc( this, &Server::_cmdSync ), q );
        registerCommand( CMD_DATA, CmdFunc( this, &Server::_cmdData ), q );
        registerCommand( CMD_DATA_REPLY,CmdFunc( this, &Server::_cmdDataReply ), q );
        registerCommand( CMD_EXIT_SERVER,CmdFunc( this, &Server::_cmdExit ), q );
    }

    void waitEQ(bool v)
    {
        _monitor.waitEQ(v);
    }
    
private:
    bool _gotAsync;
    uint32_t _counter;
    lunchbox::Monitor<bool> _monitor;

    bool _cmdAsync( co::ICommand& )
    {
        std::cout<< "Received CMD_ASYNC."<< std::endl;
        _gotAsync = true;
        return true;
    }

    bool _cmdSync( co::ICommand& command )
    {
        std::cout<< "Received CMD_SYNC."<< std::endl;
        ackRequest( command.getNode(), command.get< uint32_t >( ));
        return true;
    }

    bool _cmdData( co::ICommand& command )
    {
        std::cout<< "Received CMD_DATA. contents: "<< std::endl;
        command.getNode()->send( CMD_DATA_REPLY )
            << command.get< uint32_t >() << ++_counter;
        std::cout<< command.get<std::string>() <<std::endl;
        return true;
    }

    bool _cmdDataReply( co::ICommand& command )
    {
        std::cout<< "Received CMD_REPLY."<< std::endl;
        const uint32_t request = command.get< uint32_t >();
        const uint32_t result = command.get< uint32_t >();
        //TEST( result == ++_counter );
        serveRequest( request, result );
        return true;
    }

    bool _cmdExit(co::ICommand& command)
    {
        std::cout<< "Received CMD_EXIT_SERVER."<< std::endl;
        _monitor.set(true);

        return true;
    }

};

typedef lunchbox::RefPtr<Server> ServerPtr;
