/*
  ==============================================================================

    PluginWindow.h
    Created: 25 Jan 2019 11:18:59am
    Author:  Ashley

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MixingKnobPluginAudioProcessor;

class PluginWindow : public DocumentWindow {
public:
	PluginWindow(AudioProcessor* plugin, MixingKnobPluginAudioProcessor& parent);
	~PluginWindow();

	void closeButtonPressed() override;
private:
	MixingKnobPluginAudioProcessor& parent_;
};