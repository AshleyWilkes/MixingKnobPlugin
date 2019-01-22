/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "MixingKnobPluginProcessor.h"
#include "MixingKnobPluginEditor.h"

//==============================================================================
MixingKnobPluginAudioProcessorEditor::MixingKnobPluginAudioProcessorEditor (MixingKnobPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
	directoryToScanLabel.setText("C:\\Program\ Files\\Common\ Files\\VST3", NotificationType::dontSendNotification);
	addAndMakeVisible(directoryToScanLabel);

	scanDirectoryButton.onClick = [this] { scanDirectoryClicked(); };
	scanDirectoryButton.setButtonText("Scan dir");
	addAndMakeVisible(scanDirectoryButton);

	updateKnownPluginsComboBox();
	addAndMakeVisible(knownPluginsComboBox);

	openPluginButton.onClick = [this] { openPluginClicked(); };
	openPluginButton.setButtonText("Load");
	addAndMakeVisible(openPluginButton);

	mixedRatioSlider.addListener(this);
	mixedRatioSlider.setRange(0, 100);
	mixedRatioSlider.setValue(50);
	mixedRatioSlider.setSliderStyle(Slider::Rotary);
	mixedRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(mixedRatioSlider);
	// Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

MixingKnobPluginAudioProcessorEditor::~MixingKnobPluginAudioProcessorEditor()
{
}

//==============================================================================
void MixingKnobPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MixingKnobPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	directoryToScanLabel.setBounds(10, 10, 300, 30);
	scanDirectoryButton.setBounds(320, 10, 50, 30);
	knownPluginsComboBox.setBounds(10, 60, 300, 30);
	openPluginButton.setBounds(320, 60, 50, 30);
	mixedRatioSlider.setBounds(60, 110, 60, 60);
}

//==============================================================================
void MixingKnobPluginAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &mixedRatioSlider) {
		processor.setGain(slider->getValue());
	}
}

//==============================================================================
void MixingKnobPluginAudioProcessorEditor::scanDirectoryClicked()
{
	FileChooser chooser("Select a directory to scan...", directoryToScanLabel.getText());
	if (chooser.browseForDirectory()) {
		auto dir = chooser.getResult();
		auto path = dir.getFullPathName();
		directoryToScanLabel.setText(path, NotificationType::dontSendNotification);
		processor.scanDirectory(path);
		updateKnownPluginsComboBox();
	}
}

void MixingKnobPluginAudioProcessorEditor::openPluginClicked()
{
	int index = knownPluginsComboBox.getSelectedId() - 1;
	processor.loadPluginWithIndex(index);
}

void MixingKnobPluginAudioProcessorEditor::updateKnownPluginsComboBox()
{
	int id = 0;
	for (auto pluginDescription : processor.getKnownPluginList()) {
		knownPluginsComboBox.addItem(pluginDescription->createIdentifierString(), ++id);
	}
}