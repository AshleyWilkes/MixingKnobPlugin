/*
  ==============================================================================

    GraphMixingImplementation.h
    Created: 22 Jan 2019 10:21:32am
    Author:  Ashley

  ==============================================================================
*/

#pragma once

#include "GainProcessor.h"
#include "MixingImplementation.h"

class GraphMixingImplementation : public MixingImplementation {
public:
	GraphMixingImplementation();
	void prepareToPlay(double sampleRate, int samplesPerBlock, 
		int numInputChannels, int numOutputChannels, int blockSize) override;
	void releaseResources() override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	void setGain(float newGainValue) override;
	float getGain() override;
	void takeOwnershipOfPluginAndUseIt(AudioProcessor* plugin) override;
private:
	using GraphIO = AudioProcessorGraph::AudioGraphIOProcessor;
	using NodePtr = AudioProcessorGraph::Node::Ptr;
	using NodeId = AudioProcessorGraph::NodeID;

	float gainValue;

	std::unique_ptr<AudioProcessorGraph> processorGraph;
	NodePtr audioInputNode;
	NodePtr audioOutputNode;
	GainProcessor* leftGain{ nullptr };
	GainProcessor* rightGain{ nullptr };
	AudioProcessor* innerPlugin{ nullptr };

	NodeId INPUT{ 100 };
	NodeId OUTPUT{ 101 };
	NodeId LEFT_GAIN{ 102 };
	NodeId RIGHT_GAIN{ 103 };
	NodeId INNER_PLUGIN{ 104 };

	void initializeGraph(double sampleRate, int samplesPerBlock,
		int numInputChannels, int numOutputChannels, int blockSize);
	void connect(NodeId in, NodeId out);

	double sampleRate_;
	int samplesPerBlock_;
	int numInputChannels_;
	int numOutputChannels_;
	int blockSize_;
};