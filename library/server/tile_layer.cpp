#include "includes.hpp"

#include "tile_layer.hpp"
#include "tileset.hpp"
#include "layer.hpp"

#include "network/message.hpp"
#include "network/connection_port.hpp"

namespace srv {


TileLayer::TileLayer(const Rect& size, const Ratio& ratio, TNFLAG flags)
	: mTileField(size)
	, mRatio(ratio)
{
	clear();
}

TileLayer::~TileLayer()
{
}


void TileLayer::clear()
{
	// Clear the layer
	auto& storage = mTileField.storage();
	memset(storage.data(), 0, storage.size() * sizeof(TNTILE));
}


void TileLayer::putTile(const Point& pos, TNTILE* tile)
{
	std::lock_guard<std::mutex> guard(mMutex);
	mTileField.at(pos) = *tile;
}

const Ratio& TileLayer::ratio() const
{
	return mRatio;
}

const Rect& TileLayer::size() const
{
	return mTileField.size();
}

OVERRIDE void TileLayer::destroy()
{
	NOT_IMPLEMENTED();
}

OVERRIDE shared_ptr<TilenetObject> TileLayer::clone()
{
	NOT_IMPLEMENTED();
}

OVERRIDE void TileLayer::flush()
{
	NOT_IMPLEMENTED();
}

shared_ptr<net::Message> TileLayer::getStateMessage()
{
	std::lock_guard<std::mutex> guard(mMutex);
	proto::curv::to_client::LayerControl_SendFullLayer fullLayer;

	fullLayer.layerId = id();
	fullLayer.xratio = ratio().x;
	fullLayer.yratio = ratio().y;
	fullLayer.width = size().w;
	fullLayer.height = size().h;

	auto& storage = mTileField.storage();
	auto& content = fullLayer.layerContent;
	content.reserve(storage.size());

	for(auto& tile : mTileField.storage())
	{
		std::shared_ptr<Tileset> tileset;
		if(tile.tileset && (tileset = Tileset::Resolve(tile.tileset)))
		{
			content.push_back(tileset->constructTile(&tile));
		}else{
			content.push_back(net::PTile());
		}
	}

	tnAssert(content.size() == size().area());
	tnAssert(storage.size() == content.size());
	

	return net::make_message(fullLayer);
}


}