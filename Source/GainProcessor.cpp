/*
  ==============================================================================

    GainProcessor.cpp
    Created: 17 Jan 2019 3:59:06am
    Author:  Ashley

  ==============================================================================
*/

#include "GainProcessor.h"

void GainProcessor::setGain(float newGain)
{
	//gain.setGainDecibels(newGain);
	gain.setGainLinear(newGain);
}

void GainProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	dsp::ProcessSpec spec{ sampleRate, static_cast<uint32> (samplesPerBlock), 2 };
	gain.prepare(spec);
}

void GainProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer&)
{
	dsp::AudioBlock<float> block{ buffer };
	dsp::ProcessContextReplacing<float> context{ block };
	gain.process(context);
}