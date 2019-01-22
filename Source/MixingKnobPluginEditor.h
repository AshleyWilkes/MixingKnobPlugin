/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MixingKnobPluginProcessor.h"

//==============================================================================
/**
*/
class MixingKnobPluginAudioProcessorEditor  : public AudioProcessorEditor,
	public Slider::Listener
{
public:
    MixingKnobPluginAudioProcessorEditor (MixingKnobPluginAudioProcessor&);
    ~MixingKnobPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

	//==============================================================================
	void sliderValueChanged(Slider* slider);

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MixingKnobPluginAudioProcessor& processor;

	Label directoryToScanLabel;
	TextButton scanDirectoryButton;
	ComboBox knownPluginsComboBox;
	TextButton openPluginButton;
	Slider mixedRatioSlider;

	void scanDirectoryClicked();
	void openPluginClicked();
	void updateKnownPluginsComboBox();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixingKnobPluginAudioProcessorEditor)
};
