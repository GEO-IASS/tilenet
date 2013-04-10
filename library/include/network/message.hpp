#pragma once
#ifndef _MESSAGE_HPP
#define _MESSAGE_HPP

#include <strstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/noncopyable.hpp>
#include "settings.hpp"

namespace proto {
	template<msgid_type Id>
	struct MsgFormat;
}

namespace net {



class Message
	: public boost::noncopyable
{
public:
	Message(const msgid_type id, const std::vector<byte>&& msg);
	~Message();


	const std::vector<byte>& buffer() const;
	msgid_type id() const;

private:
	msgid_type				mId;
	std::vector<byte>	mBuffer;
};



template<msgid_type Id>
shared_ptr<Message> make_message(const proto::MsgFormat<Id>& msg)
{
	std::stringstream ss;
	boost::archive::text_oarchive archive(ss);
	archive << Id << msg;

	std::string buf = ss.str();

	std::vector<byte> buffer(buf.begin(), buf.end());

	return shared_ptr<Message>(new Message(Id, std::move(buffer)));
}



template<msgid_type Id>
void extract_message(const shared_ptr<Message>& msg, proto::MsgFormat<Id>& dest)
{
	std::stringstream ss(std::string(msg->buffer().begin(), msg->buffer().end()));
	boost::archive::text_iarchive archive(ss);
	msgid_type extrId;
	archive >> extrId;

	tnAssert(extrId == Id);
	archive >> dest;
}



}


#endif