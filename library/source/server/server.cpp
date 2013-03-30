#include "includes.hpp"
#include "server/server.hpp"
#include "server/event_queue.hpp"

namespace srv {

	


Acceptor::Acceptor()
{
}

Acceptor::~Acceptor()
{
}

override void Acceptor::flush()
{
	BOOST_THROW_EXCEPTION(excp::NotSupportedException() << excp::InfoWhat(L"Acceptor can not be flushed!"));
}



Server::Server(const TNSVRCONFIG* init)
{

}

Server::~Server()
{

}

void Server::destroy()
{
	NOT_IMPLEMENTED();
}

shared_ptr<TilenetObject> Server::clone()
{
	BOOST_THROW_EXCEPTION(excp::NotSupportedException() << excp::InfoWhat(L"Server can not be cloned!"));
}

void Server::flush()
{
	BOOST_THROW_EXCEPTION(excp::NotSupportedException() << excp::InfoWhat(L"Server can not be flushed!"));
}


bool Server::fetchNextEvent( TNEVENT* dest, size_t* timeout )
{
	if(timeout)
	{
		return mEvents->pop(dest, std::chrono::milliseconds(*timeout));
	}else{
		mEvents->pop(dest);
		return true;
	}
}

void Server::addAcceptor(const std::shared_ptr<Acceptor>& acceptor)
{
	mAcceptors.push_back(acceptor);
	acceptor->start();
}





}