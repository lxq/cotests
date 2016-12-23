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

class Server: public co::LocalNode
{
    typedef co::CommandFunc< Server > CmdFunc;

public:
    Server()
        : _gotAsync( false )
        , _counter( 0 )
    {
        co::CommandQueue* q = getCommandThreadQueue();
        registerCommand( CMD_ASYNC, CmdFunc( this, &Server::_cmdAsync ), q );
        registerCommand( CMD_SYNC, CmdFunc( this, &Server::_cmdSync ), q );
        registerCommand( CMD_DATA, CmdFunc( this, &Server::_cmdData ), q );
        registerCommand( CMD_DATA_REPLY,CmdFunc( this, &Server::_cmdDataReply ), q );
    }

private:
    bool _gotAsync;
    uint32_t _counter;

    bool _cmdAsync( co::ICommand& )
    {
        _gotAsync = true;
        return true;
    }

    bool _cmdSync( co::ICommand& command )
    {
        //TEST( _gotAsync );
        ackRequest( command.getNode(), command.get< uint32_t >( ));
        return true;
    }

    bool _cmdData( co::ICommand& command )
    {
        //TEST( _gotAsync );
        command.getNode()->send( CMD_DATA_REPLY )
            << command.get< uint32_t >() << ++_counter;
        //TEST( command.get< std::string >() == payload );
        return true;
    }

    bool _cmdDataReply( co::ICommand& command )
    {
        //TEST( !_gotAsync );
        const uint32_t request = command.get< uint32_t >();
        const uint32_t result = command.get< uint32_t >();
        //TEST( result == ++_counter );
        serveRequest( request, result );
        return true;
    }

};

typedef lunchbox::RefPtr<Server> ServerPtr;
