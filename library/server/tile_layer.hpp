#pragma once
#ifndef _TILELAYER_HPP
#define _TILELAYER_HPP

#include <mutex>
#include <boost/asio/strand.hpp>

#include "settings.hpp"

#include "layer.hpp"
#include "utils/field.hpp"
#include "commit_queue.hpp"

#include "network/v1.0/protocol_tile.hpp"

namespace net {
	class Message;
}


namespace srv {

namespace job {
	class UpdateLayerJob;
}

class TileLayer
	: public Layer
{
	friend class job::UpdateLayerJob;
public:
	TileLayer(const Rect& size, const Ratio& ratio, const string& aspectName, TNFLAG flags);
	~TileLayer();

	void init();
	void update(const TNTILE* tiles, const TNBOOL* toupdate);

	const Ratio& ratio() const;
	const Rect&	size() const;

	OVERRIDE void destroy();

private:
	Commit update(const std::vector<net::PTile>& tiles, const std::vector<bool>& toupdate);
	Commit makeFullSnapshotCommit(bool asNewCommit);
	void makeInitialCommit();


	virtual OVERRIDE std::vector<Commit> getCommitsUpTo(TNID nr);
	virtual OVERRIDE Commit getDelta( TNID nr );


private:
	boost::asio::strand mUpdateStrand;
	std::mutex			mMutex;
	Field<net::PTile>	mTileField;
	const Ratio			mRatio;
	string				mAspectName;
	CommitQueue			mCommits;
};



}


#endif