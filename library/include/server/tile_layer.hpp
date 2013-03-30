#pragma once
#ifndef _TILELAYER_HPP
#define _TILELAYER_HPP

#include "settings.hpp"
#include "layer.hpp"


namespace net {
	class Message;
}


namespace srv {

class TileLayer
	: public Layer
{
public:
	TileLayer(const Rect& size, const Ratio& ratio, TNFLAG flags);
	~TileLayer();

	void putTile(const Point& pos, TNTILE* tile);

	const Ratio& ratio() const;
	const Rect&	size() const;

	override void destroy();
	override shared_ptr<TilenetObject> clone();
	override void flush();
	override shared_ptr<net::Message> getStateMessage();





private:
};



}


#endif