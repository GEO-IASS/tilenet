#pragma once
#ifndef _CLIENT_HPP
#define _CLIENT_HPP


#include <boost/asio/io_service.hpp>
#include <boost/asio/steady_timer.hpp>

#include "settings.hpp"
#include "network/connection_port.hpp"

namespace net {
class Message;
}

namespace client {
namespace com {
	class ComHandler;
}

class Messenger;
class ClientWindow;
class PackageManager;

class ClientApp
	: public std::enable_shared_from_this<ClientApp>
	, public net::ConnectionPort::Handler
	, boost::noncopyable
{
	typedef boost::asio::basic_waitable_timer<std::chrono::steady_clock> steady_timer;
public:
	ClientApp();
	~ClientApp();

	void start();
	void postConnection(const shared_ptr<net::ConnectionPort>& port);
	void stop(bool now = false);

	ClientWindow& window() const;
	const shared_ptr<net::ConnectionPort>& port() const;
	const shared_ptr<PackageManager>& pmanager() const;
	boost::asio::io_service& service();
private:
	void run();
	void disconnect();
	void handleNewConnection(const shared_ptr<net::ConnectionPort>& port);
	void processWindow();

	OVERRIDE void onReceive(const shared_ptr<net::Message>& msg);
	OVERRIDE void onDisconnect();
private:
	boost::asio::io_service mService;
	std::shared_ptr<com::ComHandler> mComHandler;
	std::shared_ptr<net::ConnectionPort> mPort;
	std::unique_ptr<ClientWindow> mWindow;
	std::shared_ptr<Messenger> mMessenger;
	std::shared_ptr<PackageManager> mPackManager;
	std::unique_ptr<boost::asio::io_service::work> mBusyWork;
	steady_timer mWindowProcessTimer;
	float mFrameRate;
	Log	log;


	static ClientApp* Singleton;
};





}










#endif