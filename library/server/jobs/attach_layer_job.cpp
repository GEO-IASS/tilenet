#include "includes.hpp"
#include "network/protocol.hpp"
#include "network/connection_port.hpp"
#include "network/message.hpp"
#include "attach_layer_job.hpp"

#include "server/participant.hpp"
#include "server/layer.hpp"
#include "server/layer_link_manager.hpp"

namespace srv {
namespace job {



AttachLayerJob::AttachLayerJob(const shared_ptr<Participant>& participant, const shared_ptr<Layer>& layer)
	: mParticipant(participant)
	, mLayer(layer)
{
	tnAssert(mParticipant);
}

AttachLayerJob::~AttachLayerJob()
{

}



void AttachLayerJob::process()
{
	LayerLinkManager& llm = LayerLinkManager::Inst();

	llm.linkLayerToParticipant(mLayer, mParticipant);

	// set the new id of the top layer
	{
		proto::curv::to_client::LayerControl_AttachLayer attach;

		attach.layerId = mLayer->id();
		mParticipant->port()->send(net::make_message(attach));
	}

	// send the all layers to the client
	{
		std::vector<shared_ptr<Layer>> layers = llm.getLinkedLayers(mParticipant);

		for(auto& layer : layers)
		{
			TNID lastCommit = layer->currentCommitNr();
			auto commits = layer->getCommitsUpTo(lastCommit);

			for(auto commit : commits)
				mParticipant->port()->send(commit);
		}
	}
}

}}