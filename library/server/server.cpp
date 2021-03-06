#include "includes.hpp"
#include "server/server.hpp"
#include "server/event_queue.hpp"

#include "server/participant.hpp"

namespace srv {

	


Acceptor::Acceptor(const shared_ptr<Server>& server)
	: mServer(server)
{
}

Acceptor::~Acceptor()
{
}

const shared_ptr<Server>& Acceptor::server() const
{
	return mServer;
}

OVERRIDE void Acceptor::init()
{
	TilenetObject::init();
}





Server::Server(const TNSVRCONFIG* init)
	: mEvents(std::make_shared<EventQueue>())
	, mServerName(init->name)
	, mServerInfo(init->info)
	, mPackageName(init->pkg)
	, mPackageInterface(init->pkgi)
	, mNumParticipants(0)
{

}

Server::~Server()
{

}


OVERRIDE void Server::init()
{
	TilenetObject::init();
}


void Server::destroy()
{
	// Do not destroy if participants are connected to the server
	if(mNumParticipants > 0)
	{
		BOOST_THROW_EXCEPTION(excp::NotReadyException() << excp::InfoWhat(L"Participants are still connected to the server!"));
	}

	if(mNumAcceptors > 0)
	{
		BOOST_THROW_EXCEPTION(excp::NotReadyException() << excp::InfoWhat(L"Acceptors are still related to the server!"));
	}
}


bool Server::fetchNextEvent( TNEVENT* dest, size_t* timeout )
{
	if(timeout)
	{
		std::chrono::milliseconds timeout_in_milliseconds(*timeout);
		bool success = mEvents->pop(dest, timeout_in_milliseconds);
		*timeout = static_cast<std::size_t>(timeout_in_milliseconds.count());
		return success;
	}else{
		mEvents->pop(dest);
		return true;
	}
}

std::shared_ptr<Participant> Server::addParticipant( const shared_ptr<net::ConnectionPort>& conport )
{
	shared_ptr<Participant> participant = Participant::Create(mEvents, conport, self<Server>());
	++mNumParticipants;
	return participant;
}


void Server::removeParticipant( const std::shared_ptr<Participant>& p )
{
	--mNumParticipants;
}


void Server::addAcceptor()
{
	++mNumAcceptors;
}

void Server::removeAcceptor()
{
	--mNumAcceptors;
}



std::shared_ptr<EventQueue> Server::eventQueue() const
{
	return mEvents;
}

const string& Server::serverName() const
{
	return mServerName;
}

const string& Server::serverInfo() const
{
	return mServerInfo;
}

const string& Server::packageName() const
{
	return mPackageName;
}

const string& Server::packageInterface() const
{
	return mPackageInterface;
}












}