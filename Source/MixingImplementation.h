/*
  ==============================================================================

    MixerImplementation.h
    Created: 22 Jan 2019 10:19:55am
    Author:  Ashley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class MixingImplementation
{
public:
	virtual ~MixingImplementation() {};
	virtual void prepareToPlay(double sampleRate, int samplesPerBlock, 
		int numInputChannels, int numOutputChannels, int blockSize) = 0;
	virtual void releaseResources() = 0;
	virtual void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) = 0;
	virtual void setGain(float newGainValue) = 0;
	virtual float getGain() = 0;
	virtual void takeOwnershipOfPluginAndUseIt(AudioProcessor* plugin) = 0;
};