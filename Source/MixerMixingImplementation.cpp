/*
  ==============================================================================

    MixerMixingImplementation.cpp
    Created: 22 Jan 2019 10:21:57am
    Author:  Ashley

  ==============================================================================
*/

#include "MixerMixingImplementation.h"

MixerMixingImplementation::MixerMixingImplementation() {}

void MixerMixingImplementation::prepareToPlay(double sampleRate, int samplesPerBlock,
	int /*numInputChannels*/, int /*numOutputChannels*/, int /*blockSize*/) {
	samplesPerBlock_ = samplesPerBlock;
	sampleRate_ = sampleRate;
}

void MixerMixingImplementation::releaseResources() {}

void MixerMixingImplementation::processBlock(AudioBuffer<float>& buffer, MidiBuffer& /*midiMessages*/) {
	//impl
	//vytvorit left a right audio sourcy
	MemoryAudioSource leftAudioSource{ buffer, true };
	leftAudioSource.prepareToPlay(samplesPerBlock_, sampleRate_);
	AudioBuffer<float> leftBuffer{ buffer.getNumChannels(), buffer.getNumSamples() };
	AudioSourceChannelInfo leftChannelInfo(leftBuffer);
	leftAudioSource.getNextAudioBlock(leftChannelInfo);

	leftBuffer.applyGain(1 - gainValue);
	MemoryAudioSource realLeftAudioSource{ leftBuffer, true };
	//realLeftAudioSource.prepareToPlay(samplesPerBlock_, sampleRate_);

	MemoryAudioSource rightAudioSource{ buffer, true };
	rightAudioSource.prepareToPlay(samplesPerBlock_, sampleRate_);
	AudioBuffer<float> rightBuffer{ buffer.getNumChannels(), buffer.getNumSamples() };
	AudioSourceChannelInfo rightChannelInfo(rightBuffer);
	rightAudioSource.getNextAudioBlock(rightChannelInfo);

	rightBuffer.applyGain(gainValue);
	MemoryAudioSource realRightAudioSource{ rightBuffer, true };
	//realRightAudioSource.prepareToPlay(samplesPerBlock_, sampleRate_);

	//innerProcessor->processBlock(rightBuffer, midiMessages);

	//vytorit mixer a poslat mu sourcy
	MixerAudioSource mixerAudioSource;
	mixerAudioSource.addInputSource(&realLeftAudioSource, false);
	mixerAudioSource.addInputSource(&realRightAudioSource, false);

	//zavolat mixer.getNext...
	AudioSourceChannelInfo channelInfo(buffer);
	mixerAudioSource.getNextAudioBlock(channelInfo);
}

void MixerMixingImplementation::setGain(float newGainValue) {
	if (newGainValue < 0) newGainValue = 0;
	if (newGainValue > 100) newGainValue = 100;
	gainValue = newGainValue / 100;
}

float MixerMixingImplementation::getGain() {
	return gainValue * 100;
}

void MixerMixingImplementation::takeOwnershipOfPluginAndUseIt(AudioProcessor* /*plugin*/) {
	AlertWindow::showMessageBox(AlertWindow::AlertIconType::InfoIcon, "Error", "MixerMixingImplementation::takeOwnership is not implemented!", "Close");
}