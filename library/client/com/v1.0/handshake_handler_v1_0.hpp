#pragma once
#ifndef _HANDSHAKE_HANDLER_V1_HPP
#define _HANDSHAKE_HANDLER_V1_HPP


#include "settings.hpp"
#include "client/com/com_handler.hpp"

#include "network/v1.0/protocol_v1_0.hpp"
#include "network/dispatcher.hpp"


namespace net {
	class ConnectionPort;
}

namespace client {

class ClientApp;


namespace com {
namespace v1_0 {

class ServerInfo;

class HandshakeHandler
	: public ComHandler
{
public:
	HandshakeHandler(ClientApp& app, const shared_ptr<net::ConnectionPort>& port);
	~HandshakeHandler();

	virtual OVERRIDE shared_ptr<ComHandler> handleMessage(const shared_ptr<net::Message>&);
private:
	void handleServerInformation(proto::v1_0::to_client::Handshake_P2_ServerInformation& handshake);
	void handleAccessGranted(proto::v1_0::to_client::Handshake_P4_AcceptesGranted& handshake);

	virtual shared_ptr<ComInterface> getComInterface();


private:
	shared_ptr<ComHandler> mNextHandler;
	net::Dispatcher mDispatcher;
	ClientApp& mApp;
	const shared_ptr<net::ConnectionPort> mPort;
	shared_ptr<ServerInfo> mServerInfo;
};




}}}




#endif