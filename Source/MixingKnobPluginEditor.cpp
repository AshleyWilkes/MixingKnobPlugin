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
	p.addChangeListener(this);

	rescanDirectoriesButton.onClick = [this] { rescanDirectoriesClicked(); };
	rescanDirectoriesButton.setButtonText("Rescan directories");
	addAndMakeVisible(rescanDirectoriesButton);

	editDirectoriesButton.onClick = [this] { editDirectoriesClicked(); };
	editDirectoriesButton.setButtonText("Edit directories");
	addAndMakeVisible(editDirectoriesButton);

	updateKnownPluginsComboBox();
	knownPluginsComboBox.addListener(this);
	addAndMakeVisible(knownPluginsComboBox);

	openPluginButton.onClick = [this] { openPluginClicked(); };
	openPluginButton.setButtonText("Edit");
	addAndMakeVisible(openPluginButton);

	mixedRatioSlider.addListener(this);
	mixedRatioSlider.setRange(0, 100);
	mixedRatioSlider.setValue(processor.getGain());
	mixedRatioSlider.setSliderStyle(Slider::Rotary);
	mixedRatioSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
	addAndMakeVisible(mixedRatioSlider);

	addAndMakeVisible(mixedRatioLabel);
	// Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 300);
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
	//directoryToScanLabel.setBounds(10, 10, 300, 30);
	rescanDirectoriesButton.setBounds(30, 10, 150, 30);
	editDirectoriesButton.setBounds(320, 10, 150, 30);
	knownPluginsComboBox.setBounds(10, 60, 300, 30);
	openPluginButton.setBounds(320, 60, 150, 30);
	mixedRatioSlider.setBounds(60, 110, 60, 70);
	mixedRatioLabel.setBounds(60, 180, 60, 30);
}

//==============================================================================
void MixingKnobPluginAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
	if (slider == &mixedRatioSlider) {
		processor.setGain(slider->getValue());
		mixedRatioLabel.setText(
			"Mix: " + std::to_string(static_cast<int>(slider->getValue())) + "%",
			NotificationType::dontSendNotification);
	}
}

void MixingKnobPluginAudioProcessorEditor::comboBoxChanged(ComboBox* comboBox)
{
	if (comboBox == &knownPluginsComboBox) {
		int index = knownPluginsComboBox.getSelectedId() - 1;
		processor.loadPluginWithIndex(index);
	}
}

void MixingKnobPluginAudioProcessorEditor::changeListenerCallback(ChangeBroadcaster* changeBroadcaster)
{
	if (changeBroadcaster == &processor) {
		updateKnownPluginsComboBox();
	}
}

//==============================================================================
void MixingKnobPluginAudioProcessorEditor::rescanDirectoriesClicked()
{
	processor.rescanUsedDirectories();
}

void MixingKnobPluginAudioProcessorEditor::editDirectoriesClicked()
{
	processor.openUsedDirectoriesWindow();
}

void MixingKnobPluginAudioProcessorEditor::openPluginClicked()
{
	processor.openPluginWindow();
}

void MixingKnobPluginAudioProcessorEditor::updateKnownPluginsComboBox()
{
	int id = 0;
	knownPluginsComboBox.clear();
	for (auto pluginDescription : processor.getAvailablePluginsList()) {
		auto manufacturerString = "[" + pluginDescription.manufacturerName + "]";
		auto nameString = "[" + pluginDescription.name + "]";
		auto pluginDisplayString = manufacturerString + " - " + nameString;
		//auto pluginDisplayString = pluginDescription.createIdentifierString();
		knownPluginsComboBox.addItem(pluginDisplayString, ++id);
		if (processor.isInnerPluginSet()
					&& pluginDescription.isDuplicateOf(processor.getInnerPluginDescription())) {
			knownPluginsComboBox.setSelectedId(id);
		}
	}
	knownPluginsComboBox.repaint();
}