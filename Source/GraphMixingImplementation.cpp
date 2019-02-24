/*
  ==============================================================================

    GraphMixingImplementation.cpp
    Created: 22 Jan 2019 10:21:32am
    Author:  Ashley

  ==============================================================================
*/

#include "GraphMixingImplementation.h"

GraphMixingImplementation::GraphMixingImplementation() : processorGraph(std::make_unique<AudioProcessorGraph>()) {}

void GraphMixingImplementation::prepareToPlay(double sampleRate, int samplesPerBlock,
	int numInputChannels, int numOutputChannels, int blockSize) {
	processorGraph->setPlayConfigDetails(
		numInputChannels, numOutputChannels,
		sampleRate, blockSize);
	processorGraph->prepareToPlay(sampleRate, samplesPerBlock);

	initializeGraph(sampleRate, samplesPerBlock,
		numInputChannels, numOutputChannels, blockSize);
}

void GraphMixingImplementation::releaseResources() {
	processorGraph->releaseResources();
}

void GraphMixingImplementation::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) {
	leftGain->setGain(1 - gainValue);
	rightGain->setGain(gainValue);
	processorGraph->processBlock(buffer, midiMessages);
}

void GraphMixingImplementation::setGain(float newGainValue) {
	if (newGainValue < 0) newGainValue = 0;
	if (newGainValue > 100) newGainValue = 100;
	gainValue = newGainValue / 100;
}

float GraphMixingImplementation::getGain() {
	return gainValue * 100;
}

void GraphMixingImplementation::connect(NodeId in, NodeId out) 
{
	for (int channel = 0; channel < 2; ++channel) {
		processorGraph->addConnection({
			{ in, channel },
			{ out, channel }
			});
	}
}

void GraphMixingImplementation::initializeGraph(double sampleRate, int samplesPerBlock,
	int numInputChannels, int numOutputChannels, int blockSize)
{
	sampleRate_ = sampleRate;
	samplesPerBlock_ = samplesPerBlock;
	numInputChannels_ = numInputChannels;
	numOutputChannels_ = numOutputChannels;
	blockSize_ = blockSize;

	processorGraph->clear();//pro jistotu a taky proto, abych to nemohl zapomenout, az budu implementovat zmenu vlozeneho pluginu
	processorGraph->setPlayConfigDetails(
		numInputChannels, numOutputChannels,
		sampleRate, blockSize);

							//vytvorim input/output
	audioInputNode = processorGraph->addNode(new GraphIO(GraphIO::audioInputNode), INPUT);
	audioOutputNode = processorGraph->addNode(new GraphIO(GraphIO::audioOutputNode), OUTPUT);

	auto left = std::make_unique<GainProcessor>();
	left->setPlayConfigDetails(
		numInputChannels, numOutputChannels,
		sampleRate, blockSize);
	processorGraph->addNode(left.get(), LEFT_GAIN);
	leftGain = left.release();

	auto right = std::make_unique<GainProcessor>();
	right->setPlayConfigDetails(
		numInputChannels, numOutputChannels,
		sampleRate, blockSize);
	processorGraph->addNode(right.get(), RIGHT_GAIN);
	rightGain = right.release();
}

void GraphMixingImplementation::takeOwnershipOfPluginAndUseIt(AudioProcessor* plugin) {
	if (innerPlugin) {
		innerPlugin = nullptr;
		processorGraph->removeNode(INNER_PLUGIN);
	}

	innerPlugin = plugin;
	plugin->setPlayConfigDetails(
		numInputChannels_, numOutputChannels_,
		sampleRate_, blockSize_);
	processorGraph->addNode(plugin, INNER_PLUGIN);

	connect(INPUT, LEFT_GAIN);
	connect(LEFT_GAIN, OUTPUT);
	connect(INPUT, INNER_PLUGIN);
	connect(INNER_PLUGIN, RIGHT_GAIN);
	connect(RIGHT_GAIN, OUTPUT);
}