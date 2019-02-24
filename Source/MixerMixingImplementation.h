/*
  ==============================================================================

    MixerMixingImplementation.h
    Created: 22 Jan 2019 10:21:57am
    Author:  Ashley

  ==============================================================================
*/

#pragma once
#include "GainProcessor.h"
#include "MixingImplementation.h"

class MixerMixingImplementation : public MixingImplementation {
public:
	MixerMixingImplementation();
	void prepareToPlay(double sampleRate, int samplesPerBlock,
		int numInputChannels, int numOutputChannels, int blockSize) override;
	void releaseResources() override;
	void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;
	void setGain(float newGainValue) override;
	float getGain() override;
	void takeOwnershipOfPluginAndUseIt(AudioProcessor* plugin) override;
private:
	float gainValue;
	int samplesPerBlock_;
	double sampleRate_;

	AudioProcessor* innerProcessor;
};