#include "includes.hpp"
#include "network/message.hpp"
#include "network/connection_port.hpp"

#include "handshake_handler_v1_0.hpp"
#include "main_com_handler_v1_0.hpp"
#include "server_info_v1_0.hpp"

namespace client {
namespace com {
namespace v1_0 {


HandshakeHandler::HandshakeHandler(ClientApp& app, const shared_ptr<net::ConnectionPort>& port)
	: mApp(app)
	, mPort(port)
{
	tnAssert(port);

	mDispatcher.add(&HandshakeHandler::handleServerInformation, this);
	mDispatcher.add(&HandshakeHandler::handleAccessGranted, this);
}

HandshakeHandler::~HandshakeHandler()
{

}

OVERRIDE shared_ptr<ComHandler> HandshakeHandler::handleMessage( const shared_ptr<net::Message>& msg)
{
	mDispatcher.dispatch(msg);
	return mNextHandler;
}

void HandshakeHandler::handleServerInformation( proto::v1_0::to_client::Handshake_P2_ServerInformation& handshake )
{
	mServerInfo = std::make_shared<ServerInfo>(handshake.server_name, handshake.server_info, handshake.package_name, handshake.package_interface);

	Log(L"client-com").info()	<< L"Server information:"
								<< L"\nName:      " << handshake.server_name
								<< L"\nInfo:      " << handshake.server_info
								<< L"\nPackage:   " << handshake.package_name
								<< L"\nInterface: " << handshake.package_interface;

	proto::v1_0::to_srv::Handshake_P3_accessrequest confirmation;

	confirmation.accept_handshake = true;
	mPort->send(net::make_message(confirmation));


}

void HandshakeHandler::handleAccessGranted( proto::v1_0::to_client::Handshake_P4_AcceptesGranted& handshake )
{
	tnAssert(mServerInfo);
	// Yea, we got access
	mNextHandler = MainComHandler::Create(mApp, mPort, mServerInfo);
}

shared_ptr<ComInterface> HandshakeHandler::getComInterface()
{
	return nullptr;
}




}}}