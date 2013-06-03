#pragma once
#ifndef _RENDERER_V1_0_HPP
#define _RENDERER_V1_0_HPP


#include <SFML/Graphics/RenderTarget.hpp>
#include <unordered_map>

#include "settings.hpp"
#include "client/render_interface.hpp"
#include "network/v1.0/protocol_tile.hpp"


namespace client{

class ClientWindow;
class PackageManager;
class Package;

namespace com {
namespace v1_0 {

class ServerInfo;
class TileMapper;


class Renderer
	: public RenderInterface
{
	class Layer;
	class FrameLayer;
	class RenderLayer;
public:
	Renderer(ClientWindow& window, const shared_ptr<TileMapper>& mapper, const shared_ptr<PackageManager>& pmanager, const shared_ptr<ServerInfo>& info);
	~Renderer();

	virtual OVERRIDE void render(sf::RenderTarget& target);

	void setTopLayer(TNID id);
	void defineLayer(TNID id, Rect size, Ratio r);
	void putTile(TNID layerid, Point pos, const net::PTile& tile);

private:
	shared_ptr<Layer> layer(TNID id) const;

	void calculateView(sf::RenderTarget& target, const shared_ptr<Layer>& layer);

private:
	shared_ptr<PackageManager> mPackManager;
	shared_ptr<TileMapper> mTileMapper;
	shared_ptr<ServerInfo> mServerInfo;
	shared_ptr<Package> mPackage;
	ClientWindow& mWindow;

	std::unordered_map<TNID, shared_ptr<Layer>> mIdToLayerMapping;
	TNID mTopLayerId;

	sf::Color mBGCOlor;
};




}}}


#endif