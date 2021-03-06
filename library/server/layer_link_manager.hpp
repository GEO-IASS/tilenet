#pragma once
#ifndef _LAYER_LINK_MANAGER_HPP
#define _LAYER_LINK_MANAGER_HPP


#include <unordered_map>
#include <mutex>
#include "settings.hpp"



namespace srv {

class Layer;
class FrameLayer;
class Participant;


class LayerLinkManager
{
	class Node;
public:
	LayerLinkManager();
	~LayerLinkManager();

	void registerParticipant(const shared_ptr<Participant> participant);
	void unregisterParticipant(const shared_ptr<Participant> participant);
	void registerLayer(const shared_ptr<Layer> layer);
	void unregisterLayer(const shared_ptr<Layer> layer);

	void linkLayerToParticipant(const shared_ptr<Layer>& layer, const shared_ptr<Participant>& participant);
	void updateFrameLinks(const shared_ptr<FrameLayer>& frame, const std::vector<shared_ptr<Layer>>& addLayers, const std::vector<shared_ptr<Layer>>& removeLayers);

	std::vector<shared_ptr<Participant>> getLinkedParticipants(const shared_ptr<Layer>& layer) const;
	std::vector<shared_ptr<Layer>> getLinkedLayers(const shared_ptr<Participant>& participant) const;

	static LayerLinkManager& Inst();
private:
	Node* _layer_node(const shared_ptr<Layer>& layer) const;
	Node* _participant_node(const shared_ptr<Participant>& participant) const;

private:
	mutable std::mutex mMutex;
	std::unordered_map<shared_ptr<Layer>, Node*> mLayerToNodeMapping;
	std::unordered_map<shared_ptr<Participant>, Node*> mParticipantToNodeMapping;
};




}



#endif