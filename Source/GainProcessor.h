/*
  ==============================================================================

    GainProcessor.h
    Created: 17 Jan 2019 3:59:06am
    Author:  Ashley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class GainProcessor : public AudioProcessor
{
public:
	//==============================================================================
	GainProcessor() {
		//gain.setGainDecibels(-6.0f);
	}
	~GainProcessor() {}

	//==============================================================================
	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override {}

#ifndef JucePlugin_PreferredChannelConfigurations
	//bool isBusesLayoutSupported(const BusesLayout& layouts) const override { return true; }
#endif

	void processBlock(AudioBuffer<float>&, MidiBuffer&) override;

	//==============================================================================
	AudioProcessorEditor* createEditor() override { return nullptr; }
	bool hasEditor() const override { return false; }

	//==============================================================================
	const String getName() const override { return "Gain"; }

	bool acceptsMidi() const override { return false; }
	bool producesMidi() const override { return false; }
	bool isMidiEffect() const override { return false; }
	double getTailLengthSeconds() const override { return 0; }

	//==============================================================================
	int getNumPrograms() override { return 1; }
	int getCurrentProgram() override { return 0; }
	void setCurrentProgram(int index) override {}
	const String getProgramName(int index) override { return {}; }
	void changeProgramName(int index, const String& newName) override {}

	//==============================================================================
	void getStateInformation(MemoryBlock& destData) override {}
	void setStateInformation(const void* data, int sizeInBytes) override {}

	//==============================================================================
	void setGain(float newGain);
private:
	dsp::Gain<float> gain;
	//==============================================================================
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(GainProcessor)
};
